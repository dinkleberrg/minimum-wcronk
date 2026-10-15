#ifndef IOCTL_H
#define IOCTL_H

#include "minemu/platform.h"

int32_t ioctl(int fd, uint32_t request, ...);

#endif