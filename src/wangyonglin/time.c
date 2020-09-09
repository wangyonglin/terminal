
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>



struct tm * wangyonglin_time_now(){
    time_t time_seconds = time(0);
    struct tm* now_time = localtime(&time_seconds);
    now_time->tm_year += 1900;
    now_time->tm_mon += 1;
    return now_time;
}

time_t old_t;
void __start(int timeout)
{
	time(&old_t);
	old_t += timeout/2 + 1;
}

int __loop(void)
{
time_t t;

	time(&t);
	if(t >= old_t)
	  	return 1;
	return 0;
}
void wangyonglin_timeout_init(wangyonglin_timeout_t * t){
    t->start=__start;
    t->loop=__loop;
}