#include "serial.h"

/*
* This function is simply a wrapper around the native 'open' function. 
 * If the file open is succesful then the native file descriptor is returned. If
 * open is unsuccessful it will attempt to throw an IOException using the char*
 * returned by a call to strerror(). If the IOException class cannot be created
 * for some reason then the value of -1 will be returned, i.e. -1 will *only* be
 * returned if and only if open fails *and* the exception object cannot be
 * created.
 * Parameters:
 *      JNIEnv  :     
 *      jobject :
 *      jstring :path       The file system path to the serial port, 
 *                          e.g. "/dev/ttyS0"
 *      jint    :flags      Requested open flags.
 *  
 *      Return  :           If succesful the underlying file descriptor, 
 *                          or throws an IOException in the JVM or -1 if open()
 *                          failed AND IOException object could not be created.
 * 
*/
JNIEXPORT jint JNICALL
Java_com_javatechnics_rs232_Serial_openSerialPort (JNIEnv * env, jobject obj, jstring path, jint flags){
	jint return_value = -1;
        int native_flags = get_real_flags(open_flags, universal_flag_array, \
                                            flags, number_open_flags);
        const jbyte *c_path = (*env)->GetStringUTFChars(env, path, NULL);
        if (c_path != NULL){
            return_value = open(c_path, native_flags);
            if (return_value == -1){
                jint error = errno; //preserve the correct error number
                // Throw exception
                jclass newIOException = (*env)->FindClass(env, \
                        "java/io/IOException");
                if (newIOException != NULL){
                    (*env)->ThrowNew(env, newIOException, strerror(error));
                }
            }
        }
        
	return return_value;
};

/*
 * This function is a wrapper around the native close() function. The file 
 * closed is that of the passed in file descriptor. If the function is successful
 * it returns 0 to the calling function -1 otherwise. In the Java code it
 * throws an exception if -1 is returned from the native close() call. If
 * an IOException object cannot be created this function simply returns -1
 * indicating that both a file close and class instantiation error occurred.
 * Parameters:
 *      JNIEnv  :     
 *      jobject :
 *      jint    :flags      File descriptor of the file to close.
 *  
 *      Return  :           If succesful the underlying file descriptor, 
 *                          or throws an IOException in the JVM or -1 if open()
 *                          failed AND IOException object could not be created.
 */
JNIEXPORT jint JNICALL 
Java_com_javatechnics_rs232_Serial_closeSerialPort (JNIEnv *env, jobject obj, jint fd){
    jint return_value = -1;
    return_value = close(fd);
    if (return_value == -1){
        jint error = errno;
        jclass newIOException = (*env)->FindClass(env, \
                        "java/io/IOException");
        if (newIOException != NULL){
            (*env)->ThrowNew(env, newIOException, strerror(error));
        }
    }
    return return_value;
}

/**
 *  This is a helper function that converts Java-defined flag values to their
 * equivalent in native code and returns a native flag value.
 * @param java_flags An array of the flag values as defined in the Java code.
 * @param native_flags An array of the equivalent native flags.
 * @param selected_flags An integer value of the required Java-defined flags.
 * @param size The size of the java_flags array.
 * @return The converted Java-to-Native flags.
 */
int get_real_flags(const int java_flags[], \
                    const int native_flags[], \
                    const int selected_flags, \
                    const int size){
    int return_flag = 0, count = 0;
    for (; count < size; count++){
        if (selected_flags & java_flags[count])
            return_flag |= native_flags[count];
    }
    return return_flag;
}