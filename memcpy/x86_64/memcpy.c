#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <stdint.h>
#include  <time.h>
#include  <err.h>

#include  "memcpy.h"

#define CALCTIME(SNAME, ...)											\
	do {																\
		struct timespec sta_ts, end_ts;									\
		SYSCALLWRAP(clock_gettime, CLOCK_MONOTONIC, &sta_ts);			\
		for(size_t i=0; i<TESTLEN()/MALLOCLEN(); i++) {					\
			SNAME(__VA_ARGS__);											\
		}																\
		SYSCALLWRAP(clock_gettime, CLOCK_MONOTONIC, &end_ts);			\
		if (sta_ts.tv_nsec > end_ts.tv_nsec) {						\
			end_ts.tv_nsec += 1000*1000*1000;							\
			end_ts.tv_sec -= 1;										\
		}																\
		long long diff_time_usec = 1000*1000*(end_ts.tv_sec - sta_ts.tv_sec) + (end_ts.tv_nsec - sta_ts.tv_nsec) / (1000); \
		printf("[%lld.%06lld] "#SNAME"\n", diff_time_usec / (1000*1000), diff_time_usec % (1000*1000)); \
	} while(0)

#define TEST(func,dst,src,len)					\
	do {										\
		memset (dst,0x5a,len);					\
		memset (src,0xa5,len);					\
		CALCTIME(func,dst,src,len);				\
	} while(0)

#define TESTLEN()   (256*1024*1024)
#define MALLOCLEN() (g_malloclen)
static size_t g_malloclen;
static void my_main(size_t len) {
	char *p, *q;
	g_malloclen = len;
	p = malloc(len);
	q = malloc(len);

	TEST(memcpy,p,q,len);
	TEST(memcpy_linux_simple,p,q,len);
	TEST(memcpy_linux_orig,p,q,len);
	TEST(memcpy_linux_erms,p,q,len);

	free(q);
	free(p);
}

int main(){
	printf("### 32KB ###\n");
	my_main(32*1024);

	printf("### 256KB ###\n");
	my_main(256*1024);

	printf("### 2MB ###\n");
	my_main(2*1024*1024);

	printf("### 16MB ###\n");
	my_main(16*1024*1024);

	return 0;
}
