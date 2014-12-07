#include "version.h"

JNIEXPORT jstring JNICALL
Java_com_javatechnics_rs232_Serial_getNativeLibraryVersion(JNIEnv * env, jobject obj){
	return (*env)->NewStringUTF(env, versionNumber);
}
