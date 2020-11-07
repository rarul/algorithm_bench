
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <sys/stat.h>
#include  <sys/types.h>
#include  <sys/errno.h>

extern unsigned long update_crc(unsigned long crc,unsigned char *buf, int len);


static void usage(){
    printf("./checkcrc32 FILENAME\n");
    exit(1);
}

static ssize_t do_read (const int fd, void *buf, const size_t count) {
    ssize_t ret_size;
    size_t  proceed_size = 0;
    while (proceed_size < count) {
        ret_size = read(fd, buf+proceed_size, (count-proceed_size));
        if (ret_size < 0) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            if (proceed_size <= 0) {
                return -1;
            }
            break;
        } else if (ret_size == 0) {
            break;
        }
        proceed_size += ret_size;
    }
    return proceed_size;
}

#define BUFSIZE (64*1024)
static int do_main(const char *target_file) {
    ssize_t ret_read_size = 0;
    int fd = open (target_file, O_RDONLY);
    if (fd < 0) {
        perror(target_file);
        return 1;
    }

    unsigned char *buf = malloc(BUFSIZE);
	unsigned long val_crc32 = 0;
	while (1) {
		ret_read_size = do_read (fd, buf, BUFSIZE);
		if (ret_read_size < 0) {
			perror("read");
			break;
		} else if (ret_read_size == 0) {
			// EOF
			break;
		}
		val_crc32 = update_crc(val_crc32, buf, ret_read_size);
	}	
    free(buf);

    close(fd);

	if (ret_read_size != 0) {
		return 1;
	}
	printf("%08lx\n", val_crc32);
	return 0;
}
int main(int argc, char *argv[]) {
    char *target_file = NULL;

    if (argc < 2) {
        usage();
    }
    target_file = argv[1];

    return do_main(target_file);
}


