#define FOREACHTYPE(expr) \
	expr(uint8_t)		  \
	expr(uint16_t)		  \
	expr(uint32_t)		  \
	expr(uint64_t)		  \
	expr(__int128)

#define FOREACHLOOP(expr) \
	expr(1)				  \
	expr(2)				  \
	expr(4)				  \
	expr(6)				  \
	expr(8)

#define FOREACHTYPELOOP_INNER(expr,TYPE)	\
	expr(TYPE,1)							\
	expr(TYPE,2)							\
	expr(TYPE,4)							\
	expr(TYPE,6)							\
	expr(TYPE,8)

#define FOREACHTYPELOOP(expr)			\
	FOREACHTYPELOOP_INNER(expr,uint8_t)			\
	FOREACHTYPELOOP_INNER(expr,uint16_t)		\
	FOREACHTYPELOOP_INNER(expr,uint32_t)		\
	FOREACHTYPELOOP_INNER(expr,uint64_t)		\
	FOREACHTYPELOOP_INNER(expr,__int128)		\
	
#define MEMSET(val) memset(&val,0,sizeof(val))
#define SYSCALLWRAP(SNAME, ...)								\
	do {													\
		int ret;											\
		ret = SNAME(__VA_ARGS__);							\
		if (ret) {											\
			err(1, #SNAME);									\
		}													\
	} while(0)

