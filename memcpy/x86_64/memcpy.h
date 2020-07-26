#define MEMSET(val) memset(&val,0,sizeof(val))
#define SYSCALLWRAP(SNAME, ...)								\
	do {													\
		int ret;											\
		ret = SNAME(__VA_ARGS__);							\
		if (ret) {											\
			err(1, #SNAME);									\
		}													\
	} while(0)

extern void *memcpy_linux_simple(void *dest, const void *src, size_t n);
extern void *memcpy_linux_orig(void *dest, const void *src, size_t n);
extern void *memcpy_linux_erms(void *dest, const void *src, size_t n);
