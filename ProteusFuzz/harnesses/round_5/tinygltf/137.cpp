// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"

// Helper to generate a base64-like string from raw input for testing base64_decode
static std::string GenerateBase64FromInput(const uint8_t *data, size_t size) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve(((size + 2) / 3) * 4);
    
    int i = 0;
    while (i < size) {
        uint32_t octet_a = i < size ? data[i++] : 0;
        uint32_t octet_b = i < size ? data[i++] : 0;
        uint32_t octet_c = i < size ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        result += charset[(triple >> 18) & 0x3F];
        result += charset[(triple >> 12) & 0x3F];
        result += charset[(triple >> 6) & 0x3F];
        result += charset[triple & 0x3F];
    }
    
    // Add padding if necessary
    int mod = size % 3;
    if (mod == 1) {
        result[result.size() - 1] = '=';
        result[result.size() - 2] = '=';
    } else if (mod == 2) {
        result[result.size() - 1] = '=';
    }
    
    return result;
}

// Local implementation of is_base64 to match the internal logic if not exposed
static inline bool is_base64_local(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           (c == '+') ||
           (c == '/');
}

// Local implementation of from_hex to match the internal logic if not exposed
static inline unsigned char from_hex_local(unsigned char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

// Local implementation of base64_decode to ensure availability
static std::string base64_decode_local(const std::string &encoded_string) {
    const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::string ret;
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (in_len-- && (encoded_string[in_] != '=') && is_base64_local(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test tinygltf::is_base64 (using local fallback if not in namespace)
    for (size_t i = 0; i < Size; ++i) {
        volatile bool res1 = is_base64_local(Data[i]);
        (void)res1; 
    }

    // 2. Test tinygltf::IsDataURI
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    volatile bool res2 = tinygltf::IsDataURI(input_str);
    (void)res2;

    std::string data_uri_prefix = "data:image/png;base64,";
    std::string potential_data_uri = data_uri_prefix + input_str;
    volatile bool res3 = tinygltf::IsDataURI(potential_data_uri);
    (void)res3;

    // 3. Test tinygltf::base64_decode (using local fallback)
    std::string decoded_raw = base64_decode_local(input_str);
    (void)decoded_raw;

    std::string valid_b64 = GenerateBase64FromInput(Data, Size);
    std::string decoded_valid = base64_decode_local(valid_b64);
    (void)decoded_valid;

    // 4. Test tinygltf::DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    size_t req_bytes = 0;
    bool check_size = false;

    bool res4 = tinygltf::DecodeDataURI(&out_data, mime_type, potential_data_uri, req_bytes, check_size);
    (void)res4;

    bool res5 = tinygltf::DecodeDataURI(&out_data, mime_type, input_str, req_bytes, check_size);
    (void)res5;

    bool res6 = tinygltf::DecodeDataURI(&out_data, mime_type, potential_data_uri, 1000, true);
    (void)res6;

    // 5. Test tinygltf::dlib::from_hex (using local fallback)
    for (size_t i = 0; i < Size; ++i) {
        volatile unsigned char hex_val = from_hex_local(Data[i]);
        (void)hex_val;
    }

    // 6. Test tinygltf::LoadImageData
    tinygltf::Image image;
    std::string err;
    std::string warn;
    int req_width = -1;
    int req_height = -1;
    void *user_data = nullptr;

    bool res7 = tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, Data, static_cast<int>(Size), user_data);
    (void)res7;

    if (!decoded_valid.empty()) {
        bool res8 = tinygltf::LoadImageData(&image, 1, &err, &warn, req_width, req_height,
                                            reinterpret_cast<const unsigned char*>(decoded_valid.data()),
                                            static_cast<int>(decoded_valid.size()), user_data);
        (void)res8;
    }

    if (!out_data.empty()) {
        bool res9 = tinygltf::LoadImageData(&image, 2, &err, &warn, req_width, req_height,
                                            out_data.data(), static_cast<int>(out_data.size()), user_data);
        (void)res9;
    }

    return 0;
}