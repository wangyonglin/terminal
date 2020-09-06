#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
#include <mqttclient.h>

#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>
#include <signal.h>
#define ADDRESS     "tcp://a1pfKiaC8Ee.iot-as-mqtt.cn-shanghai.aliyuncs.com:1883"
#define CLIENTID    "320921198808117132|securemode=3,signmethod=hmacsha1|"
#define USERNAME    "A001&a1pfKiaC8Ee"
#define PASSWORD    "A8D188259569BB9A7B385E6F4F97C7693E6ECAC1"
#define TOPIC       "/a1pfKiaC8Ee/A001/user/get"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
#define KEEPALIVE_INTERVAL 30
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
int main(int argc, char *argv[])
{
	wangyonglin_timeout_t __tt;
	wangyonglin_timeout_init(&__tt);
	char *host = "a1pfKiaC8Ee.iot-as-mqtt.cn-shanghai.aliyuncs.com";
	int port = 1883;
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	MQTTString topicString = MQTTString_initializer;
	data.clientID.cstring = CLIENTID;
	data.willFlag=0;
	data.keepAliveInterval = KEEPALIVE_INTERVAL;
	data.cleansession = 1;
	data.MQTTVersion = 3;
	data.username.cstring = USERNAME;
	data.password.cstring = PASSWORD;
	stop_init();
	 if((sockfd=wangyonglin_socket_connect(host,port)) == -1){
		 fprintf(stderr,"wangyonglin_socket_connect res %d \n",sockfd);
		 exit(EXIT_FAILURE);
	 }

	if(wangyonglin_mqtt_connect(sockfd,&data,__defultcallback)==-1){
		fprintf(stderr,"wangyonglin_mqtt_connect falt \n");
		goto exit;
	}

	__tt.start(KEEPALIVE_INTERVAL);
/* subscribe */
	topicString.cstring = TOPIC;
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
			__tt.start(KEEPALIVE_INTERVAL);
		
		}
		
	}

	fprintf(stdout,"wangyonglin_mqtt_disconnect\n");
	wangyonglin_mqtt_disconnect(sockfd,__defultcallback);
	exit:
		fprintf(stderr,"wangyonglin_socket_disconnect\n");
		wangyonglin_socket_disconnect(sockfd);
	return EXIT_SUCCESS;
}