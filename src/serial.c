#include "serial.h"

/*
* This function is simply a wrapper around the native 'open' function. Currently it reports only success or falure.
* Failure is represented by the return value of -1. Success is a non-zero, positive number that represents
* the underlying native file-descriptor. En extension to this function would be to throw an exception for the 
* specific error (as reported in errno) when -1 is returned.
*/
JNIEXPORT jint JNICALL
Java_com_javatechnics_rs232_Serial_openSerialPort (JNIEnv * env, jobject obj, jstring path, jint flags){
	jint return_value = -1;
	return return_value;
};
