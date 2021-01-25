CC = g++
GPUCC = nvcc
CCFLAGS = -Wall -Wextra -Iinclude -std=c++17
GPUCCFLAGS = -Iinclude -std=c++17
TARGET = load_mri
CPUTARGET = $(TARGET)_cpu
GPUTARGET = $(TARGET)_gpu

cpu:
	$(CC) -g $(CCFLAGS) -o $(CPUTARGET) src/cpu/*.cc

cpu_optim:
	$(CC) -O3 $(CCFLAGS) -o $(CPUTARGET) src/cpu/*.cc

cpu_shared:
	$(CC) -shared -fPIC -Iinclude src/cpu/*.cc -o $(CPUTARGET).so
	
gpu:
	$(GPUCC) -g $(GPUCCFLAGS) -o $(GPUTARGET) src/gpu/*.cu

clean:
	rm -f $(TARGET) $(CPUTARGET) $(GPUTARGET) $(TARGET).so $(CPUTARGET).so