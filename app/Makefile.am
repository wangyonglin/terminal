AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=terminal
terminal_SOURCES=main.c httpserver.c jsonresult.c mqttclient.c
AM_CPPFLAGS=-I ../include/
terminal_LDADD= \
    ../src/ccl/libccl.la \
    ../src/mqtt/libmqtt.la \
    ../src/cjson/libjson.la \
    ../src/wangyonglin/libcore.la \
    $(FLIBS)
LIBS += -lm -levent
