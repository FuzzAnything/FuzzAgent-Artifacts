// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
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
#include <string>
#include <fstream>
#include <vector>

static bool dummyFileExists(const std::string &abs_filename, void *) {
    // Always return true for fuzzing
    return true;
}

static std::string dummyExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char> *out,
                               std::string *err,
                               const std::string &filepath, void *) {
    // Read from the dummy file
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool dummyWriteWholeFile(std::string *err,
                                const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *) {
    // Not used for fuzzing, just return success
    return true;
}

static bool dummyGetFileSizeInBytes(size_t *size,
                                    std::string *err,
                                    const std::string &filename,
                                    void *) {
    // Return dummy size
    *size = 1024;
    return true;
}

static bool dummyURIEncode(const std::string &in,
                           const std::string &base_path,
                           std::string *out,
                           void *) {
    *out = in;
    return true;
}

static bool dummyURIDecode(const std::string &in,
                           std::string *out,
                           void *) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return 0;
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Test SetFsCallbacks
    tinygltf::FsCallbacks fsCallbacks;
    fsCallbacks.FileExists = dummyFileExists;
    fsCallbacks.ExpandFilePath = dummyExpandFilePath;
    fsCallbacks.ReadWholeFile = dummyReadWholeFile;
    fsCallbacks.WriteWholeFile = dummyWriteWholeFile;
    fsCallbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    fsCallbacks.user_data = nullptr;
    
    bool fsResult = loader.SetFsCallbacks(fsCallbacks, &err);
    (void)fsResult; // Ignore result for fuzzing
    
    // Test SetURICallbacks
    tinygltf::URICallbacks uriCallbacks;
    uriCallbacks.encode = dummyURIEncode;
    uriCallbacks.decode = dummyURIDecode;
    uriCallbacks.user_data = nullptr;
    
    bool uriResult = loader.SetURICallbacks(uriCallbacks, &err);
    (void)uriResult; // Ignore result for fuzzing
    
    // Test LoadASCIIFromFile
    tinygltf::Model model1;
    bool loadFileResult = loader.LoadASCIIFromFile(&model1, &err, &warn,
                                                   "./dummy_file");
    (void)loadFileResult; // Ignore result for fuzzing
    
    // Test LoadASCIIFromString
    tinygltf::Model model2;
    std::string gltfString(reinterpret_cast<const char*>(Data), Size);
    bool loadStringResult = loader.LoadASCIIFromString(&model2, &err, &warn,
                                                       gltfString.c_str(),
                                                       gltfString.size(),
                                                       "./");
    (void)loadStringResult; // Ignore result for fuzzing
    
    // Test FileExists
    bool exists = tinygltf::FileExists("./dummy_file", nullptr);
    (void)exists; // Ignore result for fuzzing
    
    // Test IsDataURI
    bool isDataURI = tinygltf::IsDataURI(gltfString);
    (void)isDataURI; // Ignore result for fuzzing
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}