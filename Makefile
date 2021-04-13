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
SRCDIR := src/cxx
INCDIR := include


TESTSRCDIR := test/cxx
BOOSTDIR := /usr/include/boost

BMSRCDIR := benchmark/cpp/src
BMINCDIR := benchmark/cpp/include

BIN := bin

IM2GRGOALS := im2gr-all im2gr-st im2gr-mt im2gr-cuda
BMGOALS := benchmark-all benchmark-st benchmark-mt benchmark-cuda
TESTGOALS := test

INCPATH = -I$(INCDIR)
BMINCPATH = -I$(BMINCDIR)

# ----
CXXFLAGS += $(INCPATH)
GPUCXXFLAGS += $(INCPATH)

__SRCCPU := $(wildcard $(SRCDIR)/*.cc)
__SRCCPU := $(filter-out $(SRCDIR)/main.cc, $(__SRCCPU))

__SRCGPU := $(wildcard $(SRCDIR)/cuda/*.cu)
__SRCGPU := $(filter-out $(SRCDIR)/cuda/main.cu, $(__SRCGPU))


# ---- all ----
all: im2gr-all test benchmark-all

list:
	@echo "flags:"
	@echo "\tOPT DEBUG"
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
	$(CC) $(CXXFLAGS) -o $(BIN)/$@ $(SRCDIR)/*.cc

im2gr-mt: | $(BIN)
	$(CC) $(CXXFLAGS) -DMULTITHREAD -o $(BIN)/$@ $(SRCDIR)/*.cc -pthread

im2gr-cuda: | $(BIN)
	$(GPUCC) $(GPUCXXFLAGS) -o $(BIN)/$@ $(SRCDIR)/*.cu

# ---- tests -----
test: | $(BIN)
	$(CC) $(CXXFLAGS) -DMULTITHREAD -o $(BIN)/im2gr-$@ $(__SRCCPU) $(TESTSRCDIR)/*.cc \
            -lboost_unit_test_framework -pthread

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
            $(__SRCGPU) $(BMSRCDIR)/cuda/*.cu

# ----
$(BIN):
	@mkdir -p $@


.PHONY: all im2gr-all test benchmark-all clean list

