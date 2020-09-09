#ifndef _MQTTCLIENT_H_INCLUDE_
#define _MQTTCLIENT_H_INCLUDE_
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

typedef struct {
    int (*publish)(unsigned char * data , int len);
}wangyonglin_mqttclient_callback_t;

#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>




int wangyonglin_mqtt_connect(wangyonglin_socket_t * addr,MQTTPacket_connectData *data,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_subscribe(wangyonglin_socket_t * addr,MQTTString *topicString,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_pingreq(wangyonglin_socket_t * addr,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_disconnect(wangyonglin_socket_t * addr,int (*getfn)(unsigned char*, int));
int wangyonglin_mqtt_publish(wangyonglin_socket_t * addr,MQTTString topicName, unsigned char* payload, int payloadlen);
int wangyonglin_mqttclient_addcallback(wangyonglin_socket_t * addr,int (*callback)(wangyonglin_socket_t * addr,unsigned char * data,int len));
#endif