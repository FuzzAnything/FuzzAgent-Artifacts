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

// Static helper to write dummy file for WriteImageData testing
static bool DummyWriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        if (err) *err = "Failed to open file";
        return false;
    }
    size_t written = fwrite(contents.data(), 1, contents.size(), f);
    fclose(f);
    if (written != contents.size()) {
        if (err) *err = "Failed to write full content";
        return false;
    }
    return true;
}

static bool DummyFileExists(const std::string &filename, void *) {
    return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *contents, std::string *err,
                               const std::string &filename, void *) {
    return false;
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *) {
    if (size) *size = 0;
    return true;
}

// Safe base64 decode implementation to avoid out-of-bounds access
static std::string SafeBase64Decode(const std::string &s) {
    static const char *const base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::string ret;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[static_cast<unsigned char>(base64_chars[i])] = i;
    
    size_t in_len = s.size();
    size_t i = 0, j = 0;
    size_t in_64 = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    while (in_64 < in_len) {
        unsigned char c = static_cast<unsigned char>(s[in_64]);
        if (c == '=') break;
        
        if (T[c] < 0) {
            in_64++;
            continue;
        }
        
        char_array_4[i++] = static_cast<unsigned char>(T[c]);
        in_64++;
        
        if (i == 4) {
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; (i < 3); i++)
                ret += static_cast<char>(char_array_3[i]);
            i = 0;
        }
    }
    
    if (i > 0) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (j = 0; (j < i - 1); j++) ret += static_cast<char>(char_array_3[j]);
    }
    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input into chunks for different test scenarios
    size_t chunk_size = Size / 6;
    const uint8_t *p = Data;

    // 1. Test is_base64 logic (local implementation since it's static inline in header)
    auto local_is_base64 = [](unsigned char c) -> bool {
        return (c >= 'A' && c <= 'Z') ||
               (c >= 'a' && c <= 'z') ||
               (c >= '0' && c <= '9') ||
               (c == '+') ||
               (c == '/');
    };

    for (size_t i = 0; i < (chunk_size > 0 ? chunk_size : Size); ++i) {
        (void)local_is_base64(p[i]);
    }

    // 2. Test base64_encode logic (local implementation)
    auto local_base64_encode = [](const unsigned char *bytes_to_encode, unsigned int in_len) -> std::string {
        static const char *const base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
        std::string ret;
        int in_len_64 = in_len;
        int i = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];
        while (in_len_64--) {
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
            for (int j = i; j < 3; j++)
                char_array_3[j] = '\0';
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            for (int j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];
            while ((i++ < 3))
                ret += '=';
        }
        return ret;
    };

    // Use the second chunk as binary data
    size_t enc_len = (Size > chunk_size) ? chunk_size : (Size - chunk_size);
    std::string encoded = local_base64_encode(p + chunk_size, static_cast<unsigned int>(enc_len));

    // 3. Test base64_decode logic (using safe implementation)
    std::string decoded_str = SafeBase64Decode(encoded);
    
    // Create a raw string from the input data for decoding test
    std::string raw_str(reinterpret_cast<const char*>(p + chunk_size), enc_len);
    std::string decoded_raw = SafeBase64Decode(raw_str);

    // 4. Test DecodeDataURI
    std::string mime_type;
    std::vector<unsigned char> out_data;
    std::string data_uri = "data:image/png;base64," + encoded;
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 0, false);
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 100, true);

    // 5. Test LoadImageData
    tinygltf::Image img;
    std::string err, warn;
    size_t img_data_start = 2 * chunk_size;
    size_t img_data_len = (Size > img_data_start) ? chunk_size : (Size - img_data_start);
    const unsigned char *img_bytes = (Size > img_data_start) ? (p + img_data_start) : nullptr;
    
    if (img_bytes && img_data_len > 0) {
        tinygltf::LoadImageData(&img, 0, &err, &warn, 0, 0, img_bytes, static_cast<int>(img_data_len), nullptr);
    }

    // 6. Test WriteImageData
    tinygltf::FsCallbacks fs_cb = {
        DummyFileExists,
        DummyExpandFilePath,
        DummyReadWholeFile,
        DummyWriteWholeFile,
        DummyGetFileSizeInBytes,
        nullptr
    };
    
    tinygltf::URICallbacks uri_cb = {
        nullptr, 
        nullptr, 
        nullptr  
    };

    std::string basepath = "./";
    std::string filename = "dummy_texture.png";
    std::string out_uri;
    
    if (img.image.empty()) {
        img.width = 1;
        img.image.push_back(0xFF);
        img.image.push_back(0xFF);
        img.image.push_back(0xFF);
        img.image.push_back(0xFF);
    }

    tinygltf::WriteImageData(&basepath, &filename, &img, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    tinygltf::WriteImageData(&basepath, &filename, &img, false, &fs_cb, &uri_cb, &out_uri, nullptr);

    return 0;
}