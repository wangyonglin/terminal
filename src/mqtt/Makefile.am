AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libmqtt.la
libmqtt_la_SOURCES= \
    MQTTDeserializePublish.c  \
    MQTTSerializePublish.c  \
    MQTTUnsubscribeClient.c \
    MQTTConnectClient.c  \
    MQTTFormat.c  \
    MQTTSubscribeClient.c   \
    MQTTUnsubscribeServer.c     \
    MQTTConnectServer.c   \
    MQTTPacket.c   \
    MQTTSubscribeServer.c   \
    transport.c         
AM_CPPFLAGS=-I $(top_srcdir)/include/