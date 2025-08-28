#include <opus/opusenc.h>
#include <opus/opus.h>
#include <fstream>
#include <iostream>
#include <vector>

void check_error(int ret, const char* msg) {
    if (ret != OPE_OK) {
        std::cerr << "ERROR: " << msg << " (code " << ret << ")\n";
        std::exit(1);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.opus>\n";
        return 1;
    }

    // 1. 创建解码器（通过文件读取）
    OggOpusEnc* enc;
    OggOpusComments* comments = ope_comments_create();
    int error;
    
    enc = ope_encoder_create_file(
        argv[1], // 输入文件
        comments,
        16000,   // 采样率（需与实际一致）
        1,       // 声道数（假设单声道）
        0,       // 映射通道
        &error
    );
    check_error(error, "ope_encoder_create_file");

    // 2. 获取音频信息（通过libopusenc原生方式）
    int channels = 1; // 默认值，libopusenc无直接获取接口
    std::cout << "Channels: " << channels << "\n";

    // 3. 创建PCM输出
    std::ofstream pcm_file(std::string(argv[1]) + ".pcm", std::ios::binary);
    
    // 4. 解码配置
    const int frame_size = 120;
    std::vector<float> pcm_frame(frame_size * channels);
    
    // 5. 数据处理循环
    while (ope_encoder_write_float(enc, pcm_frame.data(), frame_size) == OPE_OK) {
        pcm_file.write(
            reinterpret_cast<const char*>(pcm_frame.data()),
            frame_size * channels * sizeof(float)
        );
    }

    // 6. 释放资源
    ope_encoder_drain(enc);
    ope_encoder_destroy(enc);
    ope_comments_destroy(comments);
    
    std::cout << "Converted to: " << argv[1] << ".pcm\n";
    return 0;
}