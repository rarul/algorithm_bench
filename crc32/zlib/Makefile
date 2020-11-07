TARGETS = myzlibcrc32 myzlibcrc32dynamic myzlibcrc32byfour myzlibcrc32dynamicbyfour
CFLAGS += -DZ_PREFIX_SET
include ../../mk/simple_compile.mk


myzlibcrc32: myzlibcrc32.o zlib/crc32.o


ADDCFLAGS := $(CFLAGS) -DMAKECRCH
myzlibcrc32dynamic: myzlibcrc32dynamic.o zlib/crc32dynamic.o
myzlibcrc32dynamic.o: myzlibcrc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^
zlib/crc32dynamic.o: zlib/crc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^


ADDCFLAGS := $(CFLAGS) -DZ_U4=unsigned
myzlibcrc32byfour: myzlibcrc32byfour.o zlib/crc32byfour.o
myzlibcrc32byfour.o: myzlibcrc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^
zlib/crc32byfour.o: zlib/crc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^


ADDCFLAGS := $(CFLAGS) -DMAKECRCH -DZ_U4=unsigned
myzlibcrc32dynamicbyfour: myzlibcrc32dynamicbyfour.o zlib/crc32dynamicbyfour.o
myzlibcrc32dynamicbyfour.o: myzlibcrc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^
zlib/crc32dynamicbyfour.o: zlib/crc32.c
	$(CC) -c $(ADDCFLAGS) -o $@ $^

## Note that we are using "clean::", not "clean:"
clean::
	$(RM) zlib/*.o
