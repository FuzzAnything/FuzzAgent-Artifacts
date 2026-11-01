// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
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

// Helper to safely parse a string from fuzzing data
static std::string ParseString(const uint8_t *data, size_t size, size_t &offset) {
    if (offset >= size) return "";
    size_t len = 0;
    // Read length (4 bytes)
    if (offset + 4 > size) return "";
    len = data[offset] | (data[offset+1] << 8) | (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + len > size) return "";
    std::string s(reinterpret_cast<const char*>(&data[offset]), len);
    offset += len;
    return s;
}

// Helper to parse a vector of doubles
static std::vector<double> ParseDoubles(const uint8_t *data, size_t size, size_t &offset) {
    std::vector<double> result;
    if (offset >= size) return result;
    size_t count = 0;
    if (offset + 4 > size) return result;
    count = data[offset] | (data[offset+1] << 8) | (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (count > 100) count = 100; // Limit to prevent huge allocations
    
    for (size_t i = 0; i < count; ++i) {
        if (offset + 8 > size) break;
        double val;
        std::memcpy(&val, &data[offset], sizeof(double));
        result.push_back(val);
        offset += 8;
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string filename = "./dummy_file";

    // Use the first byte to decide configuration
    bool setImagesAsIs = (Data[0] % 2) != 0;
    bool setStoreOriginalJSON = ((Data[0] >> 1) & 1) != 0;

    // Apply configurations
    loader.SetImagesAsIs(setImagesAsIs);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(setStoreOriginalJSON);

    // Build a dummy model using fuzzing data
    size_t offset = 1;
    
    // Add some nodes
    tinygltf::Node node;
    node.name = ParseString(Data, Size, offset);
    model.nodes.push_back(node);

    // Add some meshes
    tinygltf::Mesh mesh;
    mesh.name = ParseString(Data, Size, offset);
    tinygltf::Primitive prim;
    if (offset < Size) {
        prim.material = static_cast<int>(Data[offset] % 10);
        offset += 1;
    }
    mesh.primitives.push_back(prim);
    model.meshes.push_back(mesh);

    // Add some materials
    tinygltf::Material mat;
    mat.name = ParseString(Data, Size, offset);
    mat.emissiveFactor = ParseDoubles(Data, Size, offset);
    model.materials.push_back(mat);

    // Add some buffers
    tinygltf::Buffer buf;
    buf.name = ParseString(Data, Size, offset);
    // Fill buffer with remaining data or a subset
    if (offset < Size) {
        buf.data.assign(&Data[offset], &Data[Size]);
    }
    model.buffers.push_back(buf);

    // Add some images
    tinygltf::Image img;
    img.name = ParseString(Data, Size, offset);
    if (offset < Size) {
        img.image.assign(&Data[offset], &Data[Size]);
    }
    img.as_is = setImagesAsIs;
    model.images.push_back(img);

    // Add asset
    model.asset.version = "2.0";

    // Call the target API functions to exercise logic
    
    // 1. GetImagesAsIs
    bool imagesAsIs = loader.GetImagesAsIs();
    (void)imagesAsIs;

    // 2. GetStoreOriginalJSONForExtrasAndExtensions
    bool storeOriginalJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)storeOriginalJSON;

    // 4. GetPreserveImageChannels
    bool preserveImageChannels = loader.GetPreserveImageChannels();
    (void)preserveImageChannels;

    // 3. WriteGltfSceneToFile
    bool embedImages = (Data[0] % 3) != 0;
    bool embedBuffers = ((Data[0] >> 3) & 1) != 0;
    bool prettyPrint = ((Data[0] >> 4) & 1) != 0;
    bool writeBinary = ((Data[0] >> 5) & 1) != 0;

    // Write to file
    bool writeSuccess = loader.WriteGltfSceneToFile(
        &model, 
        filename, 
        embedImages, 
        embedBuffers, 
        prettyPrint, 
        writeBinary
    );
    
    // Try to load the file back to exercise the loader with the written data
    // Using LoadASCIIFromString or LoadBinary if LoadFile is not available in this version
    // Since LoadFile is reported missing, we will try to load the file content manually if possible
    // or just skip the loading part to avoid compilation error.
    // However, to be safe, we will just skip the loading part as the write path is covered.
    
    return 0;
}