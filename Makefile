# Makefile for all C++ components of im2gr 
CC := g++
GPUCC := nvcc
CXXFLAGS := -Wall -Wextra -std=c++17
GPUCXXFLAGS := -std=c++17

DEBUG :=
OPT :=

ifdef DEBUG
  CXXFLAGS += -g -DDEBUG
  GPUCXXFLAGS += -g -DDEBUG
endif

ifdef OPT
  CXXFLAGS += -O2
endif

# ---- directories ----
SRCDIR := im2gr/cpp/src
INCDIR := im2gr/cpp/include

TESTSRCDIR := test/cpp/src
TESTINCDIR := test/cpp/include

BMSRCDIR := benchmark/cpp/src
BMINCDIR := benchmark/cpp/include

BIN := bin

IM2GRGOALS := im2gr-all im2gr-st im2gr-mt im2gr-cuda
BMGOALS := benchmark-all benchmark-st benchmark-mt benchmark-cuda
TESTGOALS := test-all test-st test-mt test-cuda

INCPATH = -I$(INCDIR)
TESTINCPATH = -I$(TESTINCDIR)
BMINCPATH = -I$(BMINCDIR)

# ----
CXXFLAGS += $(INCPATH)
GPUCXXFLAGS += $(INCPATH)

__SRCCPU := $(SRCDIR)/cpu/*.cc
__SRCCPU += $(filter-out $(SRCDIR)/cpu/main.cc, $(__TESTSRCCPU))

__SRCGPU := $(SRCDIR)/cuda/*.cu
__SRCGPU += $(filter-out $(SRCDIR)/cuda/main.cu, $(__TESTSRCGPU))

# ---- all ----
all: im2gr-all test-all benchmark-all

list:
	@echo "im2gr:"
	@echo "\t$(IM2GRGOALS)"
	@echo "benchmark:"
	@echo "\t$(BMGOALS)"
	@echo "test:"
	@echo "\t$(TESTGOALS)"

clean:
	rm -rf bin

# ---- im2gr ----
im2gr-all: im2gr-st im2gr-mt im2gr-cuda

im2gr-st: | $(BIN)
	$(CC) $(CXXFLAGS) -o $(BIN)/$@ $(SRCDIR)/cpu/*.cc

im2gr-mt: | $(BIN)
	$(CC) $(CXXFLAGS) -DMULTITHREAD -o $(BIN)/$@ $(SRCDIR)/cpu/*.cc

im2gr-cuda: | $(BIN)
	$(GPUCC) $(GPUCXXFLAGS) -o $(BIN)/$@ $(SRCDIR)/cuda/*.cu

# ---- tests -----
test-all: test-st test-mt test-cuda

test-st: | $(BIN)
	$(CC) $(CXXFLAGS) $(TESTINCPATH)/cpu -o $(BIN)/im2gr-$@ \
    $(__SRCCPU) $(TESTSRCDIR)/cpu/*-st.cc

test-mt: | $(BIN)
	$(CC) $(CXXFLAGS) $(TESTINCPATH)/cpu -DMULTITHREAD -o $(BIN)/im2gr-$@ \
    $(__SRCCPU) $(TESTSRCDIR)/cpu/*-mt.cc -pthread

test-cuda: | $(BIN)
	$(GPUCC) $(GPUCXXFLAGS) $(TESTINCPATH)/cuda -o $(BIN)/im2gr-$@ \
    $(__SRCGPU) $(TESTSRCDIR)/cuda/*.cu

# ---- benchmark ----
benchmark-all: benchmark-st benchmark-mt benchmark-cuda

benchmark-st: | $(BIN)
	$(CC) $(CXXFLAGS) $(BMINCPATH)/cpu -o $(BIN)/im2gr-$@ \
    $(__SRCCPU) $(BMSRCDIR)/cpu/*-st.cc

benchmark-mt: | $(BIN)
	$(CC) $(CXXFLAGS) $(BMINCPATH)/cpu -DMULTITHREAD -o $(BIN)/im2gr-$@ \
    $(__SRCCPU) $(BMSRCDIR)/cpu/*-mt.cc -pthread

benchmark-cuda: | $(BIN)
	$(GPUCC) $(GPUCXXFLAGS) $(BMINCPATH)/cuda -o $(BIN)/im2gr-$@ \
    $(__SRCPU) $(BMSRCDIR)/cuda/*.cu

# ----
$(BIN):
	mkdir -p $@


.PHONY: all im2gr-all test-all benchmark-all clean list

