#include "redis.h"
#include "sds.h"
#include <math.h> /* isnan(), isinf() */
#include "snowflake.h"
#include "objectid.h"
#include "serialid.h"

void getHackCommnd(redisClient *c) {
    if (!strcasecmp(c->argv[1]->ptr, "step")) {
        int oldargc = c->argc;
        sds old = c->argv[0]->ptr;
        // fix aof record command
        c->argc = 2;
        c->argv[0]->ptr = c->argv[1]->ptr;
        c->argv[1]->ptr = c->argv[2]->ptr;

        stepCommand(c);

        // recover origin argv in case memory leak
        c->argc = oldargc;
        c->argv[2]->ptr = c->argv[1]->ptr;
        c->argv[1]->ptr = c->argv[0]->ptr;
        c->argv[0]->ptr = old;
    } else if (!strcasecmp(c->argv[1]->ptr, "snowflake")) {
        snowflakeCommand(c);
    } else if (!strcasecmp(c->argv[1]->ptr, "objectid")) {
        objectidCommand(c);
    } else if (!strcasecmp(c->argv[1]->ptr, "serialid")) {
        serialidCommand(c);
    } else {
        snowflakeCommand(c);
    }
}

void stepIncrCommand(redisClient *c, long long incr) {
    long long value, oldvalue;
    robj *o, *new;

    o = lookupKeyWrite(c->db,c->argv[1]);
    if (o != NULL && checkType(c,o,REDIS_STRING)) return;
    if (getLongLongFromObjectOrReply(c,o,&value,NULL) != REDIS_OK) return;


    oldvalue = value;
    if ((incr < 0 && oldvalue < 0 && incr < (LLONG_MIN-oldvalue)) ||
        (incr > 0 && oldvalue > 0 && incr > (LLONG_MAX-oldvalue))) {
        addReplyError(c,"increment or decrement would overflow");
        return;
    }
    value += incr;

    if (o && o->refcount == 1 && o->encoding == REDIS_ENCODING_INT &&
        (value < 0 || value >= REDIS_SHARED_INTEGERS) &&
        value >= LONG_MIN && value <= LONG_MAX)
    {
        new = o;
        o->ptr = (void*)((long)value);
    } else {
        new = createStringObjectFromLongLong(value);
        if (o) {
            dbOverwrite(c->db,c->argv[1],new);
        } else {
            unsigned char *value;
            unsigned int vlen;
            if (zipmapGet(server.step,(unsigned char*) c->argv[1]->ptr, sdslen(c->argv[1]->ptr), &value,&vlen)) {
                new->ptr = (void *)((long)strtoll((const char *)value, NULL, 10));
            }
            dbAdd(c->db,c->argv[1],new);
        }
    }
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(REDIS_NOTIFY_STRING,"incrby",c->argv[1],c->db->id);
    server.dirty++;
    addReply(c,shared.colon);
    addReply(c,new);
    addReply(c,shared.crlf);
}

void stepCommand(redisClient *c) {
    stepIncrCommand(c,server.step_offset);
}

void snowflakeCommand(redisClient *c) {
    long long snowflake = (long long)getSnowflake(server.datacenter_id);
    robj o = {
        .type = REDIS_STRING,
        .ptr = (void*)((long)snowflake),
        .encoding = REDIS_ENCODING_INT,
        .refcount = 1
    };
    addReply(c,shared.colon);
    addReply(c,&o);
    addReply(c,shared.crlf);
}

void objectidCommand(redisClient *c) {
    uint8_t objectid[12] = {0};
    getObjectid(server.datacenter_id, objectid);

    for (int i = 0; i < 24; i+=2) {
        server.objectid[i] = objectid[i/2] / 16;
        server.objectid[i+1] = objectid[i/2] % 16;

        server.objectid[i] += server.objectid[i] > 9 ? 'A' - 10 : + '0';
        server.objectid[i+1] += server.objectid[i+1] > 9 ? 'A' - 10 : + '0';
    }

    robj o = {
        .type = REDIS_STRING,
        .ptr = (void*)server.objectid,
        .encoding = REDIS_ENCODING_RAW,
        .refcount = 1
    };

    addReplyBulk(c, &o);
}

void serialidCommand(redisClient *c) {
	long long seriaid = (long long)getSerialid(server.datacenter_id);
	robj o = {
	        .type = REDIS_STRING,
	        .ptr = (void*)((long)seriaid),
	        .encoding = REDIS_ENCODING_INT,
	        .refcount = 1
	    };
	addReply(c,shared.colon);
	addReply(c,&o);
	addReply(c,shared.crlf);
}
