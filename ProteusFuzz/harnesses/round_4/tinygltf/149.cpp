// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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

static void FuzzExpandFilePath(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    std::string expanded = tinygltf::ExpandFilePath(input, nullptr);
    (void)expanded;
}

static void FuzzFileExists(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    bool exists = tinygltf::FileExists(input, nullptr);
    (void)exists;
}

static void FuzzGetBaseFilename(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    // Manually implement GetBaseFilename logic since it might not be exposed as a free function
    // Logic: find last occurrence of '/' or '\', return substring after it, or original if not found
    size_t pos = input.find_last_of("/\\");
    std::string base;
    if (pos == std::string::npos) {
        base = input;
    } else {
        base = input.substr(pos + 1);
    }
    (void)base;
}

static void FuzzReadWholeFile(const uint8_t *Data, size_t Size) {
    std::string filepath = "./dummy_file";
    {
        std::ofstream file(filepath, std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
        }
    }

    std::vector<unsigned char> out;
    std::string err;
    bool success = tinygltf::ReadWholeFile(&out, &err, filepath, nullptr);
    (void)success;
    (void)err;
}

static void FuzzGetFileSizeInBytes(const uint8_t *Data, size_t Size) {
    std::string filepath = "./dummy_file";
    {
        std::ofstream file(filepath, std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
        }
    }

    size_t filesize_out = 0;
    std::string err;
    bool success = tinygltf::GetFileSizeInBytes(&filesize_out, &err, filepath, nullptr);
    (void)success;
    (void)filesize_out;
    (void)err;
}

static void FuzzWriteWholeFile(const uint8_t *Data, size_t Size) {
    std::string filepath = "./dummy_file";
    std::string err;
    std::vector<unsigned char> contents(Data, Data + Size);
    bool success = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
    (void)success;
    (void)err;
}

extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    FuzzExpandFilePath(Data, Size);
    FuzzFileExists(Data, Size);
    FuzzGetBaseFilename(Data, Size);
    FuzzReadWholeFile(Data, Size);
    FuzzGetFileSizeInBytes(Data, Size);
    FuzzWriteWholeFile(Data, Size);

    return 0;
}