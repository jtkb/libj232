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
        int native_flags = get_real_flags(open_flags, java_open_flags, \
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

/*
 * This function is a wrapper around the native tcsetattr() function.
 * Parameters:
 *      JNIEnv  :
 *      jobject : pointer to the calling Java object.
 *      jint    :file_descriptor    File descriptor of the (opened) serial port.
 *      jint    :term_action        Terminal action flags. See tcsetattr() docs.
 *      jobject :termios            Java TermIOS object that mimics the termios
 *                                  struct in termios.h
 * 
 *      returns :               0 upon success, throws an IOException if the 
 *                              terminal attributes could not be set or -1 if
 *                              an exception could not be thrown.
 */
JNIEXPORT jint JNICALL 
Java_com_javatechnics_rs232_Serial_setNativeTerminalAttributes (JNIEnv *env,
                                                                jobject obj,
                                                                jint file_descriptor,
                                                                jint term_action, 
                                                                jobject termios){
    int return_value = -1;
    jfieldID field_ids[5] = {0, 0, 0, 0, 0};
    int i = 0;
    struct termios l_termios;
    
    //Get the terminal action flags
    int termattr = get_real_flags(java_terminal_settings_flags,
                                    terminal_settings_flags,
                                    term_action,
                                    number_terminal_settings_flags);
    //Get the class of termios TODO: check termios is not null;
    jclass cls = (*env)->GetObjectClass(env, termios);
    //Now get all the field IDs for termios
    for (; i < JAVA_TERMIOS_FIELD_COUNT; i++){
        field_ids[i] = (*env)->GetFieldID(env,
                                            cls,
                                            java_termios_fields[i],
                                            java_termios_field_descriptors[i]);
        if (field_ids[i] == NULL){
            //Exception is already thrown by the JVM if the call fails
            return return_value;
        }
    }
    
    l_termios.c_iflag = get_real_flags(java_input_flags, \
                                        input_flags, \
                                        (*env)->GetIntField(env, termios, field_ids[0]), \
                                        number_input_flags);
    l_termios.c_oflag = get_real_flags(java_output_flags, \
                                        output_flags, \
                                        (*env)->GetIntField(env, termios, field_ids[1]), \
                                        number_output_flags);
    l_termios.c_cflag = get_real_flags(java_control_flags, \
                                        control_flags, \
                                        (*env)->GetIntField(env, termios, field_ids[2]), \
                                        number_control_flags);
    l_termios.c_lflag = (unsigned int)get_real_flags(java_line_flags, \
                                        line_flags, \
                                        (*env)->GetIntField(env, termios, field_ids[3]), \
                                        number_line_flags);
    
#ifdef DEBUG
    syslog(LOG_USER | LOG_DEBUG, "c_cflag = %u", l_termios.c_cflag);
#endif
    
    jbyteArray j_c_cc = (*env)->GetObjectField(env, termios, field_ids[4]);
    (*env)->GetByteArrayRegion(env, j_c_cc, 0, 32, (jbyte*)(l_termios.c_cc));
    
    return_value = tcsetattr(file_descriptor, termattr, &l_termios);
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
 * This function is a wrapper around the tcgetattr() function. See termios.h
 * 
 * @param env
 * @param obj
 * @param fileDescriptor integer value of the serial port file descriptor.
 * @return TermIOS Java object representing the terminal settings.
 * @throws IOException if an error occurs.
 */
JNIEXPORT jobject JNICALL 
Java_com_javatechnics_rs232_Serial_getNativeTerminalAttributes (JNIEnv *env, 
                                                                jobject obj,
                                                                jint fileDescriptor){
    jclass termiosClass;
    jmethodID cid;      //Constructor ID for TermIOS
    jfieldID field_ids[JAVA_TERMIOS_FIELD_COUNT];
    jobject returnObject = NULL;
    struct termios l_termios;
    tcflag_t* termios_flags[] = { &l_termios.c_iflag, &l_termios.c_oflag, \
                                    &l_termios.c_cflag, &l_termios.c_lflag };
    // An array of pointers to the Java flags used in TermIOS
    const int* const java_flags_array[] = {java_input_flags, java_output_flags, java_control_flags, java_line_flags};
    // An array of pointers to native flags used in termios structure.
    const int* const native_flags_array[] = {input_flags, output_flags, control_flags, line_flags};
    //An array of sizes of each array in the above two.
    int flags_array_sizes[] = {number_input_flags, number_output_flags, number_control_flags, number_line_flags};
    int i;
#ifdef DEBUG
    syslog(LOG_USER | DEBUG, "Entered getNativeTerminalAttributes." );
#endif
    termiosClass = (*env)->FindClass(env,TERMIOS_CLASS_STRING);
    if (termiosClass == NULL){
        //Throw exception and return null
        returnObject = NULL;
        
    }else{
        //Try to get the default constructor
        cid = (*env)->GetMethodID(env, termiosClass, "<init>", "()V");
        if (cid == NULL){
            returnObject = NULL;
        } else {
            returnObject = (*env)->NewObject(env, termiosClass, cid);
            if (returnObject == NULL){
                //Exception thrown. return NULL
            } else {
                // Get the termios structure for the fileDescriptor
                int result = tcgetattr(fileDescriptor, &l_termios);
#ifdef DEBUG
                syslog(LOG_USER | LOG_DEBUG, "termios struct: c_iflag:%d c_oflag:%d c_cflag:%d c_lflag:%d", l_termios.c_iflag, l_termios.c_oflag, l_termios.c_cflag, l_termios.c_lflag);
#endif
                if (result == -1){
                    // Throw exception.
                    jint error = errno;
                    jclass newIOException = (*env)->FindClass(env, \
                        "java/io/IOException");
                    if (newIOException != NULL){
                        (*env)->ThrowNew(env, newIOException, strerror(error));
                    } else {
                        returnObject = NULL;
                    }
                } else {
                     
                    int result = get_field_ids(env, termiosClass, \
                                                    java_termios_fields, \
                                                    java_termios_field_descriptors, \
                                                    field_ids, \
                                                    JAVA_TERMIOS_FIELD_COUNT);
                    if (result != 0){
                        // Exception will automatically be thrown in the Java code.
                        returnObject = NULL;
                    } else {
                        unsigned int flag = 0;
                        for (i = 0; i < JAVA_TERMIOS_FIELD_COUNT - 1; i++){
#ifdef DEBUG
                            syslog(LOG_USER | LOG_DEBUG, "termios.%s = %u", java_termios_fields[i], (unsigned int) *termios_flags[i]);
#endif
                            flag = get_java_flags(java_flags_array[i], \
                                                   native_flags_array[i],     \
                                                    (int) *termios_flags[i], \
                                                    flags_array_sizes[i]);
#ifdef DEBUG
                            syslog(LOG_USER | LOG_DEBUG, "Returned flag value: %d", flag);
#endif
                            (*env)->SetIntField(env, returnObject, field_ids[i], (int) flag);
                        }
                        //Set the control characters
                        jbyteArray j_c_cc = (*env)->GetObjectField(env, returnObject, field_ids[4]);
                        (*env)->SetByteArrayRegion(env, j_c_cc, 0, NCCS, l_termios.c_cc);
                    }
                }
            }
        }
    }
    
    return returnObject;
    
}
/**
 * This function is a wrapper around ioctl() and gets the serial port control
 * bits.
 * @param env Pointer to the JNI environment.
 * @param jobj the calling object.
 * @param file_descriptor the file descriptor of serial port.
 * @param flags the ioctl request flags.
 * @return the requested control bits or -1 if an error and IOException was
 * not thrown.
 * @throws IOException if an error occurs.
 */
JNIEXPORT jint JNICALL 
Java_com_javatechnics_rs232_Serial_getNativeModemControlBits (JNIEnv * env,
                                                    jobject jobj,
                                                    jint file_descriptor,
                                                    jint ioctl_request){
    int return_value = 0;
    unsigned int control_bits = 0;
    int native_request = get_native_value(java_modem_control_requests,
                                            modem_control_requests,
                                            ioctl_request,
                                            number_modem_control_requests);
    

    if (native_request !=-1){
#ifdef DEBUG
        syslog(LOG_USER | LOG_DEBUG, "Native request flag is: %x", native_request);
#endif
        ioctl(file_descriptor, native_request, &control_bits);
        if (return_value == -1){
            throw_ioexception(env, errno);
        } else {
            // Convert native to Java values.

            return_value = get_java_flags(java_modem_control_flags, 
                                            modem_control_flags,
                                            control_bits,
                                            number_modem_control_flags);

        }
    } else {
        return_value = -1;
        //TODO: Throw an exception
    }
    return return_value;
}

/**
 * This function is a wrapper around ioctl() and sets the serial port control
 * bits.
 * @param env Pointer to the JNI environment.
 * @param jobj the calling Java object.
 * @param fileDescriptor file descriptor of the serial port.
 * @param flags the modem control bits to set.
 * @return 0 if success -1 if an error occurs and an exception not thrown.
 * @throws IOException if an error occurs.
 */
JNIEXPORT jint JNICALL
Java_com_javatechnics_rs232_Serial_setNativeModemcontrolBits (JNIEnv * env,
                                                    jobject jobj,
                                                    jint fileDescriptor,
                                                    jint flags){
    int return_value = 0;
    int native_flags = get_real_flags(java_modem_control_flags,
                                        modem_control_flags,
                                        flags,
                                        number_modem_control_flags);
#ifdef DEBUG
    syslog(LOG_USER | LOG_DEBUG, "Native Modem Control Bits to Set: %x", native_flags);
#endif
    return_value = ioctl(fileDescriptor, TIOCMSET, &native_flags);
    if (return_value == -1)
        throw_ioexception(env, errno);
}
/**
 * A helper method that throws an IOException in the JVM.
 * @param env pointer to the JNI environment.
 * @param error_number error number, typically as reported in global <i>errno</i>.
 * @return 0 upon success or -1 if IOException could not be thrown.
 */
int throw_ioexception(JNIEnv *env, int error_number){
    int return_value = 0;
    jclass newIOException = (*env)->FindClass(env, \
                        "java/io/IOException");
    if (newIOException != NULL){
        (*env)->ThrowNew(env, newIOException, strerror(error_number));
    } else {
        return_value = -1;
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

/**
 * This is a helper function that converts a native flag int into a java
 * flag int.
 * @param java_flags an array of java flags
 * @param native_flags an array of native flags, equivalent index-for-index to
 * the java_flags array.
 * @param selected_flags the value of the native flag
 * @param size the size of java_flags and thus native flags.
 * @return the equivalent java flag value.
 */
int get_java_flags(const int java_flags[], const int native_flags[], \
                            const int selected_flags, const int size){
    int return_flags = 0, i = 0;
#ifdef DEBUG
    syslog(LOG_USER | LOG_DEBUG, "Selected flags : %d, size : %d", selected_flags, size);
#endif
    for (; i < size; i++){

        if ((selected_flags & native_flags[i]) == native_flags[i])
            return_flags |= java_flags[i];
#ifdef DEBUG
        syslog(LOG_USER | LOG_DEBUG, "Native Flag: %d, Java Flag: %d, Return Flag: %d", native_flags[i], java_flags[i], return_flags);
#endif
    }
    return return_flags;
}

/**
 * A helper function to obtain the field ids for a given class, filed names and
 * their descriptors.
 * @param env
 * @param cls
 * @param field_names A char array of field names to obatin IDs.
 * @param field_name_descriptors Java descriptors of the field types.
 * @param field_ids The array where the field IDs will be returned.
 * @param field_count The number of field IDs to obtain. This number should
 * equal the length of arrays field_names and field_name_descriptors.
 * @return 0 upon success or throws and exception in the JVM and returns -1.
 */
int get_field_ids(JNIEnv* env, jclass cls, const char* const field_names[], \
                                            const char* const field_name_descriptors[],
                                            jfieldID field_ids[], \
                                            const int field_count){
    int count, return_value = 0;
    for (count = 0; count < field_count; count++){
        field_ids[count] = (*env)->GetFieldID(env, cls, field_names[count], field_name_descriptors[count] );
        if (field_ids[count] == NULL){
            // Exception automatically thrown. break and return -1
            return_value = -1;
            break;
        }
    }
    return return_value;
}

/**
 * A helper function that gets single native flag value from an array of native flag 
 * values for a given java flag value.
 * @param java_values The array of Java flags from which java_value comes from.
 * @param native_flags The array of native flags that match index value for
 * index value the array of java values.
 * @param java_value the java flag for which a native flag value is sought
 * @param size the size of java_values array which should also be the size (as
 * a minimum) as native_flags array.
 * @return the native flag value or -1 if it cannot be found.
 */
int get_native_value(const int const java_values[], const int const native_flags[],
                            const int java_value, const int size){
    int return_value = -1, i = 0;
    for (; i < size; i++){
        if (java_value == java_values[i]){
            return_value = native_flags[i];
            break;
        }
    }
    return return_value;    
}