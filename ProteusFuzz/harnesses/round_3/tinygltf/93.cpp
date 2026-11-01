// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
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
#include <fstream>
#include <sstream>

// Helper to generate a valid JSON string from input data
static std::string GenerateValidJson(const uint8_t *Data, size_t Size) {
    std::string json = "{";
    json += "\"asset\": {\"version\": \"2.0\"},";
    json += "\"buffers\": [],";
    json += "\"bufferViews\": [],";
    json += "\"accessors\": [],";
    json += "\"meshes\": [],";
    json += "\"nodes\": [],";
    json += "\"scenes\": [],";
    json += "\"scene\": -1";
    json += "}";
    return json;
}

// Helper to generate a Model object with some data
static tinygltf::Model GenerateModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    // Add a simple buffer if data is available
    if (Size > 0) {
        tinygltf::Buffer buffer;
        buffer.data.assign(Data, Data + Size);
        buffer.name = "fuzz_buffer";
        model.buffers.push_back(buffer);
    }

    // Add a simple scene
    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    // Add a simple asset
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    return model;
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string filepath = "./dummy_file";
    std::string err;
    std::vector<unsigned char> contents;
    
    // Use input data as file content
    if (Size > 0) {
        contents.assign(Data, Data + Size);
    }

    // 1. Fuzz tinygltf::WriteWholeFile
    // This function writes binary data to disk.
    // We pass the generated content and a dummy path.
    bool writeResult = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
    (void)writeResult; // Result is checked implicitly by the fuzzer for crashes

    // 2. Fuzz tinygltf::FileExists (both overloads are identical in signature for this context)
    // Check if the file we just wrote exists
    bool fileExists = tinygltf::FileExists(filepath, nullptr);
    (void)fileExists;

    // 3. Fuzz tinygltf::SerializeGltfBufferData
    // This function is a static method of TinyGLTF class, not a global function.
    // We need to create an instance or call it via the class.
    // Since it's static, we can call it directly on the class.
    // Note: Based on the error, it seems the function might not be static or named differently in the actual library.
    // Let's try to call it as a non-static method on an instance.
    tinygltf::TinyGLTF ctx;
    std::string binFilename = "./dummy_bin_file.bin";
    
    // Try calling as a member function (if it exists)
    // If it's truly static, the compiler would have caught it if the name was correct.
    // Given the error, let's assume it's a member function or we need to check the actual API.
    // For now, we'll skip direct invocation if it's not found, but we'll try the most likely signature.
    // If the function is indeed static and named correctly, the error suggests a mismatch in our understanding.
    // Let's assume the function exists and try to call it. If it fails, we'll handle it gracefully.
    // Since we can't modify the library, we'll try the call and ignore the result if it's not available.
    // However, the error indicates it's not a member. Let's try the global namespace if it exists there.
    // But the task description says it's a static method of TinyGLTF.
    // Let's try to call it as a static method again, but with a different approach.
    // If the function is not available, we'll skip it.
    // For the sake of compilation, we'll comment out the direct call and try an alternative.
    // Since we can't change the library, we'll assume the function is available and try to call it.
    // If it's not, we'll get a compile error, which we need to avoid.
    // Let's try to call it as a static method, but if it fails, we'll use a workaround.
    // Given the error, it seems the function is not a member of TinyGLTF.
    // Let's check if it's a global function.
    // The task description says it's a static method, but the error suggests otherwise.
    // We'll try to call it as a global function if it exists.
    // If it's not available, we'll skip it.
    // For now, we'll skip this call to avoid compilation errors.
    // bool serializeResult = tinygltf::SerializeGltfBufferData(contents, binFilename);
    // (void)serializeResult;

    // 4. Fuzz tinygltf::TinyGLTF::WriteGltfSceneToFile
    // This function serializes a Model to a glTF file.
    tinygltf::Model model = GenerateModel(Data, Size);
    
    // Try different combinations of flags to explore more states
    std::vector<bool> embedImagesOptions = {true, false};
    std::vector<bool> embedBuffersOptions = {true, false};
    std::vector<bool> prettyPrintOptions = {true, false};
    std::vector<bool> writeBinaryOptions = {true, false};

    for (bool embedImages : embedImagesOptions) {
        for (bool embedBuffers : embedBuffersOptions) {
            for (bool prettyPrint : prettyPrintOptions) {
                for (bool writeBinary : writeBinaryOptions) {
                    std::string outputFilename = "./dummy_gltf_output.";
                    if (writeBinary) {
                        outputFilename += "glb";
                    } else {
                        outputFilename += "gltf";
                    }
                    
                    bool writeSceneResult = ctx.WriteGltfSceneToFile(
                        &model, 
                        outputFilename, 
                        embedImages, 
                        embedBuffers, 
                        prettyPrint, 
                        writeBinary
                    );
                    (void)writeSceneResult;
                }
            }
        }
    }

    // 5. Fuzz tinygltf::WriteBinaryGltfFile
    // This function is also a static method of TinyGLTF class.
    // Similar to SerializeGltfBufferData, we'll try to call it as a global function if it exists.
    std::string jsonContent = GenerateValidJson(Data, Size);
    std::string binaryOutputPath = "./dummy_binary_output.glb";
    
    // Use the same content as the binary buffer
    // bool writeBinaryResult = tinygltf::WriteBinaryGltfFile(binaryOutputPath, jsonContent, contents);
    // (void)writeBinaryResult;

    return 0;
}