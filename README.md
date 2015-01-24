libj232
=======

This repository is the native library that compliments a Java library, j232, which allows the use of serial ports on the Linux platform from Java source code.

This library is one of two components that enable access to serial ports:

The Java package:   
https://github.com/jtkb/j232  

This native library:  
https://github.com/jtkb/libj232

Currently the two libraries are two separate repositories on GitHub.

Requirements
------------

##### JDK
By default the make file expects a Java Development Kit (JDK) to be located at 
/usr/lib/jvm/jdk1.7.0

Although the location implies Java 7 is required, the native library should be agnostic to the Java version. If your JDK location is different you may override the default with the command line option to make:  
`make JDK_HOME=<location_to_your_JDK_root_directory>`

##### j232
By default the make file expects the location of the j232 class files to be located under :

`/home/$(USER)/NetBeansProjects/j232/target/classe`  

The location of j232 is only required if you are recompiling the JNI headers following a change to any of the native method signatures in j232. You may override the default location of j232's class files by providing the command line option to make:  

`make JSERIAL_CLASSPATH=</path/to/j232/classes>`  

Building
--------
To build libj232 issue the following command:  

`make`

Assuming no errors occur, the library can be installed system-wide with the following command (assumes running as root otherwise preceed the command with `sudo`):  

`make install`


