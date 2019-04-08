TARGET = elevator

# Main
HDRS = \
		./include

SRCS = \
		./*.c

.PHONY: all main clean

all: main

main: $(SRCS)
		# $(CC) -Wall -Wextra -Werror $(addprefix -I, $(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS) -lm
		$(CC) $(addprefix -I, $(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS) -lm
clean:
	rm -f $(TARGET)
