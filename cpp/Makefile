CC = g++
GPUCC = nvcc
CCFLAGS = -Wall -Wextra -Iinclude -std=c++17
GPUCCFLAGS = -Iinclude -std=c++17
TARGET = load_mri

st:
	$(CC) -g $(CCFLAGS) -o $(TARGET)_$@ src/st/*.cc

mt:
	$(CC) -g $(CCFLAGS) -o $(TARGET)_$@ src/mt/*.cc -pthread

st-o:
	$(CC) -O3 $(CCFLAGS) -o $(TARGET)_$@ src/st/*.cc

mt-o:
	$(CC) -O3 $(CCFLAGS) -o $(TARGET)_$@ src/mt/*.cc

cuda:
	$(GPUCC) -g $(GPUCCFLAGS) -o $(TARGET)_$@ src/cuda/*.cu

clean:
	rm -f $(TARGET)_*
