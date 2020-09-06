#ifndef _MQTTCLIENT_H_INCLUDE_
#define _MQTTCLIENT_H_INCLUDE_
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>

int wangyonglin_mqtt_connect(int sockfd,MQTTPacket_connectData *data,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_subscribe(int sockfd,MQTTString *topicString,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_pingreq(int sockfd,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_disconnect(int sockfd,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_publish(int sockfd,MQTTString topicName, unsigned char* payload, int payloadlen);

#endif