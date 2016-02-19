#ifndef __SNOWFLASK_H
#define __SNOWFLASK_H
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

uint64_t getSnowflake(int datacenter_id);

#endif
