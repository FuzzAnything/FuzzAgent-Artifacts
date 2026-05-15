// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static bool dummyFileExists(const std::string&, void*) { return false; }
static std::string dummyExpandFilePath(const std::string& path, void*) { return path; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return false; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return false; }
static bool dummyGetFileSizeInBytes(size_t*, std::string*, const std::string&, void*) { return false; }

static bool dummyURIEncode(const std::string&, const std::string&, std::string*, void*) { return false; }

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 11) return 0;

    // Initialize dummy file for potential file operations
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }

    // Split input for different parameters
    size_t split1 = Size / 6;
    size_t split2 = split1 * 2;
    size_t split3 = split1 * 3;
    size_t split4 = split1 * 4;
    size_t split5 = split1 * 5;

    // Ensure splits don't exceed bounds
    if (split1 == 0) split1 = 1;
    if (split2 > Size) split2 = Size;
    if (split3 > Size) split3 = Size;
    if (split4 > Size) split4 = Size;
    if (split5 > Size) split5 = Size;

    // Test IsDataURI
    std::string data_uri_str(reinterpret_cast<const char*>(Data), std::min(split1, Size));
    bool is_data_uri = tinygltf::IsDataURI(data_uri_str);

    // Test URIDecode
    size_t decode_size = (split2 > split1) ? (split2 - split1) : 0;
    if (decode_size > 0 && split1 < Size) {
        std::string decode_input(reinterpret_cast<const char*>(Data + split1), decode_size);
        std::string decode_output;
        bool decode_result = tinygltf::URIDecode(decode_input, &decode_output, nullptr);
    }

    // Test DecodeDataURI
    size_t uri_size = (split3 > split2) ? (split3 - split2) : 0;
    if (uri_size > 0 && split2 < Size) {
        std::string uri_input(reinterpret_cast<const char*>(Data + split2), uri_size);
        std::vector<unsigned char> out_vec;
        std::string mime_type;
        bool check_size = (Data[0] & 1) == 0;
        size_t req_bytes = 0;
        if (Size >= 3) {
            req_bytes = (Data[1] << 8) | Data[2];
        }
        bool decode_uri_result = tinygltf::DecodeDataURI(&out_vec, mime_type, uri_input, req_bytes, check_size);
    }

    // Test LoadImageData
    if (split3 < Size) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        int req_width = 0;
        int req_height = 0;
        if (Size >= 7) {
            req_width = static_cast<int>(Data[3]) * 256 + Data[4];
            req_height = static_cast<int>(Data[5]) * 256 + Data[6];
        }
        const unsigned char* bytes = Data + split3;
        int bytes_size = static_cast<int>(Size - split3);
        bool load_image_result = tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, bytes, bytes_size, nullptr);
    }

    // Test SetURICallbacks
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = dummyURIEncode;
    uri_callbacks.decode = tinygltf::URIDecode;
    uri_callbacks.user_data = nullptr;
    std::string seturi_err;
    bool seturi_result = loader.SetURICallbacks(uri_callbacks, &seturi_err);

    // Test WriteImageData
    if (split4 < Size && split5 <= Size) {
        std::string basepath = "./";
        std::string filename = "dummy_file";
        tinygltf::Image write_image;
        write_image.width = 0;
        if (Size >= 9) {
            write_image.width = static_cast<int>(Data[7]) * 256 + Data[8];
        }
        
        size_t image_size = (split5 > split4) ? (split5 - split4) : 0;
        if (image_size > 0 && split4 < Size) {
            write_image.image.assign(Data + split4, Data + split5);
        }
        
        write_image.as_is = false;
        if (Size >= 10) {
            write_image.as_is = (Data[9] & 1) == 1;
        }
        
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = dummyFileExists;
        fs_callbacks.ExpandFilePath = dummyExpandFilePath;
        fs_callbacks.ReadWholeFile = dummyReadWholeFile;
        fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        fs_callbacks.user_data = nullptr;
        
        tinygltf::URICallbacks write_uri_callbacks;
        write_uri_callbacks.encode = dummyURIEncode;
        write_uri_callbacks.decode = tinygltf::URIDecode;
        write_uri_callbacks.user_data = nullptr;
        
        std::string out_uri;
        bool embed_images = false;
        if (Size >= 11) {
            embed_images = (Data[10] & 1) == 1;
        }
        bool write_result = tinygltf::WriteImageData(&basepath, &filename, &write_image, embed_images, &fs_callbacks, &write_uri_callbacks, &out_uri, nullptr);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}