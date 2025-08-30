// #include <cstdlib>
// #include <cstdint>
// #include <string>
// #include <vector>
// #include <fstream>
// #include <iostream>
// #include <algorithm>
// #include "../inc/opus.h"

// void check_opus_error(int err, const char* context) {
//     if (err < 0) {
//         std::cerr << context << ": " << opus_strerror(err) << std::endl;
//         std::exit(1);
//     }
// }

// int main(int argc, char** argv) {
//     std::cout << "Program started" << std::endl;

//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <input.pcm>" << std::endl;
//         return 1;
//     }

//     // 1. 读取输入PCM（自动处理路径）
//     std::string full_path = argv[1];
//     std::ifstream pcm_file(full_path, std::ios::binary);
//     if (!pcm_file) {
//         std::cerr << "Error opening PCM file: " << full_path << std::endl;
//         return 1;
//     }

//     pcm_file.seekg(0, std::ios::end);
//     size_t file_size = pcm_file.tellg();
//     pcm_file.seekg(0, std::ios::beg);

//     std::vector<int16_t> pcm_data(file_size / sizeof(int16_t));
//     pcm_file.read(reinterpret_cast<char*>(pcm_data.data()), file_size);
//     pcm_file.close();

//     std::cout << "Loaded " << pcm_data.size() << " PCM samples" << std::endl;

//     // 2. 初始化编码器
//     const int sample_rate = 16000;
//     const int frame_size = sample_rate / 50; // 320 samples @16kHz
//     const int min_frame_size = 40; // Opus最小帧要求
    
//     int err;
//     OpusEncoder* encoder = opus_encoder_create(sample_rate, 1, OPUS_APPLICATION_AUDIO, &err);
//     check_opus_error(err, "Encoder init");
    
//     // 关键编码器设置
//     opus_encoder_ctl(encoder, OPUS_SET_BITRATE(64000));
//     opus_encoder_ctl(encoder, OPUS_SET_DTX(0)); // 必须禁用静音检测
//     opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_MUSIC));

//     // 3. 编码为Opus（带智能帧处理）
//     std::vector<unsigned char> opus_buffer(4000);
//     std::ofstream opus_file(full_path + ".opus", std::ios::binary);
    
//     for (size_t i = 0; i < pcm_data.size(); ) {
//         int remaining = pcm_data.size() - i;
//         int current_frame = std::min(frame_size, remaining);
        
//         // 智能处理末尾帧
//         if (remaining < frame_size) {
//             // 检测是否全静音
//             bool is_silence = std::all_of(&pcm_data[i], &pcm_data[i+remaining], 
//                 [](int16_t s){ return abs(s) < 5; });
            
//             if (is_silence) {
//                 std::cout << "Skipping trailing silence: " << remaining << " samples" << std::endl;
//                 break;
//             }
//             // 非静音但不足最小帧
//             else if (remaining < min_frame_size) { 
//                 current_frame = min_frame_size;
//                 pcm_data.resize(i + current_frame, 0); // 填充静音
//             }
//         }

//         int len = opus_encode(encoder, &pcm_data[i], current_frame,
//                             opus_buffer.data(), opus_buffer.size());
        
//         if (len < 0) {
//             std::cerr << "Critical error at position " << i 
//                      << " (frame size: " << current_frame << "): " 
//                      << opus_strerror(len) << std::endl;
//             return 1;
//         }
        
//         opus_file.write(reinterpret_cast<const char*>(opus_buffer.data()), len);
//         i += current_frame;
//     }
//     opus_file.close();

//     std::cout << "Encoding completed successfully" << std::endl;
    
//     // 清理资源
//     opus_encoder_destroy(encoder);
//     return 0;
// }


#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <opus/opus.h>
#include <ogg/ogg.h>
#include <opus/opusenc.h>

void check_error(int ret, const char* msg) {
    if (ret != OPE_OK) {
        std::cerr << msg << ": " << ope_strerror(ret) << std::endl;
        std::exit(1);
    }
}

int main(int argc, char** argv) {
    std::cout << "Program started" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.pcm>" << std::endl;
        return 1;
    }

    // 1. 初始化OggOpus编码器
    OggOpusComments* comments = ope_comments_create();
    ope_comments_add(comments, "ENCODER", "ARM_Opus_Encoder");
    
    std::string output_path = std::string(argv[1]) + ".opus";
    int error;
    OggOpusEnc* enc = ope_encoder_create_file(
        output_path.c_str(),    // 输出文件
        comments,              // 元数据
        16000,                 // 采样率
        1,                     // 声道数
        0,                     // 映射通道(单声道为0)
        &error                 // 错误码
    );
    check_error(error, "Encoder create failed");
    
    // 2. 设置编码参数（适配ARM平台）
    ope_encoder_ctl(enc, OPUS_SET_BITRATE(64000));
    ope_encoder_ctl(enc, OPUS_SET_COMPLEXITY(5));  // 适中复杂度
    ope_encoder_ctl(enc, OPUS_SET_SIGNAL(OPUS_SIGNAL_MUSIC));

    // 3. 读取PCM数据
    std::ifstream pcm_file(argv[1], std::ios::binary | std::ios::ate);
    size_t file_size = pcm_file.tellg();
    pcm_file.seekg(0);
    
    const int frame_size = 320; // 20ms@16kHz (适合ARM的低延迟)
    std::vector<int16_t> pcm_frame(frame_size);
    size_t total_samples = 0;

    // 4. 编码循环（带静音检测）
    while (pcm_file.read(reinterpret_cast<char*>(pcm_frame.data()), 
                       frame_size * sizeof(int16_t))) {
        // 静音检测（ARM优化版）
        bool is_silence = std::all_of(pcm_frame.begin(), pcm_frame.end(),
            [](int16_t s){ return abs(s) < 10; });
        
        if (!is_silence) {
            int ret = ope_encoder_write(enc, pcm_frame.data(), frame_size);
            check_error(ret, "Encoding failed");
        }
        total_samples += frame_size;
    }

    // 5. 处理剩余样本（ARM兼容处理）
    size_t remaining = pcm_file.gcount() / sizeof(int16_t);
    if (remaining > 0) {
        std::vector<int16_t> last_frame(frame_size, 0);
        std::copy_n(pcm_frame.begin(), remaining, last_frame.begin());
        int ret = ope_encoder_write(enc, last_frame.data(), frame_size);
        check_error(ret, "Final frame error");
    }

    // 6. 收尾工作
    ope_encoder_drain(enc);
    ope_encoder_destroy(enc);
    ope_comments_destroy(comments);

    std::cout << "Encoded " << total_samples << " samples to " 
              << output_path << std::endl;
    return 0;
}
