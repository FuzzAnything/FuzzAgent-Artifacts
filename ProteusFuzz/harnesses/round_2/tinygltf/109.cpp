// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Static helper to write data to a dummy file if needed
static void WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Minimal FsCallbacks implementation for testing
static bool FsFileExists(const std::string &filename, void *) {
    std::ifstream f(filename);
    return f.good();
}

static std::string FsExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

static bool FsReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                            const std::string &filename, void *) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) {
        if (err) *err = "File not found";
        return false;
    }
    out->assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return true;
}

static bool FsWriteWholeFile(std::string *err, const std::string &filename,
                             const std::vector<unsigned char> &contents, void *) {
    std::ofstream f(filename, std::ios::binary);
    if (!f) {
        if (err) *err = "Cannot write file";
        return false;
    }
    f.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

// Correct signature for GetFileSizeInBytes: returns bool, outputs size to pointer
static bool FsGetFileSizeInBytes(std::size_t *size, std::string *err,
                                 const std::string &filename, void *) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) {
        if (err) *err = "File not found";
        return false;
    }
    f.seekg(0, std::ios::end);
    *size = static_cast<std::size_t>(f.tellg());
    return true;
}

// Minimal URICallbacks implementation
// Note: In newer tinygltf versions, URICallbacks might use std::function with specific signatures.
// We define dummy functions that match the expected types if possible, or use nullptrs.
// Based on the error, the expected types are:
// encode: bool(const std::string&, const std::string&, std::string*, void*)
// decode: bool(const std::string&, std::string*, void*)
// However, the prompt description says they return std::string.
// Let's check the actual tinygltf.h definition for URICallbacks.
// In standard tinygltf, URICallbacks are:
// typedef std::string (*URIEncodeFunction)(const std::string &, void *);
// typedef std::string (*URIDecodeFunction)(const std::string &, void *);
// But the error message suggests the compiler expects bool return types.
// This implies the version of tinygltf in the environment has different signatures.
// Let's look at the error: 
// "no known conversion ... to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This suggests the type is: bool (const std::string&, const std::string&, std::string*, void*)
// And for decode: bool (const std::string&, std::string*, void*)
// This looks like the signature for the new URI handling in tinygltf v2.9+ where callbacks return bool and output to a pointer.
// Let's define functions matching these signatures.

static bool UriEncodeFunc(const std::string &str, const std::string &/*unused*/, std::string *out, void *) {
    if (out) *out = str;
    return true;
}

static bool UriDecodeFunc(const std::string &str, std::string *out, void *) {
    if (out) *out = str;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test tinygltf::is_base64
    // Since is_base64 is static inline and not exposed in the namespace in some versions,
    // we cannot call it directly. We rely on base64_decode to exercise it internally.

    // 2. Test tinygltf::base64_encode and base64_decode
    // If these are not in the namespace, we skip them. 
    // However, the prompt says they are target APIs. 
    // Given the compilation error, they are not visible. 
    // We will try to call them, but if the header doesn't have them, we skip.
    // But we must compile. So we use a workaround: 
    // Check if they exist via SFINAE or just skip if not available.
    // Since we can't use templates easily in the fuzzer entry, we'll just skip if not found.
    // But the code must compile. The error says they are not in the namespace.
    // We will remove the direct calls to base64_encode/decode if they are not available.
    // Instead, we rely on DecodeDataURI which uses them internally.
    
    // 3. Test tinygltf::DecodeDataURI
    std::string raw_input_str(reinterpret_cast<const char*>(Data), Size);
    std::vector<unsigned char> out_vec;
    std::string mime_type;
    bool check_size = (Size % 2 == 0);
    size_t req_bytes = (Size > 100) ? 100 : Size;
    
    // Construct a fake data URI using the raw input as base64 part (might be invalid, which is good for fuzzing)
    std::string data_uri = "data:image/png;base64," + raw_input_str;
    
    tinygltf::DecodeDataURI(&out_vec, mime_type, data_uri, req_bytes, check_size);
    
    // Test with invalid URI
    tinygltf::DecodeDataURI(&out_vec, mime_type, raw_input_str, 0, false);

    // 4. Test tinygltf::LoadImageData
    tinygltf::Image image;
    std::string err_str;
    std::string warn_str;
    
    tinygltf::LoadImageData(&image, 0, &err_str, &warn_str, -1, -1, Data, Size, nullptr);
    
    if (!out_vec.empty()) {
        tinygltf::LoadImageData(&image, 1, &err_str, &warn_str, -1, -1,
                                out_vec.data(), static_cast<int>(out_vec.size()), nullptr);
    }

    // 5. Test tinygltf::WriteImageData
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = FsFileExists;
    fs_cb.ExpandFilePath = FsExpandFilePath;
    fs_cb.ReadWholeFile = FsReadWholeFile;
    fs_cb.WriteWholeFile = FsWriteWholeFile;
    fs_cb.GetFileSizeInBytes = FsGetFileSizeInBytes;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    // Assign functions matching the expected signatures (bool return, output to pointer)
    uri_cb.encode = UriEncodeFunc;
    uri_cb.decode = UriDecodeFunc;
    uri_cb.user_data = nullptr;

    std::string basepath = "./";
    std::string filename = "fuzz_test.png";
    std::string out_uri;
    
    tinygltf::Image img_to_write;
    img_to_write.name = "fuzz_img";
    img_to_write.width = (Size > 0) ? static_cast<int>(Data[0] % 256) : 1;
    img_to_write.height = (Size > 1) ? static_cast<int>(Data[1] % 256) : 1;
    if (img_to_write.width == 0) img_to_write.width = 1;
    if (img_to_write.height == 0) img_to_write.height = 1;
    
    img_to_write.image.resize(img_to_write.width * img_to_write.height * 4);
    for (size_t i = 0; i < img_to_write.image.size() && i < Size; ++i) {
        img_to_write.image[i] = Data[i % Size];
    }

    // Test with embedImages = true
    tinygltf::WriteImageData(&basepath, &filename, &img_to_write, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Test with embedImages = false
    WriteDummyFile(Data, Size);
    tinygltf::WriteImageData(&basepath, &filename, &img_to_write, false, &fs_cb, &uri_cb, &out_uri, nullptr);

    return 0;
}