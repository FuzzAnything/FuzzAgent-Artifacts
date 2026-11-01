// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Helper to convert fuzzer input to a string, ensuring null termination if needed for paths
static std::string ToSafeString(const uint8_t *data, size_t size) {
    if (size == 0) return "";
    // Ensure we don't read beyond bounds if size is large but data is small (though Size is trusted by fuzzer)
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    const char *dummy_file_bin = "./dummy_file.bin";
    const char *dummy_file_gltf = "./dummy_file.gltf";
    const char *dummy_file_glb = "./dummy_file.glb";

    // Write raw fuzzer data to dummy files to ensure they exist for FileExists tests
    std::ofstream out_dummy(dummy_file, std::ios::binary);
    if (out_dummy.is_open()) {
        out_dummy.write(reinterpret_cast<const char*>(Data), Size);
        out_dummy.close();
    }

    tinygltf::TinyGLTF parser;
    tinygltf::Model model;

    uint8_t selector = Data[0];

    // --- Test Case 1: FileExists ---
    if (selector < 10) {
        std::string path = ToSafeString(Data + 1, Size - 1);
        if (path.empty()) path = dummy_file;
        
        bool exists = tinygltf::FileExists(path, nullptr);
        (void)exists;
    }

    // --- Test Case 2: WriteWholeFile ---
    else if (selector < 20) {
        std::string err;
        std::string filepath = ToSafeString(Data + 1, Size - 1);
        if (filepath.empty()) filepath = dummy_file;
        
        // Ensure contents is not empty to avoid accessing empty vector in WriteWholeFile
        std::vector<unsigned char> contents;
        if (Size > 1) {
            contents.assign(Data + 1, Data + Size);
        } else {
            // Provide a dummy byte if input is too small
            contents.push_back(0);
        }
        
        bool success = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
        (void)success;
    }

    // --- Test Case 3: WriteGltfSceneToFile ---
    else if (selector < 30) {
        model.asset.version = "2.0";
        
        std::string filename = ToSafeString(Data + 1, Size - 1);
        if (filename.empty()) filename = dummy_file_gltf;
        
        bool embedImages = (Size > 1 && Data[1] % 2 == 0);
        bool embedBuffers = (Size > 2 && Data[2] % 2 == 0);
        bool prettyPrint = (Size > 3 && Data[3] % 2 == 0);
        bool writeBinary = (Size > 4 && Data[4] % 2 == 0);
        
        bool success = parser.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
        (void)success;
    }

    // --- Test Case 4: WriteBinaryGltfFile (via WriteGltfSceneToFile) ---
    else if (selector < 40) {
        model.asset.version = "2.0";
        std::string filename = ToSafeString(Data + 1, Size - 1);
        if (filename.empty()) filename = dummy_file_glb;
        
        bool embedImages = false;
        bool embedBuffers = true; // Embed buffers for binary format
        bool prettyPrint = false;
        bool writeBinary = true;
        
        bool success = parser.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
        (void)success;
    }

    // --- Test Case 5: SerializeGltfBufferData (via WriteGltfSceneToFile) ---
    else if (selector < 50) {
        // Create a model with a buffer and write it to test serialization
        tinygltf::Buffer buffer;
        if (Size > 1) {
            buffer.data.assign(Data + 1, Data + Size);
        } else {
            buffer.data.push_back(0);
        }
        buffer.name = "test_buffer";
        model.buffers.push_back(buffer);
        
        tinygltf::BufferView view;
        view.buffer = 0;
        view.byteOffset = 0;
        view.byteLength = buffer.data.size();
        model.bufferViews.push_back(view);
        
        std::string filename = ToSafeString(Data + 1, Size - 1);
        if (filename.empty()) filename = dummy_file_gltf;
        
        bool success = parser.WriteGltfSceneToFile(&model, filename, false, true, false, false);
        (void)success;
    }

    // --- Test Case 6: WriteGltfFile (via WriteGltfSceneToFile) ---
    else {
        model.asset.version = "2.0";
        std::string filename = ToSafeString(Data + 1, Size - 1);
        if (filename.empty()) filename = dummy_file_gltf;
        
        bool success = parser.WriteGltfSceneToFile(&model, filename, false, false, true, false);
        (void)success;
    }

    // Cleanup
    std::remove(dummy_file);
    std::remove(dummy_file_bin);
    std::remove(dummy_file_gltf);
    std::remove(dummy_file_glb);

    return 0;
}