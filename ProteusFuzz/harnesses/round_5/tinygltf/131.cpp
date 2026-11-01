// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>

#include "tiny_gltf.h"

// Helper to write dummy file
static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Callback implementations matching tinygltf expectations
// Note: tinygltf FsCallbacks and URICallbacks use std::function with specific signatures.
// We must match the signatures exactly as defined in tiny_gltf.h.

// FileExistsFunction: bool (const std::string &, void *)
static bool DummyFileExists(const std::string &filename, void *) {
    (void)filename;
    return false;
}

// ExpandFilePathFunction: std::string (const std::string &, void *)
static std::string DummyExpandFilePath(const std::string &filepath, void *) {
    (void)filepath;
    return filepath;
}

// ReadWholeFileFunction: bool (std::vector<unsigned char> *, std::string *, const std::string &, void *)
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *) {
    (void)filename;
    if (err) *err = "File not found";
    if (out) out->clear();
    return false;
}

// WriteWholeFileFunction: bool (std::string *, const std::string &, const std::vector<unsigned char> &, void *)
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
    if (err) *err = "";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    file.close();
    return true;
}

// GetFileSizeInBytesFunction: bool (size_t *, std::string *, const std::string &, void *)
// Note: The error message indicates the expected signature is bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)
// which corresponds to bool (size_t *, std::string *, const std::string &, void *)
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *) {
    (void)filename;
    if (size) *size = 0;
    if (err) *err = "";
    return true;
}

// URIEncodeFunction: bool (const std::string &, const std::string &, std::string *, void *)
// Signature: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)
// Actually, looking at tinygltf source, URIEncodeFunction is: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)
// Wait, let's re-verify typical tinygltf signatures.
// Usually: bool URIEncodeFunction(const std::string &in, const std::string &out, std::string *out_uri, void *);
// But often it's: bool (const std::string &in, std::string *out, void *);
// Let's assume the standard tinygltf v2.8+ signature based on the error message context:
// The error says: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// So: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)?
// Actually, the standard tinygltf URIEncodeFunction is: bool (const std::string &in, const std::string &out, std::string *out_uri, void *) is unlikely.
// Let's look at the error message again: "function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>"
// This implies: bool (const std::string &, const std::string &, std::string *, void *)
// This is strange. Let's assume the library version expects:
// bool URIEncodeFunction(const std::string &in, const std::string &out, std::string *out_uri, void *);
// Wait, the second argument is usually the output buffer? No.
// Let's try to match the error message exactly.
// Error: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// This means: bool (const std::string &arg1, const std::string &arg2, std::string *arg3, void *arg4)
// This looks like: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)
// But that doesn't make sense for encoding.
// Let's re-read the tinygltf header if possible. Since I can't, I will rely on the error message.
// The error message for URIEncodeFunction says: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// So I will define: static bool DummyURIEncode(const std::string &in, const std::string &out, std::string *out_uri, void *);
// But wait, the standard tinygltf URIEncodeFunction is: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)?
// Actually, in many versions it is: bool (const std::string &in, std::string *out, void *);
// Let's look at the URIDecodeFunction error: function<bool (const basic_string<char> &, basic_string<char> *, void *)>
// This is: bool (const std::string &in, std::string *out, void *)
// So URIEncodeFunction might be: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)?
// No, the error says: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>
// Let's assume the signature is: bool (const std::string &in, const std::string &out, std::string *out_uri, void *)
// But that's weird. Let's try to find a common pattern.
// Maybe the error message is showing the function type as: bool (const std::string &, const std::string &, std::string *, void *)
// Let's just define it to match the error message exactly.

static bool DummyURIEncode(const std::string &in, const std::string &out, std::string *out_uri, void *) {
    (void)in;
    (void)out;
    if (out_uri) *out_uri = in; // Simple passthrough
    return true;
}

// URIDecodeFunction: bool (const std::string &, std::string *, void *)
static bool DummyURIDecode(const std::string &in, std::string *out, void *) {
    (void)out;
    (void)in;
    if (out) *out = in;
    return true;
}

// Local implementations of the internal functions to be fuzzed
static inline bool is_base64_local(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           (c == '+') ||
           (c == '/');
}

