// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cctype>
#include "tiny_gltf.h"

// Suppress warnings for unused parameters in fuzzing context
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Local implementation of is_base64 to mimic tinygltf behavior
static inline bool local_is_base64(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '+' || c == '/';
}

// Local implementation of from_hex to mimic tinygltf behavior
static inline unsigned char local_from_hex(unsigned char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

static void FuzzBase64Char(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    unsigned char c = Data[0];
    bool result = local_is_base64(c);
    (void)result;
}

static void FuzzIsDataURI(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    std::string input(reinterpret_cast<const char*>(Data), Size);
    bool result = tinygltf::IsDataURI(input);
    (void)result;
}

static void FuzzBase64Decode(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    std::string decoded;
    static const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    int in_len = static_cast<int>(input.size());
    int i = 0;
    int j = 0;
    unsigned char char_array_4[4];
    unsigned char char_array_3[3];
    
    while (in_len-- && input[i] != '=' && std::isprint(static_cast<unsigned char>(input[i]))) {
        char_array_4[j++] = input[i++];
        if (j == 4) {
            for (int idx = 0; idx < 4; idx++) {
                size_t pos = base64_chars.find(char_array_4[idx]);
                if (pos != std::string::npos) {
                    char_array_4[idx] = static_cast<unsigned char>(pos);
                } else {
                    char_array_4[idx] = 0;
                }
            }
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (int idx = 0; idx < 3; idx++)
                decoded += static_cast<char>(char_array_3[idx]);
            j = 0;
        }
    }
    if (j) {
        for (int idx = j; idx < 4; idx++)
            char_array_4[idx] = 0;
        for (int idx = 0; idx < 4; idx++) {
            size_t pos = base64_chars.find(char_array_4[idx]);
            if (pos != std::string::npos) {
                char_array_4[idx] = static_cast<unsigned char>(pos);
            } else {
                char_array_4[idx] = 0;
            }
        }
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (int idx = 0; idx < (j - 1); idx++)
            decoded += static_cast<char>(char_array_3[idx]);
    }
    (void)decoded;
}

static void FuzzDecodeDataURI(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    std::string input(reinterpret_cast<const char*>(Data), Size);
    std::vector<unsigned char> out;
    std::string mime_type;
    size_t reqBytes = 0;
    bool checkSize = false;
    
    if (Size > 1) {
        reqBytes = static_cast<size_t>(Data[1]);
        checkSize = (Size > 2 && Data[2] > 0);
    }
    
    bool result = tinygltf::DecodeDataURI(&out, mime_type, input, reqBytes, checkSize);
    (void)result;
}

static void FuzzLoadImageData(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int size = static_cast<int>(Data[0]) | (static_cast<int>(Data[1]) << 8) | 
               (static_cast<int>(Data[2]) << 16) | (static_cast<int>(Data[3]) << 24);
    
    if (size < 0 || size > 1024 * 1024) return;
    
    if (Size <= 4 + static_cast<size_t>(size)) return;
    
    tinygltf::Image image;
    std::string err;
    std::string warn;
    int image_idx = 0;
    int req_width = 0;
    int req_height = 0;
    void *user_data = nullptr;
    
    const unsigned char *bytes = Data + 4;
    bool result = tinygltf::LoadImageData(&image, image_idx, &err, &warn, 
                                          req_width, req_height, bytes, size, user_data);
    (void)result;
}

static void FuzzFromHex(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    unsigned char ch = Data[0];
    unsigned char result = local_from_hex(ch);
    (void)result;
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 7;
    
    switch (selector) {
        case 0:
            FuzzBase64Char(Data, Size);
            break;
        case 1:
            FuzzIsDataURI(Data, Size);
            break;
        case 2:
            FuzzBase64Decode(Data, Size);
            break;
        case 3:
            FuzzDecodeDataURI(Data, Size);
            break;
        case 4:
            FuzzLoadImageData(Data, Size);
            break;
        case 5:
            FuzzFromHex(Data, Size);
            break;
        case 6:
            FuzzBase64Char(Data, Size);
            FuzzFromHex(Data, Size);
            break;
    }
    
    return 0;
}

#pragma GCC diagnostic pop