#ifndef __SERIALID_H
#define __SERIALID_H
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

uint64_t getSerialid(int datacenter_id);
#endif
