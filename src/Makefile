JNI_INCLUDE = $(JDK_HOME)/include

all: libj232

libj232: version.c serial.c
	gcc -o libj232.so -fPIC -I$(JNI_INCLUDE) -I$(JNI_INCLUDE)/linux -shared version.c serial.c

jni_headers: jni_headers_clean
	$(JDK_HOME)/bin/javah -jni -classpath $(JSERIAL_CLASSPATH) -d $(PWD)/jni $(TOP_LEVEL_PACKAGE).Serial

jni_headers_clean:
	-rm -rf jni

clean:
	-rm *.so
	-rm *.o

.PHONY: all clean jni_headers_clean jni_headers
