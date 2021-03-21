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
