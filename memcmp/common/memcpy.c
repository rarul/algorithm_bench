#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <stdint.h>
#include  <time.h>
#include  <err.h>

#include  "memcpy.h"

#define MYMEMCMP_DEF(TYPE,LOOP)											\
	static void *my_memcpy_ ##TYPE ##_loop_ ##LOOP(void *dst, const void *src, size_t len) { \
		const TYPE *s = src;											\
		TYPE *d = dst;													\
		LOOPUNROLLING_##LOOP(TYPE)										\
		while (len>0) {													\
			len -= sizeof(TYPE);										\
			*d++ = *s++;												\
		}																\
		return dst;														\
	}

// no loop unrolling definition
#define LOOPUNROLLING_1(TYPE)

#define LOOPUNROLLING_2(TYPE)					\
	while (len >= 2*sizeof(TYPE)) {				\
		len -= 2*sizeof(TYPE);					\
		TYPE val0 = *s++;						\
		TYPE val1 = *s++;						\
		*d++ = val0;							\
		*d++ = val1;							\
	}

#define LOOPUNROLLING_4(TYPE)					\
	while (len >= 4*sizeof(TYPE)) {				\
		len -= 4*sizeof(TYPE);					\
		TYPE val0 = *s++;						\
		TYPE val1 = *s++;						\
		TYPE val2 = *s++;						\
		TYPE val3 = *s++;						\
		*d++ = val0;							\
		*d++ = val1;							\
		*d++ = val2;							\
		*d++ = val3;							\
	}

#define LOOPUNROLLING_6(TYPE)					\
	while (len >= 6*sizeof(TYPE)) {				\
		len -= 6*sizeof(TYPE);					\
		TYPE val0 = *s++;						\
		TYPE val1 = *s++;						\
		TYPE val2 = *s++;						\
		TYPE val3 = *s++;						\
		TYPE val4 = *s++;						\
		TYPE val5 = *s++;						\
		*d++ = val0;							\
		*d++ = val1;							\
		*d++ = val2;							\
		*d++ = val3;							\
		*d++ = val4;							\
		*d++ = val5;							\
	}

#define LOOPUNROLLING_8(TYPE)					\
	while (len >= 8*sizeof(TYPE)) {				\
		len -= 8*sizeof(TYPE);					\
		TYPE val0 = *s++;						\
		TYPE val1 = *s++;						\
		TYPE val2 = *s++;						\
		TYPE val3 = *s++;						\
		TYPE val4 = *s++;						\
		TYPE val5 = *s++;						\
		TYPE val6 = *s++;						\
		TYPE val7 = *s++;						\
		*d++ = val0;							\
		*d++ = val1;							\
		*d++ = val2;							\
		*d++ = val3;							\
		*d++ = val4;							\
		*d++ = val5;							\
		*d++ = val6;							\
		*d++ = val7;							\
	}

FOREACHTYPELOOP(MYMEMCMP_DEF)


#define ARG_PROCEED(length) do{ dst+=length; src+=length; len-=length; }while(0)
#define CALL_MEMCMP_BY_CASE(TYPE,LOOP)									\
	if ( (xor_addr & (sizeof(TYPE)-1)) == 0) {							\
		first_proceed_len = (sizeof(TYPE) - (dst_addr & (sizeof(TYPE)-1))) & sizeof(TYPE); \
		my_memcpy_uint8_t_loop_ ##LOOP (dst, src, first_proceed_len);	\
		ARG_PROCEED(first_proceed_len);									\
		main_proceed_len = len - (len % (sizeof(TYPE)));				\
		my_memcpy_ ##TYPE ##_loop_ ##LOOP (dst, src, main_proceed_len);	\
		ARG_PROCEED(main_proceed_len);									\
	}

#define MY_MEMCPY_FUNCTION(LOOP)										\
	static void *my_memcpy_loop_ ##LOOP(void *dst, const void *src, size_t len) { \
	size_t  first_proceed_len, main_proceed_len;						\
	void *original_dst = dst;											\
	const unsigned long dst_addr = (unsigned long)dst;					\
	const unsigned long src_addr = (unsigned long)src;					\
	const unsigned long xor_addr = dst_addr ^ src_addr;					\
	CALL_MEMCMP_BY_CASE(__int128,LOOP)									\
	else																\
		CALL_MEMCMP_BY_CASE(uint64_t,LOOP)								\
	else																\
		CALL_MEMCMP_BY_CASE(uint32_t,LOOP)								\
	else																\
		CALL_MEMCMP_BY_CASE(uint16_t,LOOP)								\
	my_memcpy_uint8_t_loop_ ##LOOP (dst, src, len);						\
	return original_dst;												\
}

FOREACHLOOP(MY_MEMCPY_FUNCTION)

#define CALCTIME(SNAME, ...)											\
	do {																\
		struct timespec sta_ts, end_ts;									\
		long diff_usec;													\
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
		CALCTIME(func,dst,src,len);				\
	} while(0)

#define TESTCASES1(LOOP)							\
	TEST(my_memcpy_loop_##LOOP, p, q, len);

#define TESTCASES2(TYPE,LOOP)							\
	TEST(my_memcpy_ ##TYPE ##_loop_##LOOP, p, q, len);

#define TESTLEN()   (256*1024*1024)
#define MALLOCLEN() (g_malloclen)
static size_t g_malloclen;
static void my_main(size_t len) {
	char *p, *q;
	g_malloclen = len;
	p = malloc(len);
	q = malloc(len);

	FOREACHLOOP(TESTCASES1)
	FOREACHTYPELOOP(TESTCASES2)
	TEST(memcpy,p,q,len);

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
