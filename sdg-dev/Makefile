
JAVA_HOME=C:\\Arquivos de programas\\Java\\jdk1.5.0_04
CPP_HOME=C:\\Dev-Cpp\\include
INCLUDES=-I"$(JAVA_HOME)\\include" -I"$(JAVA_HOME)\\include\\win32"

CPP=g++
CFLAGS=-Wall -g $(INCLUDES)
JNI_CFLAGS=-D_JNI_IMPLEMENTATION_

JNI_LIBS=-Wl,--kill-at -L. -ljawt
LIBS=

all: JFrameEx.dll

JFrameEx.o: JFrameEx.cpp JFrameEx.h
	$(CPP) $(JNI_CFLAGS) $(CFLAGS) -c JFrameEx.cpp -o JFrameEx.o
	
JFrameEx.dll: JFrameEx.o
	$(CPP) $(JNI_LIBS) $(LIBS) -shared JFrameEx.o jawt.lib -o JFrameEx.dll

clean:
	rm -f JFrameEx.o
	rm -f JFrameEx.dll
