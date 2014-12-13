#ifndef _SERIAL_H
#define _SERIAL_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <jni.h>
#include "jni/com_javatechnics_rs232_Serial.h"

const int universal_flag_array[] = {0x00001, 0x00002, 0x00004, 0x00008, \
                        0x00010, 0X00020, 0X00040, 0X00080, \
                        0X00100, 0X00200, 0X00400, 0X00800, \
                        0X01000, 0X02000, 0X04000, 0X08000, \
                        0X10000, 0X20000};

const int open_flags[] = { O_APPEND, O_ASYNC, O_CLOEXEC, O_CREAT, \
                            O_DIRECT, O_DIRECTORY, O_DSYNC, O_EXCL,
                            O_LARGEFILE, O_NOATIME, O_NOCTTY, O_NOFOLLOW, \
                            O_NONBLOCK, O_SYNC, O_TRUNC, O_RDONLY, \
                            O_WRONLY, O_RDWR };
const int number_open_flags = sizeof(open_flags) / sizeof(open_flags[0]);

int get_real_flags(const int java_flags[], const int native_flags[], \
                            const int selected_flags, const int size);

#endif
