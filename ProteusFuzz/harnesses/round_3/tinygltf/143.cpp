// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Define function pointer types matching tinygltf expectations
typedef bool (*FileExistsFunction)(const std::string &, void *);
typedef std::string (*ExpandFilePathFunction)(const std::string &, void *);
typedef bool (*ReadWholeFileFunction)(std::vector<unsigned char> *, std::string *, const std::string &, void *);
typedef bool (*WriteWholeFileFunction)(std::string *, const std::string &, const std::vector<unsigned char> &, void *);
typedef bool (*GetFileSizeFunction)(size_t *, std::string *, const std::string &, void *);

static bool FuzzFileExists(const std::string &filename, void *) {
    return false;
}

static std::string FuzzExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

static bool FuzzReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                              const std::string &filepath, void *) {
    if (err) {
        *err = "FuzzReadWholeFile: Simulated failure";
    }
    return false;
}

static bool FuzzWriteWholeFile(std::string *err, const std::string &filepath,
                               const std::vector<unsigned char> &contents, void *) {
    std::ofstream ofs(filepath, std::ios::binary);
    if (!ofs) {
        if (err) *err = "FuzzWriteWholeFile: Cannot open file";
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    ofs.close();
    return true;
}

static bool FuzzGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *) {
    if (size) *size = 0;
    if (err) *err = "";
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Test SetFsCallbacks
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = FuzzFileExists;
    callbacks.ExpandFilePath = FuzzExpandFilePath;
    callbacks.ReadWholeFile = FuzzReadWholeFile;
    callbacks.WriteWholeFile = FuzzWriteWholeFile;
    callbacks.GetFileSizeInBytes = FuzzGetFileSizeInBytes;
    callbacks.user_data = nullptr;

    bool setCbResult = ctx.SetFsCallbacks(callbacks, &err);
    (void)setCbResult;

    // 2. Test SetSerializeDefaultValues and GetSerializeDefaultValues
    bool serializeDefaults = (Data[0] % 2) == 0;
    ctx.SetSerializeDefaultValues(serializeDefaults);
    bool currentDefaults = ctx.GetSerializeDefaultValues();
    (void)currentDefaults;

    // Prepare a simple model to write
    model.asset.version = "2.0";
    
    tinygltf::Node node;
    node.name = "FuzzNode";
    model.nodes.push_back(node);

    tinygltf::Scene scene;
    scene.name = "FuzzScene";
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);

    // Helper lambda to safely get boolean from input
    auto getBool = [&](size_t index) -> bool {
        if (index < Size) {
            return (Data[index] % 2) == 0;
        }
        return false;
    };

    // 3. Test WriteGltfSceneToFile
    bool embedImages = getBool(1);
    bool embedBuffers = getBool(2);
    bool prettyPrint = getBool(3);
    bool writeBinary = getBool(4);

    std::string filename = "./dummy_file";
    if (writeBinary) {
        filename += ".glb";
    } else {
        filename += ".gltf";
    }

    bool writeToFileResult = ctx.WriteGltfSceneToFile(
        &model, filename, embedImages, embedBuffers, prettyPrint, writeBinary
    );
    (void)writeToFileResult;

    // 4. Test WriteGltfSceneToStream (Overload 1)
    std::ofstream fileStream(filename + "_stream1", std::ios::binary);
    if (fileStream.is_open()) {
        bool streamResult1 = ctx.WriteGltfSceneToStream(
            &model, fileStream, prettyPrint, writeBinary
        );
        (void)streamResult1;
        fileStream.close();
    }

    // 5. Test WriteGltfSceneToStream (Overload 2)
    std::ofstream fileStream2(filename + "_stream2", std::ios::binary);
    if (fileStream2.is_open()) {
        bool streamResult2 = ctx.WriteGltfSceneToStream(
            &model, fileStream2, prettyPrint, writeBinary
        );
        (void)streamResult2;
        fileStream2.close();
    }

    // Cleanup
    std::remove(filename.c_str());
    std::remove((filename + "_stream1").c_str());
    std::remove((filename + "_stream2").c_str());

    return 0;
}