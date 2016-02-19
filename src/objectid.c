#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/types.h>
#include "objectid.h"

#define TIMESTAMP_BIT (32)
#define DATACENTER_BIT (24)
#define MACHINE_BIT (16)
#define INCRMENT_BIT (24)

static uint64_t getTimestamp() {
    struct timeval tv;
    gettimeofday(&tv, 0);

    return (uint64_t)tv.tv_sec;
}

static uint64_t getMachineId() {
   return (uint64_t)getpid() % (2 << (MACHINE_BIT));
}

static uint64_t getDatacenterId(int datacenter_id) {
    return (uint64_t) datacenter_id % (2 << DATACENTER_BIT);
}

static uint64_t getIncrement() {
    static uint64_t i = 0;
    i = (i + 1) & ~(1 << INCRMENT_BIT);
    return i;
}

void getObjectid(int datacenter_id, uint8_t *objectid) {
    uint64_t t = getTimestamp();
    uint64_t d = getDatacenterId(datacenter_id);
    uint64_t m = getMachineId();
    uint64_t i = getIncrement();

    //time
    objectid[0] = (uint8_t)((t & 0xFF000000) >> 24);
    objectid[1] = (uint8_t)((t & 0x00FF0000) >> 16);
    objectid[2] = (uint8_t)((t & 0x0000FF00) >> 8);
    objectid[3] = (uint8_t)(t & 0x000000FF);

    //datacneter
    objectid[4] = (uint8_t)((d & 0x00FF0000) >> 16);
    objectid[5] = (uint8_t)((d & 0x0000FF00) >> 8);
    objectid[6] = (uint8_t)(d & 0x000000FF);
    //machine
    objectid[7] = (uint8_t)((m & 0x0000FF00) >> 8);
    objectid[8] = (uint8_t)(m & 0x000000FF);
    //increment
    objectid[9] = (uint8_t)((i & 0x00FF0000) >> 16);
    objectid[10] = (uint8_t)((i & 0x0000FF00) >> 8);
    objectid[11] = (uint8_t)(i & 0x000000FF);
}
