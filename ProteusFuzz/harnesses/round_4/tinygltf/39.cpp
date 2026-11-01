// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
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
#include <functional>

// Static dummy implementations for callbacks
static bool DummyFileExists(const std::string &f, void *) { return false; }
static std::string DummyExpandFilePath(const std::string &f, void *) { return f; }
static bool DummyReadWholeFile(std::vector<unsigned char> *, std::string *, const std::string &, void *) { return false; }
static bool DummyWriteWholeFile(std::string *, const std::string &, const std::vector<unsigned char> &, void *) { return false; }
static bool DummyGetFileSizeInBytes(size_t *out_size, std::string *err, const std::string &filename, void *user_data) {
    (void)filename; (void)user_data;
    if (out_size) *out_size = 0;
    if (err) *err = "";
    return true;
}

static bool DummyURIEncode(const std::string &in, const std::string &basepath, std::string *out, void *user_data) {
    (void)in; (void)basepath; (void)user_data;
    if (out) *out = "";
    return true;
}

static bool DummyURIDecode(const std::string &in, std::string *out, void *user_data) {
    (void)in; (void)user_data;
    if (out) *out = in;
    return true;
}

static bool DummyLoadImageData(tinygltf::Image *, int, int, std::string *, std::string *, const std::string &, const tinygltf::FsCallbacks *, void *) {
    return false;
}

static bool DummyWriteImageDataFunc(const std::string *basepath, const std::string *filename, const tinygltf::Image *image, bool embedImages, const tinygltf::FsCallbacks *fs_cb, const tinygltf::URICallbacks *uri_cb, std::string *out_uri, void *user_data) {
    (void)basepath; (void)filename; (void)image; (void)embedImages; (void)fs_cb; (void)uri_cb; (void)user_data;
    if (out_uri) *out_uri = "dummy_uri";
    return true;
}

// Helper to safely parse JSON if possible, otherwise return a default object
#ifdef TINYGLTF_HAS_JSON
#include <nlohmann/json.hpp>
using JsonType = nlohmann::json;
#else
// Fallback: use tinygltf::detail::json if available
// If not, we will use a dummy type
namespace tinygltf {
namespace detail {
// Dummy json type for compilation
class json {
public:
    json() {}
    json(const std::string &s) {}
    // Minimal interface to satisfy ParseImage
    bool is_object() const { return true; }
    bool is_null() const { return false; }
    bool is_string() const { return true; }
    bool is_number() const { return false; }
    bool is_boolean() const { return false; }
    bool is_array() const { return false; }
    bool is_discarded() const { return false; }
    bool is_primitive() const { return true; }
    bool is_structured() const { return false; }
    bool is_binary() const { return false; }
    
    // Minimal operators
    json& operator[](const std::string &key) { return *this; }
    json& operator[](size_t index) { return *this; }
    std::string dump() const { return "{}"; }
    
    static json parse(const std::string &s) { return json(s); }
};
} // namespace detail
} // namespace tinygltf
using JsonType = tinygltf::detail::json;
#endif

static JsonType SafeParseJSON(const std::string &input) {
    try {
        return JsonType::parse(input);
    } catch (...) {
        // Return a minimal valid JSON object if parsing fails
        JsonType o;
        return o;
    }
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = DummyFileExists;
    fs_cb.ExpandFilePath = DummyExpandFilePath;
    fs_cb.ReadWholeFile = DummyReadWholeFile;
    fs_cb.WriteWholeFile = DummyWriteWholeFile;
    fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = DummyURIEncode;
    uri_cb.decode = DummyURIDecode;
    uri_cb.user_data = nullptr;

    std::string input(reinterpret_cast<const char*>(Data), Size);

    // 1. Test IsDataURI
    tinygltf::IsDataURI(input);

    // 2. Test URIDecode
    std::string decoded_uri;
    tinygltf::URIDecode(input, &decoded_uri, nullptr);

    // 3. Test DecodeDataURI
    std::vector<unsigned char> decoded_data;
    std::string mime_type;
    // Try with reqBytes = 0 and checkSize = false first
    tinygltf::DecodeDataURI(&decoded_data, mime_type, input, 0, false);
    // Try with reqBytes = Size and checkSize = true
    tinygltf::DecodeDataURI(&decoded_data, mime_type, input, Size, true);

    // 4. Test WriteImageData
    tinygltf::Image img;
    img.image.assign(decoded_data.begin(), decoded_data.end());
    img.width = (Size > 0) ? static_cast<int>(Data[0]) : 0;
    img.name = "test_image";
    std::string basepath = "./dummy_file";
    std::string filename = "dummy.png";
    std::string out_uri;
    
    // Test with embedImages = false
    tinygltf::WriteImageData(&basepath, &filename, &img, false, &fs_cb, &uri_cb, &out_uri, nullptr);
    // Test with embedImages = true
    tinygltf::WriteImageData(&basepath, &filename, &img, true, &fs_cb, &uri_cb, &out_uri, nullptr);

    // 5. Test UpdateImageObject
    // Since the error says it's not in the namespace, we will skip it for now.
    // The user will need to adjust if it's available in their version.
    // std::string baseDir = "./dummy_file";
    // std::string out_uri_update;
    // int index = (Size > 1) ? static_cast<int>(Data[1]) : 0;
    // tinygltf::UpdateImageObject(img, baseDir, index, false, &fs_cb, &uri_cb, DummyWriteImageDataFunc, nullptr, &out_uri_update);
    // tinygltf::UpdateImageObject(img, baseDir, index, true, &fs_cb, &uri_cb, DummyWriteImageDataFunc, nullptr, &out_uri_update);

    // 6. Test ParseImage
    // ParseImage is a static function inside the TinyGLTF class or a helper function.
    // Since it's not directly accessible in the tinygltf namespace, we will skip it for now.
    // The user will need to adjust if it's available in their version.
    // JsonType o = SafeParseJSON(input);
    // std::string err;
    // std::string warn;
    // int index = (Size > 1) ? static_cast<int>(Data[1]) : 0;
    // tinygltf::ParseImage(&img, index, &err, &warn, o, false, "./dummy_file", Size, &fs_cb, &uri_cb, DummyLoadImageData, nullptr);

    return 0;
}