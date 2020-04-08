#include <sys/time.h>

double getCurrentTime(void);

double getCurrentTime(void)
{
    struct timeval t;
    struct timezone tz;

    gettimeofday(&t, &tz);

    return t.tv_sec * 1000.0 + t.tv_usec / 1000.0;
}