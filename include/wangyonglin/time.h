#ifndef _WANGYONGLIN_TIME_H_INCLUDE_
#define _WANGYONGLIN_TIME_H_INCLUDE_

#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

struct wangyonglin_time_s
{
    struct tm *t;
	struct timeval *tv;
};
struct  wangyonglin_timeout_s{
  void (*start)(int timeout);
  int (*loop)(void);
};

void wangyonglin_timeout_init(wangyonglin_timeout_t * t);

  struct tm * wangyonglin_time_now();
#endif /**_WANGYONGLIN_TIME_H_INCLUDE_**/
