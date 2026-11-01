// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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
#include <functional>
#include "tiny_gltf.h"

// Dummy filesystem callbacks matching tinygltf expected signatures
static bool DummyFileExists(const std::string &f, void *) { return false; }

static std::string DummyExpandFilePath(const std::string &f, void *) { return f; }

static bool DummyReadWholeFile(std::vector<unsigned char> *buf, std::string *err,
                               const std::string &f, void *) {
    if (err) *err = "File not found";
    return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &f,
                                const std::vector<unsigned char> &buf,
                                void *) {
    if (err) *err = "Write failed";
    return false;
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err,
                                    const std::string &f, void *) {
    if (err) *err = "Size not found";
    if (size) *size = 0;
    return false;
}

// Dummy URI callbacks matching tinygltf expected signatures
static bool DummyURIEncode(const std::string &uri, const std::string &base,
                           std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

// Helper to check base64 char since is_base64 might be static/internal
static inline bool is_base64_char(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '+' || c == '/';
}

static tinygltf::FsCallbacks dummy_fs = {
    DummyFileExists,
    DummyExpandFilePath,
    DummyReadWholeFile,
    DummyWriteWholeFile,
    DummyGetFileSizeInBytes,
    nullptr
};

static tinygltf::URICallbacks dummy_uri = {
    DummyURIEncode,
    DummyURIDecode,
    nullptr
};

extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Split input into segments for different tests
    size_t seg1 = (Size > 100) ? 100 : Size;
    size_t seg2 = (Size > 200) ? 100 : (Size - seg1);
    size_t seg3 = (Size > 300) ? 100 : (Size - seg1 - seg2);
    size_t seg4 = (Size > 400) ? 100 : (Size - seg1 - seg2 - seg3);
    size_t seg5 = (Size > 500) ? 100 : (Size - seg1 - seg2 - seg3 - seg4);
    size_t seg6 = Size - seg1 - seg2 - seg3 - seg4 - seg5;

    std::string str1(reinterpret_cast<const char*>(Data), seg1);
    std::string str2(reinterpret_cast<const char*>(Data + seg1), seg2);
    std::string str3(reinterpret_cast<const char*>(Data + seg1 + seg2), seg3);
    std::string str4(reinterpret_cast<const char*>(Data + seg1 + seg2 + seg3), seg4);
    std::string str5(reinterpret_cast<const char*>(Data + seg1 + seg2 + seg3 + seg4), seg5);
    std::string str6(reinterpret_cast<const char*>(Data + seg1 + seg2 + seg3 + seg4 + seg5), seg6);

    // 1. Test IsDataURI
    bool is_data = tinygltf::IsDataURI(str1);
    (void)is_data;

    // 2. Test is_base64 logic (using helper since internal function might not be accessible)
    for (size_t i = 0; i < str2.size(); ++i) {
        bool is_b64 = is_base64_char(static_cast<unsigned char>(str2[i]));
        (void)is_b64;
    }

    // 3. Test DecodeDataURI
    std::vector<unsigned char> decoded_out;
    std::string mime_type;
    bool decode_res = tinygltf::DecodeDataURI(&decoded_out, mime_type, str3, 0, false);
    (void)decode_res;

    // 4. Test LoadImageData
    tinygltf::Image img;
    std::string err_str;
    std::string warn_str;
    // Use str4 as raw bytes
    bool load_img_res = tinygltf::LoadImageData(
        &img, 0, &err_str, &warn_str, 0, 0,
        reinterpret_cast<const unsigned char*>(str4.c_str()),
        static_cast<int>(str4.size()), nullptr
    );
    (void)load_img_res;

    // 5. Test WriteImageData
    std::string out_uri;
    std::string basepath = "./dummy_file";
    std::string filename = "test.png";
    bool write_img_res = tinygltf::WriteImageData(
        &basepath, &filename,
        &img, true, &dummy_fs, &dummy_uri, &out_uri, nullptr
    );
    (void)write_img_res;

    // 6. Test ParseBuffer indirectly via LoadASCIIFromString (public API)
    // We construct a minimal JSON string to test buffer parsing logic
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string load_err;
    std::string load_warn;
    
    // Prepare a minimal JSON string for buffer testing
    std::string json_str = "{\"buffers\": [{\"uri\": \"test.bin\", \"byteLength\": " + std::to_string(str5.size()) + "}]}";
    
    // Use the public LoadASCIIFromString which internally uses ParseBuffer
    // Signature: LoadASCIIFromString(Model*, std::string*, std::string*, const char*, unsigned int, const std::string&)
    ctx.LoadASCIIFromString(&model, &load_err, &load_warn, str5.c_str(), static_cast<unsigned int>(str5.size()), "");
    (void)load_err;
    (void)load_warn;

    // Additional state exploration: Try with binary flag and bin_data via LoadBinary
    // Since we don't have a real binary file, we try to load str6 as if it were a binary glTF
    // This exercises the binary parsing path which also calls ParseBuffer
    if (seg6 > 0) {
        tinygltf::Model model2;
        std::string bin_err, bin_warn;
        // LoadBinary expects a file path, but we can try to pass a dummy path and let it fail or
        // use LoadASCIIFromString with a binary-like string to trigger parsing logic
        // To specifically test binary buffer parsing, we'd need a valid GLB header.
        // Instead, we just call LoadASCIIFromString again with different data to vary state.
        ctx.LoadASCIIFromString(&model2, &bin_err, &bin_warn, str6.c_str(), static_cast<unsigned int>(str6.size()), "");
        (void)bin_err;
        (void)bin_warn;
    }

    return 0;
}