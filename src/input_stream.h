/* 
 * File:   input_stream.h
 * Author: Kerry Billingham <java@avionicengineers.com>
 *
 * Created on 21 December 2014, 21:37
 */

#ifndef INPUT_STREAM_H
#define	INPUT_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include "jni/com_javatechnics_rs232_stream_SerialPortInputStream.h"

extern int throw_ioexception(JNIEnv *env, int error_number);

JNIEXPORT jint JNICALL
Java_com_javatechnics_rs232_stream_SerialPortInputStream_readNative (JNIEnv * env,\
                                                                    jobject obj,\
                                                                    jint fileDescriptor,\
                                                                    jbyteArray buffer,\
                                                                    jint offset,\
                                                                    jint length);
#endif	/* INPUT_STREAM_H */

