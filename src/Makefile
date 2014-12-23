JNI_INCLUDE = $(JDK_HOME)/include
all: libj232

install:
	-cp libj232.so /usr/lib/
	-chown 0:0 /usr/lib/libj232.so
	-chmod a-x /usr/lib/libj232.so

uninstall:
	-rm -f /usr/lib/libj232.so

debug: clean libj232.so
	
libj232: libj232.so

libj232.so: version.c serial.c input_stream.c
	cc -o libj232.so $(CPPFLAGS) $(DEBUG_CPPFLAGS) -fPIC -I$(JNI_INCLUDE) -I$(JNI_INCLUDE)/linux -shared input_stream.c version.c serial.c 

jni_headers: jni_headers_clean
	$(JDK_HOME)/bin/javah -jni -classpath $(JSERIAL_CLASSPATH) -d $(PWD)/jni $(TOP_LEVEL_PACKAGE).Serial
	$(JDK_HOME)/bin/javah -jni -classpath $(JSERIAL_CLASSPATH) -d $(PWD)/jni $(TOP_LEVEL_PACKAGE).stream.SerialPortInputStream

jni_headers_clean:
	-rm -rf jni

clean:
	-rm *.so
	-rm *.o

.PHONY: all clean jni_headers_clean jni_headers install uninstall
