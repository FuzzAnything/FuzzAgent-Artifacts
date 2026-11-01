// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

// Helper to convert fuzz input to a string
static std::string ToJsonString(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input_str = ToJsonString(Data, Size);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // Use the public API LoadASCIIFromString which internally calls the parsing logic
    // that exercises ParseTexture, ParseMaterial, ParseIntegerProperty, etc.
    // Signature: bool LoadASCIIFromString(Model* model, std::string* err, std::string* warn,
    //                                    const char* str, unsigned int length,
    //                                    const std::string& base_dir, unsigned int check_sections);
    bool success = loader.LoadASCIIFromString(&model, &err, &warn, 
                                              input_str.c_str(), 
                                              static_cast<unsigned int>(input_str.length()), 
                                              "", 
                                              0);
    
    // To further explore states, write to a dummy file and try loading from file.
    const std::string filename = "./dummy_file";
    std::ofstream out_file(filename);
    out_file.write(input_str.c_str(), input_str.size());
    out_file.close();
    
    // Reset model for file load
    model = tinygltf::Model();
    
    // Use LoadASCIIFromFile which is the public counterpart to LoadFile (which is private)
    // Signature: bool LoadASCIIFromFile(Model* model, std::string* err, std::string* warn,
    //                                   const std::string& filename, const std::string& base_dir,
    //                                   unsigned int check_sections);
    // Note: The signature in the header provided shows 5 arguments max (filename, base_dir, check_sections)
    // The previous error indicated 6 arguments were passed. Correcting to match the header:
    // bool LoadASCIIFromFile(Model *model, std::string *err, std::string *warn,
    //                        const std::string &filename,
    //                        unsigned int check_sections = REQUIRE_VERSION);
    // Wait, looking at the error message again:
    // /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/sources/tinygltf/tiny_gltf.h:1411:8: note: 'LoadASCIIFromFile' declared here
    //   1411 |   bool LoadASCIIFromFile(Model *model, std::string *err, std::string *warn,
    //   1412 |                          const std::string &filename,
    //   1413 |                          unsigned int check_sections = REQUIRE_VERSION);
    // It seems the function signature in the specific version of tiny_gltf.h used here does NOT have a base_dir parameter.
    // Let's adjust the call to match the observed signature: (model, err, warn, filename, check_sections)
    bool file_success = loader.LoadASCIIFromFile(&model, &err, &warn, filename, 0);
    
    // Clean up
    std::remove(filename.c_str());
    
    return 0;
}