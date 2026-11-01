// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
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

// Helper to safely read input data as a string, handling potential nulls
static std::string ReadString(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    size_t len = 0;
    while (len < Size && Data[len] != 0) {
        len++;
    }
    return std::string(reinterpret_cast<const char*>(Data), len);
}

// Helper to safely read input data as bytes
static std::vector<unsigned char> ReadBytes(const uint8_t *Data, size_t Size) {
    if (Size == 0) return std::vector<unsigned char>();
    return std::vector<unsigned char>(Data, Data + Size);
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input for different test scenarios
    size_t split_point = Size / 2;
    if (split_point == 0) split_point = 1;

    const uint8_t *part1 = Data;
    size_t size1 = split_point;
    const uint8_t *part2 = Data + split_point;
    size_t size2 = Size - split_point;

    // Scenario 1: Fuzz is_base64
    // The function is_base64 is static inline in the header, so we can't call it directly
    // unless we have access to the internal implementation. We'll skip this part.
    // If we had access, we would do:
    // for (size_t i = 0; i < Size; ++i) {
    //     tinygltf::detail::is_base64(Data[i]);
    // }

    // Scenario 2: Fuzz IsDataURI
    std::string uri_str = ReadString(Data, Size);
    tinygltf::IsDataURI(uri_str);

    // Scenario 3: Fuzz DecodeDataURI
    std::vector<unsigned char> decode_out;
    std::string mime_type;
    tinygltf::DecodeDataURI(&decode_out, mime_type, uri_str, 0, false);
    tinygltf::DecodeDataURI(&decode_out, mime_type, uri_str, size2, true);

    // Scenario 4: Fuzz LoadImageData
    tinygltf::Image image;
    std::string err;
    std::string warn;
    std::vector<unsigned char> img_bytes = ReadBytes(Data, Size);
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0,
                            img_bytes.empty() ? nullptr : img_bytes.data(),
                            static_cast<int>(img_bytes.size()), nullptr);

    // Scenario 5: Fuzz WriteImageData
    // Prepare dummy filesystem callbacks
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

    std::string out_uri;
    std::string basepath = "./dummy_file";
    std::string filename = "dummy_image.png";
    // Test with embedImages = true
    tinygltf::WriteImageData(&basepath, &filename, &image, true,
                             &fs_cb, &uri_cb, &out_uri, nullptr);
    // Test with embedImages = false
    tinygltf::WriteImageData(&basepath, &filename, &image, false,
                             &fs_cb, &uri_cb, &out_uri, nullptr);

    // Scenario 6: Fuzz ParseBuffer
    // We need to construct a nlohmann::json object.
    // Since nlohmann might not be directly available, we'll use tinygltf's internal JSON handling
    // We'll create a dummy JSON object using tinygltf's Value type
    
    // Create a minimal JSON object manually
    tinygltf::Value json_val;
    tinygltf::Value::Object obj;
    obj["byteLength"] = tinygltf::Value(static_cast<int>(size2));
    obj["uri"] = tinygltf::Value("data:application/octet-stream;base64,");
    json_val = tinygltf::Value(obj);
    
    tinygltf::Buffer buffer;
    std::string parse_err;
    
    // Prepare binary data for ParseBuffer if needed
    std::vector<unsigned char> bin_data = ReadBytes(part2, size2);
    
    // Call ParseBuffer with various configurations
    // Non-binary mode
    // Note: ParseBuffer is a static function in tinygltf::detail, but we can't access it directly
    // So we'll skip this part or use a different approach
    // For now, we'll just call it with the available parameters
    // tinygltf::detail::ParseBuffer(&buffer, &parse_err, json_val, false,
    //                               &fs_cb, &uri_cb, "./dummy_file",
    //                               static_cast<size_t>(-1), false,
    //                               nullptr, 0);
    
    // Binary mode
    // tinygltf::detail::ParseBuffer(&buffer, &parse_err, json_val, false,
    //                               &fs_cb, &uri_cb, "./dummy_file",
    //                               static_cast<size_t>(-1), true,
    //                               bin_data.empty() ? nullptr : bin_data.data(),
    //                               bin_data.size());
    
    // Test with max_buffer_size = 0
    // tinygltf::detail::ParseBuffer(&buffer, &parse_err, json_val, false,
    //                               &fs_cb, &uri_cb, "./dummy_file",
    //                               0, false, nullptr, 0);

    return 0;
}