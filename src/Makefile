all: skypetest
CC=g++

SKYPEKITHOME=../../../sdp-distro-desktop-skypekit_4.3.1.17_1899690/
# SKYPEKITSRCDIR=${SKYPEKITHOME}src/
SKYPEKITLIBDIR=${SKYPEKITHOME}interfaces/skype/cpp_embedded/build/

INCLUDEFLAGS= \
-I ${SKYPEKITHOME}interfaces/skype/cpp_embedded/src/api \
-I ${SKYPEKITHOME}interfaces/skype/cpp_embedded/src/ipc \
-I ${SKYPEKITHOME}interfaces/skype/cpp_embedded/src/types \
-I ${SKYPEKITHOME}interfaces/skype/cpp_embedded/src/platform/threading \
-I ${SKYPEKITHOME}ipc/cpp \
-I ${SKYPEKITHOME}ipc/cpp/platform/se \
-I ${SKYPEKITHOME}ipc/cpp/ssl/cyassl/include \
-I ${SKYPEKITHOME}ipc/cpp/ssl/cyassl/ctaocrypt/include \
-I ${SKYPEKITHOME}ipc/cpp/ssl/cyassl/include/openssl \
-I ${SKYPEKITHOME}ipc/cpp/VideoBuffers \
-I ${SKYPEKITHOME}interfaces/skype/cpp_embedded/src/platform/threading/pthread \
-I ../include/SkypeRTC

SKYPEKITLIB=${SKYPEKITLIBDIR}libskypekit-cppwrapper_2_lib.a ${SKYPEKITLIBDIR}libskypekit-cyassl_lib.a
# CFLAGS=-fPIC -Wall -O ${INCLUDEFLAGS} 
CFLAGS=-DDEBUG -fPIC -Wall -O ${INCLUDEFLAGS} 

LDFLAGS=-L ${SKYPEKITHOME}
OBJECTS=skypetest.o SkypeEx.o AccountEx.o ContactGroupEx.o ParticipantEx.o ConversationEx.o VideoEx.o

.cpp.o:
	${CC} ${CFLAGS} -c $<

skypetest: ${OBJECTS}
	${CC} ${LDFLAGS} -o skypetest ${OBJECTS} ${SKYPEKITLIB} 


clean:
	rm -rf *~ skypetest *.o streamlog.txt*