CC = g++
CCFLAGS = -Wall -Wextra -Iinclude
TARGET = load_mri

all:
	$(CC) -g $(CCFLAGS) -o $(TARGET) src/*.cc

optim2:
	$(CC) -O2 $(CCFLAGS) -o $(TARGET) src/*.cc
	
optim3:
	$(CC) -O3 $(CCFLAGS) -o $(TARGET) src/*.cc
	
clean:
	rm -f $(TARGET)