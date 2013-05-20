TARGET := main.elf
CC     := msp430-gcc
CFLAGS := -Os -Wall -mmcu=msp430g2553
OBJS   += main.o uart.o fifo.o millis.o job.o nmea.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS)

clean:
	rm -rf $(TARGET) $(OBJS)

prog: $(TARGET)
	mspdebug -q rf2500 --force-reset "prog $(TARGET)"

size: $(TARGET)
	msp430-size -A $(TARGET)
