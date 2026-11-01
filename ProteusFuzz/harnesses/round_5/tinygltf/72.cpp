// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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

static void WriteDummyFile(const std::string &filename, const std::vector<unsigned char> &data) {
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
        ofs.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the first byte to determine which function to fuzz
    int mode = Data[0] % 5;
    const uint8_t *input_data = Data + 1;
    size_t input_size = Size - 1;

    // Create a dummy file for file-based operations
    const std::string dummy_file = "./dummy_file";
    std::vector<unsigned char> file_content(input_data, input_data + input_size);
    WriteDummyFile(dummy_file, file_content);

    std::string err;
    std::string warn;
    std::string expanded_path;
    std::vector<unsigned char> out_buffer;
    size_t file_size = 0;
    bool result = false;

    switch (mode) {
        case 0: {
            // Fuzz ExpandFilePath
            std::string path_str(reinterpret_cast<const char*>(input_data), input_size);
            expanded_path = tinygltf::ExpandFilePath(path_str, nullptr);
            break;
        }
        case 1: {
            // Fuzz ReadWholeFile
            std::string path_str = dummy_file;
            result = tinygltf::ReadWholeFile(&out_buffer, &err, path_str, nullptr);
            break;
        }
        case 2: {
            // Fuzz FileExists
            std::string path_str = dummy_file;
            result = tinygltf::FileExists(path_str, nullptr);
            break;
        }
        case 3: {
            // Fuzz WriteWholeFile
            // Ensure file_content is not empty to avoid out_of_range in tinygltf::WriteWholeFile
            if (file_content.empty()) {
                file_content.push_back(0); // Add a dummy byte if empty
            }
            std::string path_str = dummy_file + "_write";
            result = tinygltf::WriteWholeFile(&err, path_str, file_content, nullptr);
            break;
        }
        case 4: {
            // Fuzz GetFileSizeInBytes
            std::string path_str = dummy_file;
            result = tinygltf::GetFileSizeInBytes(&file_size, &err, path_str, nullptr);
            break;
        }
    }

    return 0;
}