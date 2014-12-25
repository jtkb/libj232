#include "input_stream.h"

JNIEXPORT jint JNICALL 
Java_com_javatechnics_rs232_stream_SerialPortInputStream_readNative (JNIEnv * env,
                                                                    jobject obj,
                                                                    jint fileDescriptor,
                                                                    jbyteArray buffer,
                                                                    jint offset,
                                                                    jint length){
    unsigned char n_buffer[length-offset];
    syslog(LOG_USER | LOG_DEBUG, "Java Buffer Length : %d Offset: %d FileDescriptor: %d", length, offset, fileDescriptor);
    int result = read(fileDescriptor, n_buffer, length - offset);
    syslog(LOG_USER | LOG_DEBUG, "Read %d bytes.", result);
    if (result == -1){
        throw_ioexception(env, errno);
    } else {
        (*env)->SetByteArrayRegion(env, buffer, offset, result, n_buffer);
    }
    return result;
    //return 0;
}
