#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/types.h>
#include "snowflake.h"

#define TIMESTAMP_BIT 41
#define DATACENTER_BIT 5
#define MACHINE_BIT 5
#define INCRMENT_BIT 12

static uint64_t getTimestamp() {
    struct timeval tv;
    gettimeofday(&tv, 0);

    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
}

static uint64_t getMachineId() {
   return (uint64_t)getpid() % (2 ^ (MACHINE_BIT));
}

static uint64_t getDatacenterId(int datacenter_id) {
    return (uint64_t) datacenter_id % (2 ^ DATACENTER_BIT);
}

static uint64_t getIncrement() {
    static uint64_t i = 0;
    i = (i + 1) & ~(1 << INCRMENT_BIT);
    return i;
}

uint64_t getSnowflake(int datacenter_id) {
    uint64_t t = getTimestamp();
    uint64_t d = getDatacenterId(datacenter_id);
    uint64_t m = getMachineId();
    uint64_t i = getIncrement();

    return (t << (DATACENTER_BIT + MACHINE_BIT + INCRMENT_BIT))
         + (d << (MACHINE_BIT + INCRMENT_BIT))
         + (m << INCRMENT_BIT)
         + i;
}