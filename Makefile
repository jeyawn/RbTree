CC = gcc 
LD = gcc

SRCS = $(wildcard *.c)

OBJS = $(patsubst %c, %o, $(SRCS))

TARGET = RbTreeTest

.PHONY:all clean
all: $(TARGET)
$(TARGET): $(OBJS)
	$(LD) -o $@ $^

%o:%c
	$(CC) -c $^
clean:
	rm -f $(OBJS) $(TARGET)	