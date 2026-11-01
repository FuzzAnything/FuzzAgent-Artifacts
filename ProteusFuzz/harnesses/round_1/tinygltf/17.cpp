// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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

// Local implementations for internal tinygltf functions to ensure compilation
// and allow fuzzing of the logic described in the prompt.
namespace tinygltf_local {

static inline bool is_base64(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') || c == '+' || c == '/';
}

std::string base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len) {
    static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while (i++ < 3)
            ret += '=';
    }

    return ret;
}

std::string base64_decode(std::string const &s) {
    static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    int l = 0;
    int in_len = s.size();
    unsigned char char_array_4[4];
    unsigned char char_array_3[3];

    for (i = 0; i < in_len; i++) {
        if (s[i] == '=') break;
        char_array_4[j++] = s[i];
        if (j == 4) {
            for (j = 0; j < 4; j++) {
                const char* ptr = strchr(base64_chars, char_array_4[j]);
                if (ptr == nullptr) return ""; // Invalid char
                char_array_4[j] = static_cast<unsigned char>(ptr - base64_chars);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; j < 3; j++)
                ret += static_cast<char>(char_array_3[j]);
            j = 0;
        }
    }

    if (j) {
        for (l = j; l < 4; l++)
            char_array_4[l] = 0;

        for (l = 0; l < 4; l++) {
            const char* ptr = strchr(base64_chars, char_array_4[l]);
            if (ptr == nullptr) return ""; // Invalid char
            char_array_4[l] = static_cast<unsigned char>(ptr - base64_chars);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (l = 0; l < j - 1; l++)
            ret += static_cast<char>(char_array_3[l]);
    }

    return ret;
}

} // namespace tinygltf_local

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input into segments for different test cases
    size_t pos = 0;
    auto get_segment = [&](size_t len) -> std::vector<unsigned char> {
        std::vector<unsigned char> seg;
        if (pos + len > Size) len = Size - pos;
        seg.assign(Data + pos, Data + pos + len);
        pos += len;
        return seg;
    };

    // 1. Test base64_encode (using local implementation)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        if (!seg.empty()) {
            std::string encoded = tinygltf_local::base64_encode(seg.data(), seg.size());
            (void)encoded; 
        }
    }

    // 2. Test base64_decode (using local implementation)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        if (!seg.empty()) {
            std::string input_str(reinterpret_cast<char*>(seg.data()), seg.size());
            std::string decoded = tinygltf_local::base64_decode(input_str);
            (void)decoded;
        }
    }

    // 3. Test is_base64 (using local implementation)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        for (unsigned char c : seg) {
            bool result = tinygltf_local::is_base64(c);
            (void)result;
        }
    }

    // 4. Test DecodeDataURI (Public API)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        if (!seg.empty()) {
            std::string uri_str(reinterpret_cast<char*>(seg.data()), seg.size());
            std::vector<unsigned char> out;
            std::string mime_type;
            size_t reqBytes = 0;
            bool checkSize = false;
            bool success = tinygltf::DecodeDataURI(&out, mime_type, uri_str, reqBytes, checkSize);
            (void)success;
            (void)out;
            (void)mime_type;
        }
    }

    // 5. Test LoadImageData (Public API)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        if (!seg.empty()) {
            tinygltf::Image image;
            std::string err;
            std::string warn;
            int req_width = 0;
            int req_height = 0;
            bool success = tinygltf::LoadImageData(
                &image, 0, &err, &warn, req_width, req_height,
                seg.data(), static_cast<int>(seg.size()), nullptr
            );
            (void)success;
            (void)image;
            (void)err;
            (void)warn;
        }
    }

    // 6. Test WriteImageData (Public API)
    {
        std::vector<unsigned char> seg = get_segment(Size / 6);
        if (!seg.empty()) {
            tinygltf::Image image;
            image.image = seg;
            image.width = static_cast<int>(seg.size() > 0 ? 1 : 0);
            image.name = "fuzz_test";

            std::string basepath = "./dummy_file";
            std::string filename = "fuzz_img.png";
            std::string out_uri;
            
            // Prepare callbacks (minimal stubs)
            tinygltf::FsCallbacks fs_cb{};
            tinygltf::URICallbacks uri_cb{};
            
            // Try both embed modes
            for (bool embed : {true, false}) {
                bool success = tinygltf::WriteImageData(
                    &basepath, &filename, &image, embed,
                    &fs_cb, &uri_cb, &out_uri, nullptr
                );
                (void)success;
                (void)out_uri;
            }
        }
    }

    return 0;
}