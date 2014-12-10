# Define the JDK top-level - used for building the JNI header files.
JDK_HOME=/usr/lib/jvm/jdk1.7.0
export JDK_HOME

# Define the Java project j232 classpath
JSERIAL_CLASSPATH=/home/$(USER)/NetBeansProjects/j232/target/classes
export JSERIAL_CLASSPATH

# Define the top-level Java package
TOP_LEVEL_PACKAGE=com.javatechnics.rs232
export TOP_LEVEL_PACKAGE

all clean libj232 jni_headers jni_headers_clean install:
	cd src && $(MAKE) $@

.PHONY: all clean jni_headers_clean install