static std::string base64_encode_local(const unsigned char *data, unsigned int len) {
    static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    unsigned int i = 0;
    unsigned int j = 0;

    while (len--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                result += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; j++)
            result += base64_chars[char_array_4[j]];

        while (i++ < 3)
            result += '=';
    }
    return result;
}

static std::string base64_decode_local(const std::string &encoded_string) {
    static const int base64_table[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    std::string result;
    unsigned char char_array_4[4];
    unsigned char char_array_3[3];
    size_t i = 0;
    size_t j = 0;
    size_t in_len = encoded_string.length();

    if (in_len % 4 != 0) {
        return "";
    }

    while (in_len--) {
        char_array_4[i++] = encoded_string[j];
        if (char_array_4[i - 1] != '=') {
            int val = base64_table[static_cast<unsigned char>(char_array_4[i - 1])];
            if (val == -1) {
                return "";
            }
            char_array_4[i - 1] = static_cast<unsigned char>(val);
        }
        if (i == 4) {
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++)
                result += static_cast<char>(char_array_3[i]);
            i = 0;
        }
        j++;
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

        for (j = 0; j < i - 1; j++)
            result += static_cast<char>(char_array_3[j]);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz is_base64 (using local implementation as per prompt description)
    for (size_t i = 0; i < Size; ++i) {
        (void)is_base64_local(Data[i]);
    }

    // 2. Fuzz base64_encode (using local implementation)
    for (size_t len = 0; len <= Size; ++len) {
        std::string encoded = base64_encode_local(Data, static_cast<unsigned int>(len));
        (void)encoded;
    }

    // 3. Fuzz base64_decode (using local implementation)
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    std::string decoded = base64_decode_local(input_str);
    (void)decoded;

    // 4. Fuzz tinygltf::DecodeDataURI
    std::string data_uri_prefix = "data:image/png;base64,";
    std::string uri_input = data_uri_prefix + base64_encode_local(Data, static_cast<unsigned int>(Size));
    std::vector<unsigned char> out_data;
    std::string mime_type;
    bool check_size = (Size % 2 == 0);
    size_t req_bytes = (Size > 0) ? Size / 2 : 0;
    (void)tinygltf::DecodeDataURI(&out_data, mime_type, uri_input, req_bytes, check_size);

    // 5. Fuzz tinygltf::LoadImageData
    tinygltf::Image image;
    std::string err_str;
    std::string warn_str;
    int req_width = (Size > 0) ? static_cast<int>(Data[0]) : 0;
    int req_height = (Size > 1) ? static_cast<int>(Data[1]) : 0;
    
    bool load_result = tinygltf::LoadImageData(&image, 0, &err_str, &warn_str, req_width, req_height, Data, static_cast<int>(Size), nullptr);
    (void)load_result;

    // 6. Fuzz tinygltf::WriteImageData
    tinygltf::FsCallbacks fs_cb = {
        DummyFileExists,
        DummyExpandFilePath,
        DummyReadWholeFile,
        DummyWriteWholeFile,
        DummyGetFileSizeInBytes,
        nullptr
    };
    tinygltf::URICallbacks uri_cb = {
        DummyURIEncode,
        DummyURIDecode,
        nullptr
    };

    tinygltf::Image write_image;
    write_image.name = "test_image";
    write_image.width = (Size > 0) ? static_cast<int>(Data[0]) : 1;
    write_image.height = (Size > 1) ? static_cast<int>(Data[1]) : 1;
    
    size_t channel_count = 3;
    size_t pixel_data_size = static_cast<size_t>(write_image.width) * static_cast<size_t>(write_image.height) * channel_count;
    write_image.image.resize(pixel_data_size);
    for (size_t i = 0; i < pixel_data_size && i < Size; ++i) {
        write_image.image[i] = Data[i % Size];
    }
    for (size_t i = (pixel_data_size > Size ? Size : pixel_data_size); i < pixel_data_size; ++i) {
        write_image.image[i] = 0;
    }

    std::string basepath = ".";
    std::string filename = "./dummy_file";
    std::string out_uri;
    
    bool embed_true = (Size % 2 == 0);
    bool write_result = tinygltf::WriteImageData(&basepath, &filename, &write_image, embed_true, &fs_cb, &uri_cb, &out_uri, nullptr);
    (void)write_result;

    if (!embed_true) {
        std::remove(filename.c_str());
    }

    return 0;
}