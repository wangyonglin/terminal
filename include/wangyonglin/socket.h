#ifndef _WANGYONGLIN_SOCKEt_H_INCLUDE_
#define _WANGYONGLIN_SOCKEt_H_INCLUDE_
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

int wangyonglin_socket_connect(const char * host,int port);
int wangyonglin_socket_disconnect(int sockfd);
int wangyonglin_socket_write(int sockfd, unsigned char* buf, int buflen);
int wangyonglin_socket_recv(int sockfd,unsigned char* buf, int count);
#endif  /**_WANGYONGLIN_SOCKEt_H_INCLUDE_**/
