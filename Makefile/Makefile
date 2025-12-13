CC := gcc

SRC_DIR := ./src .
INC_DIR := ./inc
OBJ_DIR := ./obj
LIB_STA := ./lib/Static_lib
LIB_SHA := ./lib/Shared_lib
BIN_DIR := ./bin

CFLAGS := -I$(INC_DIR) -Wall -Werror

.PHONY: clean all build static link_static shared link_shared

SRC_FILES := $(foreach src,$(SRC_DIR),$(wildcard $(src)/*.c))
INC_FILES := $(foreach inc,$(INC_DIR),$(wildcard $(inc)/*.h))


OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))
LIB_OBJ_SHARED := $(patsubst %.c,$(OBJ_DIR)/%.os,$(notdir $(filter-out %main.c, $(SRC_FILES))))


LIB_OBJS := $(filter-out $(OBJ_DIR)/main.o,$(OBJ_FILES))

vpath %.c $(SRC_DIR)

$(OBJ_DIR)/%.o: %.c $(INC_FILES)
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.os: %.c $(INC_FILES)
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) -fPIC

build: $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/main.exe $^

# --- STATIC LIB ---
static: $(LIB_OBJS)
	mkdir -p $(LIB_STA)
	# Tạo thư viện tên là libstrutils.a
	ar rcs $(LIB_STA)/libstrutils.a $^

link_static: $(OBJ_DIR)/main.o static
	mkdir -p $(BIN_DIR)
	# SỬA: Dùng -lstrutils (thay vì -lstatic) để khớp với tên libstrutils.a
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_STA) -lstrutils -o $(BIN_DIR)/main_static.exe

# --- SHARED LIB ---
shared: $(LIB_OBJ_SHARED)
	mkdir -p $(LIB_SHA)
	# Tạo thư viện tên là libstrutils.so
	$(CC) -shared $^ -o $(LIB_SHA)/libstrutils.so

link_shared: $(OBJ_DIR)/main.o shared
	mkdir -p $(BIN_DIR)
	# SỬA: Dùng -lstrutils (thay vì -lshared) để khớp với tên libstrutils.so
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_SHA) -lstrutils -o $(BIN_DIR)/main_shared.exe
	@echo "-----------------------------------------------------"
	@echo "Để chạy, hãy gõ lệnh:"
	@echo "export LD_LIBRARY_PATH="/mnt/d/course_DEVLINUX/CM_Linux/Makefile/lib/Shared_lib/""
	@echo "-----------------------------------------------------"

all:   link_shared link_static

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_STA) $(LIB_SHA) 

print-%:
	@echo $($(subst print-,,$@))