#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
#include <mqttclient.h>
#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>
#include <signal.h>


static  wangyonglin_socket_t __addr;
int toStop = 0;

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

void stop_init(void)
{
	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);
}

int __defultcallback(unsigned char* buf, int count)
{
	int rc = recv(__addr.sockfd, buf, count, 0);
	return rc;
}
int __mqttclient_callback(wangyonglin_socket_t * addr,unsigned char * data , int len){

	wangyonglin_log_info("%s %.*s",inet_ntoa(addr->addr_in.sin_addr),len,data);

	return 1;
}
int main(int argc, char *argv[])
{
	wangyonglin_log_t log_t;
	wangyonglin_pid_t pid_t;
	wangyonglin_mqtt_t mqtt_t;

	wangyonglin_conf_open("/usr/local/terminal/conf/example.conf");
	strcpy(log_t.access,wangyonglin_conf_read("access_log"));
	strcpy(log_t.error,wangyonglin_conf_read("error_log"));
	strcpy(pid_t.pidname,wangyonglin_conf_read("terminal_pid"));
	strcpy(mqtt_t.addr,wangyonglin_conf_read("mqtt_address"));
	mqtt_t.hort=atoi(wangyonglin_conf_read("mqtt_hort"));
	strcpy(mqtt_t.clientid,wangyonglin_conf_read("mqtt_clientid"));
	strcpy(mqtt_t.username,wangyonglin_conf_read("mqtt_username"));
	strcpy(mqtt_t.passwrod,wangyonglin_conf_read("mqtt_passwrod"));
	strcpy(mqtt_t.topic,wangyonglin_conf_read("mqtt_topic"));
	mqtt_t.qos=atoi(wangyonglin_conf_read("mqtt_qos"));
	mqtt_t.timeout=atoi(wangyonglin_conf_read("mqtt_timeout"));
	mqtt_t.keepalive_interval=atoi(wangyonglin_conf_read("mqtt_keepalive_interval"));
	wangyonglin_log_init(&log_t);
	wangyonglin_pid_create(&pid_t);
	wangyonglin_timeout_t __tt;
	wangyonglin_timeout_init(&__tt);
	//wangyonglin_daemon();
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	MQTTString topicString = MQTTString_initializer;
	data.clientID.cstring = mqtt_t.clientid;
	data.willFlag=0;
	data.keepAliveInterval = mqtt_t.keepalive_interval;
	data.cleansession = 1;
	data.MQTTVersion = 3;
	data.username.cstring = mqtt_t.username;
	data.password.cstring = mqtt_t.passwrod;
	stop_init();
	 if(wangyonglin_socket_connect(&__addr,mqtt_t.addr,mqtt_t.hort) == -1){
		 wangyonglin_log_error(LOG_ERROR,"wangyonglin_socket_connect failed \n");
		exit(EXIT_FAILURE);
	 }

	wangyonglin_mqttclient_addcallback(&__addr,__mqttclient_callback);

	if(wangyonglin_mqtt_connect(&__addr,&data,__defultcallback)==-1){
		wangyonglin_log_error(LOG_ERROR,"wangyonglin_mqtt_connect failed \n");
		goto exit;
	 
	}
	
	__tt.start(mqtt_t.keepalive_interval);

	topicString.cstring = mqtt_t.topic;
	if(wangyonglin_mqtt_subscribe(&__addr,&topicString,__defultcallback)==-1){
		wangyonglin_log_error(LOG_ERROR,"wangyonglin_mqtt_subscribe failed \n");
		goto exit;
	}

	while(!toStop)
	{
		wangyonglin_mqtt_loop(__defultcallback);
		if(__tt.loop()!=0)
		{
			if(wangyonglin_mqtt_pingreq(&__addr,__defultcallback)==-1){
				wangyonglin_log_error(LOG_ERROR,"wangyonglin_mqtt_pingreq failed \n");
				goto exit;
			}
			__tt.start(mqtt_t.keepalive_interval);
		
		}
		
	}

	wangyonglin_log_info("wangyonglin_mqtt_disconnect");
	wangyonglin_mqtt_disconnect(&__addr,__defultcallback);

	exit:
		fprintf(stderr,"wangyonglin_socket_disconnect\n");
		wangyonglin_socket_disconnect(&__addr);
		
	return EXIT_SUCCESS;
}