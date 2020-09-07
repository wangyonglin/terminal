#ifndef _WANGYONGLIN_MQTT_H_INCLUDE_
#define _WANGYONGLIN_MQTT_H_INCLUDE_
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

struct wangyonglin_mqtt_s
{
	char addr[128];
	unsigned int hort;
	char clientid[128];
	char username[128];
	char passwrod[128];
	char topic[128];
	int qos;
	unsigned int  timeout;
	unsigned int keepalive_interval; 
};


#endif /**_WANGYONGLIN_MQTT_H_INCLUDE_**/