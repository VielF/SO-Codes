#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdbool.h>
#include "fat16.h"

bool cstr_to_fat32wnull(char *filename, char output[FAT32STR_SIZE_WNULL]);

#endif
