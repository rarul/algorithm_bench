#include  <stdio.h>
#include  <time.h>

static int val_fibo(int n) {
	if (n < 2) {
		return n;
	} else {
		return val_fibo(n-1) + val_fibo(n-2);
	}
}

static void do_fibo(int n) {
	struct timespec sta_time, end_time;

	clock_gettime(CLOCK_MONOTONIC, &sta_time);
	int result_fibo = val_fibo(n);
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	if (sta_time.tv_nsec > end_time.tv_nsec) {
		end_time.tv_nsec += 1000*1000*1000;
		end_time.tv_sec -= 1;
	}
	long long diff_time_usec = 1000*1000*(end_time.tv_sec - sta_time.tv_sec)
		+ (end_time.tv_nsec - sta_time.tv_nsec) / (1000);
	printf("[%lld.%06lld] fibo(%d) = %d\n", diff_time_usec / (1000*1000),
		   diff_time_usec % (1000*1000), n, result_fibo);

}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))){
	do_fibo(38);
	do_fibo(39);
	do_fibo(40);
	do_fibo(41);
	return 0;
}
