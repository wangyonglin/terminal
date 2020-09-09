#ifndef _WANGYONGLIN_SOCKEt_H_INCLUDE_
#define _WANGYONGLIN_SOCKEt_H_INCLUDE_
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
struct wangyonglin_socket_s {
    struct sockaddr_in addr_in;
    int sockfd;
};

int wangyonglin_socket_connect(wangyonglin_socket_t *addr,const char * host,int port);
int wangyonglin_socket_disconnect(wangyonglin_socket_t *addr);
int wangyonglin_socket_write(wangyonglin_socket_t *addr, unsigned char* buf, int buflen);
int wangyonglin_socket_recv(wangyonglin_socket_t *addr,unsigned char* buf, int count);
#endif  /**_WANGYONGLIN_SOCKEt_H_INCLUDE_**/
