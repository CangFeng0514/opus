# 使用您原有的配置
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)g++

ARM = $(HOME)/local/arm7
OGG_DIR = $(ARM)/Ogg/ogg
OPUS_DIR = $(ARM)/Opus/opus
OPUSENC_DIR = $(ARM)/Opusenc/opusenc

# 编译目标
TARGET = demo
OPUS_FILE = res/ljj.opus

# 使用libopusenc实现解码（不依赖opusfile）
CFLAGS = -I$(OGG_DIR)/include \
         -I$(OPUS_DIR)/include \
         -I$(OPUSENC_DIR)/include \
         -std=c++11 -g -O0

LDFLAGS = -L$(OGG_DIR)/lib \
          -L$(OPUS_DIR)/lib \
          -L$(OPUSENC_DIR)/lib \
		  -I$(OPUS_DIR)/include/opus \
          -Wl,-Bstatic -lopusenc -lopus -logg -Wl,-Bdynamic -lm

all: $(TARGET)

$(TARGET): src/demo.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TARGET)
	./$(TARGET) $(OPUS_FILE)
	@echo "生成文件: $(OPUS_FILE).pcm"

clean:
	rm -f $(TARGET) *.pcm

.PHONY: all test clean