JNI_INCLUDE = /usr/lib/jvm/jdk1.7.0/include

all: libj232

libj232: version.c
	gcc -o libj232.so -fPIC -I$(JNI_INCLUDE) -I$(JNI_INCLUDE)/linux -shared version.c

clean:
	-rm *.so
	-rm *.o

.PHONY: all clean
