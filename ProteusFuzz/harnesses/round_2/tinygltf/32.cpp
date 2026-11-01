// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include <fstream>

// Helper to write dummy file
static void WriteDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Local implementations of base64 functions to ensure compilation if not exposed in namespace
// These match the logic typically found in tinygltf.h
static bool is_base64_local(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') || (c == '+') || (c == '/');
}

static std::string base64_encode_local(const unsigned char* bytes_to_encode, unsigned int in_len) {
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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

            for (i = 0; (i < 4); i++)
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

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

static std::string base64_decode_local(const std::string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    while (in_len-- && (encoded_string[in_] != '=') && is_base64_local(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
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

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input into segments for different test cases
    size_t segment_size = Size / 6;
    size_t offset = 0;

    // 1. Test is_base64 (using local implementation to ensure compilation)
    for (size_t i = 0; i < std::min(Size, static_cast<size_t>(1000)); ++i) {
        (void)is_base64_local(Data[i]);
    }

    // 2. Test base64_encode (using local implementation)
    if (Size > 0) {
        std::string encoded = base64_encode_local(Data, std::min(Size, segment_size));
        (void)encoded;
    }

    // 3. Test base64_decode (using local implementation)
    offset = segment_size;
    size_t seg2_len = std::min(Size - offset, segment_size);
    if (seg2_len > 0) {
        std::string input_str(reinterpret_cast<const char*>(Data + offset), seg2_len);
        std::string decoded = base64_decode_local(input_str);
        (void)decoded;
    }

    // 4. Test DecodeDataURI
    offset += seg2_len;
    size_t seg3_len = std::min(Size - offset, segment_size);
    if (seg3_len > 0) {
        std::string uri_input(reinterpret_cast<const char*>(Data + offset), seg3_len);
        std::vector<unsigned char> out;
        std::string mime_type;
        tinygltf::DecodeDataURI(&out, mime_type, uri_input, 0, false);
        tinygltf::DecodeDataURI(&out, mime_type, uri_input, seg3_len, true);
    }

    // 5. Test LoadImageData
    offset += seg3_len;
    size_t seg4_len = std::min(Size - offset, segment_size);
    if (seg4_len > 0) {
        tinygltf::Image image;
        std::string err, warn;
        tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data + offset, static_cast<int>(seg4_len), nullptr);
    }

    // 6. Test WriteImageData
    offset += seg4_len;
    size_t seg5_len = std::min(Size - offset, segment_size);
    if (seg5_len > 0) {
        tinygltf::Image image;
        image.image.assign(Data + offset, Data + offset + seg5_len);
        image.width = static_cast<int>(seg5_len > 0 ? 1 : 0);
        image.height = 1;
        image.component = 1;
        image.bits = 8;
        image.name = "test_image";

        std::string basepath = "./";
        std::string filename = "dummy_output.bin";
        std::string out_uri;

        tinygltf::FsCallbacks fs_cb;
        fs_cb.FileExists = nullptr;
        fs_cb.ExpandFilePath = nullptr;
        fs_cb.ReadWholeFile = nullptr;
        fs_cb.WriteWholeFile = nullptr;
        fs_cb.GetFileSizeInBytes = nullptr;
        fs_cb.user_data = nullptr;

        tinygltf::URICallbacks uri_cb;
        uri_cb.encode = nullptr;
        uri_cb.decode = nullptr;
        uri_cb.user_data = nullptr;

        tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
        tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
    }

    // 7. Additional exploration: Write raw data to a dummy file
    WriteDummyFile("./dummy_file", Data, Size);

    return 0;
}