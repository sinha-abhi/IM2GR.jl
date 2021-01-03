CC = g++
CCFLAGS = -Wall -Wextra -Iinclude -std=c++17
TARGET = load_mri

all:
	$(CC) -g $(CCFLAGS) -o $(TARGET) src/*.cc

optim2:
	$(CC) -O2 $(CCFLAGS) -o $(TARGET) src/*.cc
	
optim3:
	$(CC) -O3 $(CCFLAGS) -o $(TARGET) src/*.cc
	
shared:
	$(CC) -shared -fPIC -Iinclude src/*.cc -o $(TARGET).so
	
clean:
	rm -f $(TARGET)