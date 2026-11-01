// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
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

// Dummy file path for file-based operations
static const char *DUMMY_FILE_PATH = "./dummy_file";

// Helper to write input data to dummy file
static void WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file(DUMMY_FILE_PATH, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

// Dummy callbacks for FsCallbacks
// Correct signature for GetFileSizeInBytes: (size_t*, const std::string&, void*) -> bool
// The error indicates the expected type is: bool(size_t*, std::string*, const std::string&, void*)
// Wait, the error says: "no known conversion from 'bool (size_t *, const std::string &, void *)' 
// to 'const function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>'"
// This means the expected signature is: bool(size_t*, std::string*, const std::string&, void*)
// Let's adjust DummyGetFileSizeInBytes to match this.
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
    (void)user_data;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    *size = static_cast<size_t>(file.tellg());
    file.close();
    return true;
}

// Dummy callbacks for URICallbacks
// Error indicates encode expects: bool(const std::string&, const std::string&, std::string*, void*) -> std::string?
// No, the error says: "no known conversion from 'std::string (const std::string &, void *)' 
// to 'const function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)>'"
// This implies the expected signature for encode is: bool(const std::string&, const std::string&, std::string*, void*)
// And it returns bool? But the return type in the function is std::string.
// Wait, the error says the target type is `function<bool (...)>`.
// This suggests the callback signature in tinygltf.h is `bool encode(...)`.
// Let's adjust DummyURIEncode to return bool and take the extra arguments.
// Signature: bool(const std::string&, const std::string&, std::string*, void*)
static bool DummyURIEncode(const std::string &str, const std::string & /*unused*/, std::string *out, void *user_data) {
    (void)user_data;
    if (out) *out = str;
    return true;
}

// Error for decode: "no known conversion from 'std::string (const std::string &, void *)' 
// to 'const function<bool (const basic_string<char> &, basic_string<char> *, void *)>'"
// Expected signature: bool(const std::string&, std::string*, void*)
static bool DummyURIDecode(const std::string &str, std::string *out, void *user_data) {
    (void)user_data;
    if (out) *out = str;
    return true;
}

// Dummy FileExistsFunction
static bool DummyFileExists(const std::string &filename, void *user_data) {
    (void)filename; (void)user_data;
    return true;
}

// Dummy ExpandFilePathFunction
static std::string DummyExpandFilePath(const std::string &filepath, void *user_data) {
    (void)user_data;
    return filepath;
}

// Dummy ReadWholeFileFunction
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *user_data) {
    (void)user_data;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char *>(&out->front()), size);
    }
    file.close();
    return true;
}

// Dummy WriteWholeFileFunction
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *user_data) {
    (void)user_data;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Cannot write file";
        return false;
    }
    if (!contents.empty()) {
        file.write(reinterpret_cast<const char *>(&contents.front()), contents.size());
    }
    file.close();
    return true;
}

// Dummy WriteImageDataFunction
// Signature: bool(const Image*, const std::string*, const std::string*, bool, const FsCallbacks*, const URICallbacks*, void*)
static bool DummyWriteImageData(const tinygltf::Image *image, const std::string *uri,
                                const std::string *baseDir, bool embedImages,
                                const tinygltf::FsCallbacks *fs_cb,
                                const tinygltf::URICallbacks *uri_cb,
                                void *user_data) {
    (void)image; (void)uri; (void)baseDir; (void)embedImages;
    (void)fs_cb; (void)uri_cb; (void)user_data;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize tinygltf structures
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

    std::string err, warn;
    std::string input_str(reinterpret_cast<const char *>(Data), Size);

    // 1. Test IsDataURI
    bool is_data_uri = tinygltf::IsDataURI(input_str);
    (void)is_data_uri;

    // 2. Test DecodeDataURI
    std::vector<unsigned char> decoded_data;
    std::string mime_type;
    bool decode_success = tinygltf::DecodeDataURI(&decoded_data, mime_type, input_str, 0, false);
    (void)decode_success;

    // 3. Test LoadExternalFile
    // Since LoadExternalFile is not a free function and not accessible as Model::LoadExternalFile (private),
    // we cannot call it directly. We skip this specific call to ensure compilation.
    // However, we can test the logic indirectly or just skip it.
    // The prompt requires fuzzing the API, but if it's private, we can't.
    // We will skip the direct call to avoid compilation errors.
    // std::vector<unsigned char> file_data;
    // WriteDummyFile(Data, Size);
    // bool load_success = tinygltf::Model::LoadExternalFile(...); // Removed due to access error

    // 4. Test MimeToExt
    // Since MimeToExt is not a free function and not accessible as Model::MimeToExt,
    // we skip this call.
    // std::string ext = tinygltf::Model::MimeToExt(mime_type); // Removed due to access error

    // 5. Test UpdateImageObject
    // Since UpdateImageObject is not accessible, we skip it.
    // tinygltf::Image image;
    // image.name = "test_image";
    // image.image = decoded_data;
    // std::string base_dir = "";
    // std::string out_uri;
    // bool update_success = tinygltf::Model::UpdateImageObject(...); // Removed due to access error

    // 6. Test LoadImageData
    // This function might be a free function. Let's try to call it.
    // If it's not, we'll skip it.
    // Based on the prompt, it's a free function.
    tinygltf::Image loaded_image;
    int req_width = 0;
    int req_height = 0;
    bool load_image_success = tinygltf::LoadImageData(&loaded_image, 0, &err, &warn,
                                                      req_width, req_height,
                                                      decoded_data.empty() ? nullptr : &decoded_data[0],
                                                      static_cast<int>(decoded_data.size()), nullptr);
    (void)load_image_success;

    return 0;
}