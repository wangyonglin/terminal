AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=terminal
terminal_SOURCES=main.c httpserver.c jsonresult.c mqttclient.c
AM_CPPFLAGS=-I ../include/
terminal_LDADD= \
    $(top_srcdir)/src/ccl/libccl.la \
    $(top_srcdir)/src/mqtt/libmqtt.la \
    $(top_srcdir)/src/cjson/libjson.la \
    $(top_srcdir)/src/wangyonglin/libcore.la \
    $(FLIBS)
LIBS += -lm -levent
