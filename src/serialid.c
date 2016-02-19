#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "serialid.h"

#define DATETIME_BIT (38)
#define INCRMENT_BIT (15)
#define DATACENTER_BIT (2)
#define MACHINE_BIT (2)

static uint64_t getTensYear(int year) {
    while (year >= 100) {
        year -= 100;
    }

    return year;
}

static uint64_t getDateTime() {
	time_t rawtime;

	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	uint64_t year = getTensYear(timeinfo->tm_year); //2
	uint64_t month = timeinfo->tm_mon + 1; //2
	uint64_t day = timeinfo->tm_mday;  //2
	uint64_t hour = timeinfo->tm_hour; //2
	uint64_t min = timeinfo->tm_min;   //2
	uint64_t sec = timeinfo->tm_sec;   //2
	uint64_t f = year*10000000000 + month*100000000 + day*1000000 + hour*10000 + min*100 + sec;

    return f;
}

static uint64_t getIncrement() {
    static uint64_t i = 0;
    i = (i + 1) & ~(1 << INCRMENT_BIT);
    return i;
}

static uint64_t getDatacenterId(int datacenter_id) {
    return (uint64_t)datacenter_id % (2 << DATACENTER_BIT);
}

static uint64_t getMachineId() {
   return (uint64_t)getpid() % (2 << MACHINE_BIT);
}

uint64_t getSerialid(int datacenter_id) {
    uint64_t s = getDateTime();
    uint64_t d = getDatacenterId(datacenter_id);
    uint64_t m = getMachineId();
    uint64_t i = getIncrement();

    return s * 1000000 + (d << (MACHINE_BIT + INCRMENT_BIT))
         + (m << INCRMENT_BIT) + i;
}
