CXX      := g++
CPPINCS  := ./inc
CPPSRCS  := ./src .
CPPOBJS  := ./obj
CPPFLAGS := -I$(CPPINCS)
CXXFLAGS := -Wall Werror -g -O0

BLUE  := \033[1;34m
CYAN  := \033[1;36m
GREEN := \033[1;32m
YELL  := \033[1;33m
NC    := \033[0m

.PHONY: build all clean info

vpath %.cpp $(CPPSRCS)
vpath %.h $(CPPINCS)

# addsuffix is used to append a suffix to each word in the list $(CPPSRCS)
# Ex: if CPPSRCS = ./src ./test, then $(addsuffix /*.cpp, $(CPPSRCS)) will result in ./src/*.cpp ./test/*.cpp
SRC_PATHS := $(wildcard $(addsuffix /*.cpp, $(CPPSRCS)))
SRCS := $(notdir $(SRC_PATHS))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJ_PATHS := $(addprefix $(CPPOBJS)/,$(OBJS))

# | $(CPPOBJS)  if the directory does not exist, it will be created before compiling the source files
$(CPPOBJS)/%.o: %.cpp | $(CPPOBJS)
	@printf "$(CYAN)[CC]$(NC) $<\n"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

build: $(OBJ_PATHS)
	@printf "$(GREEN)==>$(NC) Building the project...\n"

all: $(OBJ_PATHS)
	@printf "$(BLUE)[LD]$(NC) main\n"
	$(CXX) -o main $(OBJ_PATHS)
	@printf "$(GREEN)[OK]$(NC) Build complete.\n"

info:
	@printf "$(CYAN) this project is introducing mirror case by using makefile \n"
print-%:
	@echo i have:  $* = $($*)
clean:
	@printf "$(YELL)==>$(NC) Cleaning...\n"
	rm -rf ./obj/*.o main
	@printf "$(GREEN)[OK]$(NC) Done.\n"
