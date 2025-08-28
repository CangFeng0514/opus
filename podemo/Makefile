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

ARM = $(HOME)/local/arm7

# 安装路径变量
OGG_DIR = $(ARM)/Ogg/ogg
OPUS_DIR = $(ARM)/Opus/opus
OPUSENC_DIR = $(ARM)/Opusenc/opusenc


# 编译参数
CFLAGS = -I$(OGG_DIR)/include \
         -I$(OPUS_DIR)/include \
         -I$(OPUSENC_DIR)/include \
         -std=c++11 -g -O0

# 链接参数（注意库顺序）  # -Wl,-Bstatic 强制静态链接接下来的库    -Wl,-Bdynamic 恢复动态链接       -lm  # 动态链接数学库  -I$(OPUS_DIR)/include/opus 用于库文件内部包含
LDFLAGS = -L$(OGG_DIR)/lib \
          -L$(OPUS_DIR)/lib \
          -L$(OPUSENC_DIR)/lib \
          -I$(OPUS_DIR)/include/opus \
          -Wl,-Bstatic \
          -lopusenc -lopus -logg \
          -Wl,-Bdynamic \
          -lm

TARGET = demo
SRC = src/opus_demo.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

copy:
	@mkdir -p Test
	@cp -rf res/*.pcm Test
	@cp -rf demo Test

run:
	./$(TARGET) $(RES_DIR)/ljj.pcm

clean:
	rm -f $(TARGET) $(RES_DIR)/*.opus
	rm -rf Test/*

.PHONY: all clean run copy

