#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
#include <mqttclient.h> 
#include <mqtt/MQTTPacket.h>
#include <mqtt/transport.h>

unsigned char buf[200];

typedef int (*callback_t)(wangyonglin_socket_t *addr,unsigned char * data,int len) ;
static wangyonglin_socket_t __addr;
callback_t __callback;

int wangyonglin_mqtt_loop(int (*getfn)(unsigned char*, int));

int wangyonglin_mqtt_connect(wangyonglin_socket_t * addr,MQTTPacket_connectData *data,int (*getfn)(unsigned char*, int)){
    int len= 0,rc;
    int buflen = sizeof(buf);
    len = MQTTSerialize_connect(buf,buflen, data);
	rc = wangyonglin_socket_write(addr, buf, len);
     wangyonglin_mqtt_loop(getfn);
    return rc;

}
int wangyonglin_mqtt_subscribe(wangyonglin_socket_t * addr,MQTTString *topicString,int (*getfn)(unsigned char*, int)){
    int len,rc;
    int buflen=sizeof(buf);
    int msgid = 1;
	int req_qos = 0;
    len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, topicString, &req_qos);
	rc = wangyonglin_socket_write(addr, buf, len);
     wangyonglin_mqtt_loop(getfn);
     return rc;
}

int wangyonglin_mqtt_pingreq(wangyonglin_socket_t * addr,int (*getfn)(unsigned char*, int)){
     int len,rc;
    int buflen=sizeof(buf);
    len = MQTTSerialize_pingreq(buf, buflen);
	wangyonglin_socket_write(addr, buf, len);
    //wangyonglin_mqtt_loop(getfn);
 return rc;

}
int wangyonglin_mqtt_disconnect(wangyonglin_socket_t * addr,int (*getfn)(unsigned char*, int)){
     int len,rc;
    int buflen=sizeof(buf);
    len = MQTTSerialize_disconnect(buf, buflen);
	rc = wangyonglin_socket_write(addr, buf, len);
    wangyonglin_mqtt_loop(getfn);
   return rc;
}

int wangyonglin_mqtt_publish(wangyonglin_socket_t * addr,MQTTString topicName, unsigned char* payload, int payloadlen)
{   
    int len,rc;
    int buflen = sizeof(buf);
    len=MQTTSerialize_publish(buf,buflen,0, 0, 0, 0,topicName,payload, payloadlen);
    rc = wangyonglin_socket_write(addr, buf, len);
    return rc;
}
int wangyonglin_mqtt_loop(int (*getfn)(unsigned char*, int)){
    int buflen = sizeof(buf);
        unsigned short submsgid;
		int subcount;
		int granted_qos;
        unsigned char dup;
		int qos;
		unsigned char retained;
		unsigned short msgid;
		int payloadlen_in;
		unsigned char* payload_in;
		int rc;
       // bzero(payload_in,sizeof(payload_in));
        MQTTString receivedTopic;


        switch (MQTTPacket_read(buf,buflen,getfn))
        {
           case UNSUBSCRIBE:
                  fprintf(stdout,"UNSUBSCRIBE ... 0k\n");                
                break;
            case UNSUBACK:
                  fprintf(stdout,"UNSUBACK ... 0k\n");                
                break;  
            case PUBCOMP:
                  fprintf(stdout,"PUBCOMP ... 0k\n");                
                break;
            case SUBSCRIBE:
                  fprintf(stdout,"SUBSCRIBE ... 0k\n");                
                break;
            case PUBREC:
                  fprintf(stdout,"PUBREC ... 0k\n");                
                break;
             case PUBREL:
                  fprintf(stdout,"PUBREL ... 0k\n");                
                break;
             case CONNECT:
                  fprintf(stdout,"CONNECT ... 0k\n");                
                break;
             case PUBACK:
                  fprintf(stdout,"PUBACK ... 0k\n");                
                break;    
            case CONNACK:
                  fprintf(stdout,"CONNACK ... 0k\n");
                  unsigned char sessionPresent, connack_rc;
                    if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
                    {
                        fprintf(stderr,"Unable to connect, return code %d\n", connack_rc);
                        wangyonglin_log_error(LOG_ERROR,"Unable to connect, return code %d\n", connack_rc);
                        return -1;
                    }
                break;

            case DISCONNECT:
                fprintf(stdout,"DISCONNECT ... 0k\n");
                break;
            case PINGREQ:
                fprintf(stdout,"PINGREQ ... 0k\n");
            
                break;
            case PINGRESP:
                fprintf(stdout,"PINGRESP ... 0k\n");
            
                break;
            case SUBACK:
                fprintf(stdout,"SUBACK ... ok\n");
                MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
                fprintf(stdout,"granted qos = %d\n", granted_qos);
                break;
            case PUBLISH:
                 fprintf(stdout,"PUBLISH ... ok\n");
                
			        rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
					&payload_in, &payloadlen_in, buf, buflen);
			//printf("message arrived %.*s\n", payloadlen_in, payload_in);
            if(__callback!=NULL){
                __callback(&__addr,payload_in,payloadlen_in);
            }
                break;
            default:
                break;
        }

        return 0;
}

int wangyonglin_mqttclient_addcallback(wangyonglin_socket_t * addr,int (*callback)(wangyonglin_socket_t * addr,unsigned char * data,int len)){
  
    __addr = *addr;
    __callback = callback;
    return 0;
}
