/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_javatechnics_rs232_Serial */

#ifndef _Included_com_javatechnics_rs232_Serial
#define _Included_com_javatechnics_rs232_Serial
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_javatechnics_rs232_Serial
 * Method:    getNativeLibraryVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_javatechnics_rs232_Serial_getNativeLibraryVersion
  (JNIEnv *, jobject);

/*
 * Class:     com_javatechnics_rs232_Serial
 * Method:    openSerialPort
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_javatechnics_rs232_Serial_openSerialPort
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_javatechnics_rs232_Serial
 * Method:    closeSerialPort
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_javatechnics_rs232_Serial_closeSerialPort
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
