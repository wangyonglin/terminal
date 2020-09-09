#include <wangyonglin/config.h>
#include <wangyonglin/core.h>


int wangyonglin_socket_connect(wangyonglin_socket_t *addr,const char * host,int port){
    struct hostent *he;
    static struct timeval tv;
    bzero(addr, sizeof(addr));

     //通过gethostbyname()得到服务端的主机信息
     if((he = gethostbyname(host)) == NULL)
    {
        wangyonglin_log_error(LOG_ERROR,"gethostbyname->err: %s",host);
        return wangyonglin_result_failure;
    }
   
    //使用socket()创建套接字
    if((addr->sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        wangyonglin_log_error(LOG_ERROR,"socket->err: %s", inet_ntoa(*(struct in_addr *)he->h_addr));
        return wangyonglin_result_failure;
    }
     //初始化server地址信息
   
    addr->addr_in.sin_family = AF_INET;
    addr->addr_in.sin_port = htons(port);
    addr->addr_in.sin_addr = *((struct in_addr *)he->h_addr);
     //使用connect()函数来配置套接字，建立一个与TCP服务器的连接
    if(connect(addr->sockfd, (struct sockaddr *)& addr->addr_in, sizeof(addr->addr_in)) == -1)
    {
        wangyonglin_log_error(LOG_ERROR,"connect->err: %s",inet_ntoa(*(struct in_addr *)he->h_addr));
        return wangyonglin_result_failure;
    }
    tv.tv_sec = 1;  /* 1 second Timeout */
	tv.tv_usec = 0;  
	if(setsockopt(addr->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval))!=0){
        wangyonglin_log_error(LOG_ERROR,"setsockopt->err: %s",inet_ntoa(*(struct in_addr *)he->h_addr));
        return wangyonglin_result_failure;
    }
    return 0;
}


int wangyonglin_socket_disconnect(wangyonglin_socket_t *addr){
int rc;

	rc = shutdown(addr->sockfd, SHUT_WR);
	rc = recv(addr->sockfd, NULL, (size_t)0, 0);
	rc = close(addr->sockfd);

	return rc;
}

int wangyonglin_socket_write(wangyonglin_socket_t *addr, unsigned char* buf, int buflen)
{
	int rc = 0;
	rc = write(addr->sockfd, buf, buflen);
	return rc;
}
int wangyonglin_socket_recv(wangyonglin_socket_t *addr,unsigned char* buf, int count)
{
	int rc = recv(addr->sockfd, buf, count, 0);
	//printf("received %d bytes count %d\n", rc, (int)count);
	return rc;
}