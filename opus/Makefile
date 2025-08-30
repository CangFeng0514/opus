# # 工具链配置
# CROSS_COMPILE = arm-linux-gnueabihf-
# CC = $(CROSS_COMPILE)g++
# CFLAGS = -I./inc -L./opus-armv7/lib -static -std=c++11 -g -O0
# LDFLAGS = -lopus

# # 文件路径
# SRC_DIR = src
# RES_DIR = res
# TARGET = opus_demo
# SRC = $(SRC_DIR)/opus_demo.cpp

# all: $(TARGET)

# $(TARGET): $(SRC)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# run:
# 	./$(TARGET) $(RES_DIR)/output.pcm

# clean:
# 	rm -f $(TARGET) $(RES_DIR)/*.opus $(RES_DIR)/decoded_*.pcm

# .PHONY: all clean run




# 工具链配置
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)g++

# 安装路径变量
ARM_LIBS_DIR = $(HOME)/arm-libs
OPUS_DIR = $(HOME)/opus-armv7

# 编译参数
CFLAGS = -I./inc \
         -I$(OPUS_DIR)/include \
         -I$(ARM_LIBS_DIR)/include \
         -std=c++11 -g -O0

# 链接参数（注意库顺序）
LDFLAGS = -L$(OPUS_DIR)/lib \
          -L$(ARM_LIBS_DIR)/lib \
          -Wl,-Bstatic \
          -lopusenc -lopus -logg \
          -Wl,-Bdynamic \
          -lm

TARGET = demo
SRC = src/opus_demo.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run:
	./$(TARGET) $(RES_DIR)/output.pcm

clean:
	rm -f $(TARGET) $(RES_DIR)/*.opus

.PHONY: all clean run