#ifndef __OBJECTID_H
#define __OBJECTID_H
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

void getObjectid(int datacenter_id, uint8_t *objectid);
#endif
