// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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

// Helper to convert fuzzer input to a safe string (handling nulls)
static std::string safe_string(const uint8_t *Data, size_t Size, size_t MaxSize) {
    if (Size == 0) return "";
    // Ensure we don't read past the actual data buffer
    size_t actual_size = (Size > MaxSize) ? MaxSize : Size;
    std::string result(actual_size, '\0');
    for (size_t i = 0; i < actual_size; ++i) {
        result[i] = Data[i] == 0 ? ' ' : static_cast<char>(Data[i]);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input into chunks for different test scenarios
    // Ensure chunk_size is at least 1 to avoid division by zero logic issues if Size is small
    size_t chunk_size = Size / 7;
    if (chunk_size < 1) chunk_size = 1;

    // Calculate remaining size for each chunk to prevent overflow
    size_t remaining = Size;
    
    // Chunk 1
    size_t s1_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str1 = safe_string(Data, s1_len, remaining);
    remaining -= s1_len;

    // Chunk 2
    size_t s2_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str2 = safe_string(Data + s1_len, s2_len, remaining);
    remaining -= s2_len;

    // Chunk 3
    size_t s3_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str3 = safe_string(Data + s1_len + s2_len, s3_len, remaining);
    remaining -= s3_len;

    // Chunk 4
    size_t s4_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str4 = safe_string(Data + s1_len + s2_len + s3_len, s4_len, remaining);
    remaining -= s4_len;

    // Chunk 5
    size_t s5_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str5 = safe_string(Data + s1_len + s2_len + s3_len + s4_len, s5_len, remaining);
    remaining -= s5_len;

    // Chunk 6
    size_t s6_len = (chunk_size < remaining) ? chunk_size : remaining;
    std::string str6 = safe_string(Data + s1_len + s2_len + s3_len + s4_len + s5_len, s6_len, remaining);
    remaining -= s6_len;

    // Chunk 7 (rest)
    std::string str7 = safe_string(Data + s1_len + s2_len + s3_len + s4_len + s5_len + s6_len, remaining, remaining);

    // 1. Test urldecode (Skipped as it is static/internal in many builds, relying on URIDecode)

    // 2. Test base64_decode (Skipped as it is static/internal in many builds)

    // 3. Test tinygltf::WriteImageData
    tinygltf::Image img;
    img.name = str3;
    // Ensure width is non-negative
    img.width = (str3.length() > 0) ? static_cast<int>(str3.length()) : 1;
    img.image = std::vector<unsigned char>(str4.begin(), str4.end());
    
    std::string basepath = "./";
    std::string filename = "./dummy_image.png";
    std::string out_uri;
    
    bool writeResult = tinygltf::WriteImageData(&basepath, &filename, &img, true, nullptr, nullptr, &out_uri, nullptr);
    (void)writeResult;

    // 4. Test tinygltf::URIDecode
    std::string out_uri2;
    bool uriResult = tinygltf::URIDecode(str5, &out_uri2, nullptr);
    (void)uriResult;

    // 5. Test tinygltf::TinyGLTF::WriteGltfSceneToFile
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string gltf_filename = "./dummy_gltf.gltf";
    bool writeGltfResult = ctx.WriteGltfSceneToFile(&model, gltf_filename, true, true, true, true);
    (void)writeGltfResult;

    // 6. Test tinygltf::DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    std::string data_uri = "data:image/png;base64," + str6;
    bool decodeResult = tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 0, false);
    (void)decodeResult;

    // Additional exploration
    std::string glb_filename = "./dummy_glb.glb";
    bool writeGlbResult = ctx.WriteGltfSceneToFile(&model, glb_filename, true, true, false, true);
    (void)writeGlbResult;

    bool decodeResult2 = tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 1000, true);
    (void)decodeResult2;

    std::string out_uri3;
    bool uriResult2 = tinygltf::URIDecode("", &out_uri3, nullptr);
    (void)uriResult2;

    // Test URIDecode and DecodeDataURI with empty strings
    std::string out_uri4;
    bool uriResult3 = tinygltf::URIDecode("", &out_uri4, nullptr);
    (void)uriResult3;

    std::vector<unsigned char> out_data2;
    std::string mime_type2;
    bool decodeResult3 = tinygltf::DecodeDataURI(&out_data2, mime_type2, "", 0, false);
    (void)decodeResult3;

    return 0;
}