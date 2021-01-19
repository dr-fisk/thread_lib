LIB := libuthread.a
OBJS := queue.o sem.o uthread.o context.o preempt.o

CC := gcc
CFLAGS = -Wall -Werror -Wextra -MMD

ifneq (%(V), 1)
Q = @
endif

all: $(LIB)

DEPS = $(patsubst %.o, %.d, $(OBJS))
-include $(DEPS)

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c $<

libuthread.a: $(OBJS)
	@echo "CC $@"
	$(Q)ar rcs $@ $^

clean:
	@echo "CLEAN"
	$(Q)rm $(OBJS) $(LIB) $(DEPS)