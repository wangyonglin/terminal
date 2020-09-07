#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
#include <mqttclient.h>
#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>
#include <signal.h>

static	int sockfd;
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
	int rc = recv(sockfd, buf, count, 0);
	//wangyonglin_log_info("received %d bytes count %d  \n", rc, (int)count);
	return rc;
}
int __mqtt_publistcallback(unsigned char * data , int len){

	wangyonglin_log_info("###MQTT RECV : %.*s",len,data);
	return 1;
}
int main(int argc, char *argv[])
{
	wangyonglin_log_t log_t;
	wangyonglin_pid_t pid_t;
	wangyonglin_mqtt_t mqtt_t;

	fprintf(stdout,"hello terminal !\n");
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
	wangyonglin_daemon();
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
	 if((sockfd=wangyonglin_socket_connect(mqtt_t.addr,mqtt_t.hort)) == -1){
		 fprintf(stderr,"wangyonglin_socket_connect res %d \n",sockfd);
		 exit(EXIT_FAILURE);
	 }

	if(wangyonglin_mqtt_connect(sockfd,&data,__defultcallback)==-1){
		fprintf(stderr,"wangyonglin_mqtt_connect falt \n");
		goto exit;
	}
	wangyonglin_mqttclient_addcallback(__mqtt_publistcallback);
	__tt.start(mqtt_t.keepalive_interval);

	topicString.cstring = mqtt_t.topic;
	if(wangyonglin_mqtt_subscribe(sockfd,&topicString,__defultcallback)==-1){
		fprintf(stderr,"wangyonglin_mqtt_subscribe falt \n");
		goto exit;
	}
	
	while(!toStop)
	{
		wangyonglin_mqtt_loop(__defultcallback);
		if(__tt.loop()!=0)
		{
			if(wangyonglin_mqtt_pingreq(sockfd,__defultcallback)==-1){
				fprintf(stderr,"wangyonglin_mqtt_pingreq falt \n");
				goto exit;
			}
			__tt.start(mqtt_t.keepalive_interval);
		
		}
		
	}

	fprintf(stdout,"wangyonglin_mqtt_disconnect\n");
	wangyonglin_mqtt_disconnect(sockfd,__defultcallback);

	exit:
		fprintf(stderr,"wangyonglin_socket_disconnect\n");
		wangyonglin_socket_disconnect(sockfd);
		
	return EXIT_SUCCESS;
}