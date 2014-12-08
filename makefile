SOURCES = main.c testthumb.c testarm.c

OBJS := $(patsubst %.c,%.o,$(SOURCES))

TARGET := test

CFLAGS ?= -O0

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

testarm.o: testarm.c
	$(CC) $(CFLAGS) -marm -c -o $@ $<

testthumb.o: testthumb.c
	$(CC) $(CFLAGS) -mthumb -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJS)

