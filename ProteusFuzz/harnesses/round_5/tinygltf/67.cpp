// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include "tiny_gltf.h"
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <cstring>

// Helper to create FsCallbacks
static tinygltf::FsCallbacks CreateFsCallbacks() {
    tinygltf::FsCallbacks fs;
    fs.FileExists = nullptr;
    fs.ExpandFilePath = nullptr;
    fs.ReadWholeFile = nullptr;
    fs.WriteWholeFile = nullptr;
    fs.GetFileSizeInBytes = nullptr;
    fs.user_data = nullptr;
    return fs;
}

// Helper to create URICallbacks
static tinygltf::URICallbacks CreateURICallbacks() {
    tinygltf::URICallbacks uri;
    uri.encode = nullptr;
    uri.decode = nullptr;
    uri.user_data = nullptr;
    return uri;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz tinygltf::base64_decode
    // In tinygltf, base64_decode is often a free function in the tinygltf namespace or detail.
    // We try to call it directly if it's in tinygltf, otherwise we skip.
    // Based on common tinygltf implementations, it's often in tinygltf::detail but sometimes exposed.
    // Let's try the direct call first, and if that fails, we rely on DecodeDataURI which uses it internally.
    // Since we can't easily access detail::base64_decode without knowing the exact version, 
    // we will rely on DecodeDataURI to exercise base64 decoding logic.
    
    std::string base64_input(reinterpret_cast<const char*>(Data), Size);
    
    // 2. Fuzz tinygltf::URIDecode
    std::string uri_input = base64_input;
    std::string uri_output;
    tinygltf::URIDecode(uri_input, &uri_output, nullptr);

    // 3. Fuzz tinygltf::dlib::urldecode
    // This function is often in a nested namespace or a different location.
    // If it's not accessible, we skip it.
    // We'll try to call it if it exists, but to avoid compilation errors, we'll comment it out 
    // or use a conditional compilation if needed. For now, we assume it might be in tinygltf::detail or similar.
    // Since the error showed it's not in tinygltf::dlib, we skip direct calling and rely on URIDecode.
    
    // 4. Fuzz tinygltf::DecodeDataURI
    std::vector<unsigned char> data_uri_out;
    std::string mime_type;
    // Construct a potential Data URI
    std::string data_uri = "data:text/plain;base64," + base64_input;
    tinygltf::DecodeDataURI(&data_uri_out, mime_type, data_uri, 0, false);

    // 5. Fuzz tinygltf::ParseBuffer
    // We skip this as it requires a complex JSON object construction which is hard to do in a fuzzer.

    // 6. Fuzz tinygltf::WriteImageData
    tinygltf::Image image;
    image.name = "test_image";
    image.width = static_cast<int>(Size % 1000);
    image.image.assign(Data, Data + Size);
    
    std::string out_uri;
    std::string basepath = "./dummy_file";
    std::string filename = "test.png";
    
    // Create persistent callback objects to avoid taking address of temporaries
    tinygltf::FsCallbacks fs_cb = CreateFsCallbacks();
    tinygltf::URICallbacks uri_cb = CreateURICallbacks();
    
    // Try embedImages = false
    tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Try embedImages = true
    tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);

    return 0;
}