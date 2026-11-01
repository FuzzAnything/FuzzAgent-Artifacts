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
#include <fstream>
#include <iomanip>
#include <functional>

// Include the library header
#include "tiny_gltf.h"

// Helper to convert binary data to base64 string for Data URI construction
static std::string Base64Encode(const uint8_t* data, size_t size) {
    static const char* base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    std::string ret;
    int in_len = size;
    int i = 0, j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(data++);
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

// Helper to write data to dummy file
static bool WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) return false;
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    return true;
}

// Helper to read data from dummy file
static bool ReadDummyFile(std::vector<unsigned char>& out, std::string* err) {
    std::ifstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out.resize(size);
    file.read(reinterpret_cast<char*>(out.data()), size);
    file.close();
    return true;
}

// Callback implementations for FsCallbacks
static bool ReadWholeFileCallbackFixed(std::vector<unsigned char>* out, std::string* err,
                                       const std::string& filename, void* user_data) {
    if (filename == "./dummy_file") {
        return ReadDummyFile(*out, err);
    }
    if (err) *err = "File not found";
    return false;
}

static bool WriteWholeFileCallback(std::string* err, const std::string& filename,
                                   const std::vector<unsigned char>& contents, void* user_data) {
    if (filename == "./dummy_file") {
        return WriteDummyFile(contents.data(), contents.size());
    }
    if (err) *err = "Write failed";
    return false;
}

static bool GetFileSizeInBytesCallbackFixed(size_t* size, std::string* err, const std::string& filename, void* user_data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t s = static_cast<size_t>(file.tellg());
    if (size) *size = s;
    file.close();
    return true;
}

static bool FileExistsCallback(const std::string& filename, void* user_data) {
    return true;
}

static std::string ExpandFilePathCallback(const std::string& filepath, void* user_data) {
    return filepath;
}

// Callback implementations for URICallbacks
static bool URIEncodeCallbackFixed(const std::string& str, const std::string& algo, std::string* out, void* user_data) {
    if (out) *out = str;
    return true;
}

static bool URIDecodeCallbackFixed(const std::string& str, std::string* out, void* user_data) {
    if (out) *out = str;
    return true;
}

static bool WriteImageDataCallback(const std::string* basepath, const std::string* filename,
                                   const tinygltf::Image* image, bool embedImages,
                                   const tinygltf::FsCallbacks* fs_cb,
                                   const tinygltf::URICallbacks* uri_cb,
                                   std::string* out_uri, void* user_data) {
    if (embedImages) {
        std::string b64 = Base64Encode(image->image.data(), image->image.size());
        std::string mime = "png"; 
        if (mime.empty()) mime = "png";
        *out_uri = "data:image/" + mime + ";base64," + b64;
        return true;
    } else {
        if (fs_cb && fs_cb->WriteWholeFile) {
            std::string err;
            std::string fname = filename ? *filename : "dummy_file";
            if (fs_cb->WriteWholeFile(&err, fname, image->image, user_data)) {
                *out_uri = fname;
                return true;
            }
        }
        return false;
    }
}

// Static helper to mimic MimeToExt if not exposed globally
static std::string MimeToExt(const std::string& mimeType) {
    if (mimeType.find("image/png") != std::string::npos) return "png";
    if (mimeType.find("image/jpeg") != std::string::npos) return "jpg";
    if (mimeType.find("image/bmp") != std::string::npos) return "bmp";
    if (mimeType.find("image/webp") != std::string::npos) return "webp";
    if (mimeType.find("image/ktx2") != std::string::npos) return "ktx2";
    if (mimeType.find("image/ktx") != std::string::npos) return "ktx";
    if (mimeType.find("image/ds") != std::string::npos) return "ds";
    if (mimeType.find("image/dds") != std::string::npos) return "dds";
    if (mimeType.find("image/tga") != std::string::npos) return "tga";
    if (mimeType.find("image/gif") != std::string::npos) return "gif";
    if (mimeType.find("image/avif") != std::string::npos) return "avif";
    if (mimeType.find("image/heic") != std::string::npos) return "heic";
    if (mimeType.find("image/heif") != std::string::npos) return "heif";
    return "";
}

extern "C" int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = FileExistsCallback;
    fs_cb.ExpandFilePath = ExpandFilePathCallback;
    fs_cb.ReadWholeFile = ReadWholeFileCallbackFixed;
    fs_cb.WriteWholeFile = WriteWholeFileCallback;
    fs_cb.GetFileSizeInBytes = GetFileSizeInBytesCallbackFixed;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = URIEncodeCallbackFixed;
    uri_cb.decode = URIDecodeCallbackFixed;
    uri_cb.user_data = nullptr;

    // 1. Test MimeToExt (using local implementation since it's static/internal)
    std::string mime_type = "image/png";
    std::string ext = MimeToExt(mime_type);
    (void)ext; // Suppress unused warning

    // 2. Test DecodeDataURI
    std::string data_uri = "data:image/png;base64,";
    if (Size > 5) {
        std::string b64_part = Base64Encode(Data + 5, Size - 5);
        data_uri += b64_part;
    } else {
        data_uri += "iVBORw0KGgo="; // Minimal valid PNG base64
    }

    std::vector<unsigned char> decoded_out;
    std::string mime_out;
    bool decode_res = tinygltf::DecodeDataURI(&decoded_out, mime_out, data_uri, 0, false);
    bool decode_res2 = tinygltf::DecodeDataURI(&decoded_out, mime_out, data_uri, Size, true);
    (void)decode_res;
    (void)decode_res2;

    // 3. Test LoadImageData
    tinygltf::Image img;
    std::string err, warn;
    bool load_res = tinygltf::LoadImageData(&img, 0, &err, &warn, 0, 0, Data, static_cast<int>(Size), nullptr);
    (void)load_res;

    // 4. Test SerializeGltfImage
    // Skipped as it requires internal detail::json type which is not accessible.

    // 5. Test WriteImageData
    std::string out_uri;
    std::string basepath = "./";
    std::string filename = "./dummy_file";
    bool write_res = tinygltf::WriteImageData(&basepath, &filename, &img, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    bool write_res2 = tinygltf::WriteImageData(&basepath, &filename, &img, false, &fs_cb, &uri_cb, &out_uri, nullptr);
    (void)write_res;
    (void)write_res2;

    // 6. Test UpdateImageObject
    // Skipped as it is static and likely in internal detail namespace.

    // Cleanup
    std::remove("./dummy_file");

    return 0;
}