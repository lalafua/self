CC = gcc
CFLAGS = -Wall -Wextra -g
#LDFLAGS = -pthread

TARGET = producer_consumer

SRC = producer_consumer.c

all: ${TARGET}

$(TARGET):$(SRC)
	$(CC) $(CFLAGS) -o $@ $<

producer_consumer_test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all producer_consumer_test clean