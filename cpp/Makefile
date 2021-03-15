CC = g++
GPUCC = nvcc
CCFLAGS = -Wall -Wextra -Iinclude -std=c++17
GPUCCFLAGS = -Iinclude -std=c++17
TARGET = build_graph

st:
	$(CC) -g $(CCFLAGS) -DDEBUG -o $(TARGET)_$@ src/cpu/*.cc

mt:
	$(CC) -g $(CCFLAGS) -DMULTITHREAD -DDEBUG -o $(TARGET)_$@ src/cpu/*.cc -pthread

st-o:
	$(CC) -O3 $(CCFLAGS) -o $(TARGET)_$@ src/cpu/*.cc

mt-o:
	$(CC) -O3 $(CCFLAGS) -DMULTITHREAD -o $(TARGET)_$@ src/mt/*.cc

cuda:
	$(GPUCC) -g $(GPUCCFLAGS) -o $(TARGET)_$@ src/cuda/*.cu

clean:
	rm -f $(TARGET)_*
