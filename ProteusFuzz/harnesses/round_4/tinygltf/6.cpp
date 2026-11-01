// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

static std::string GenerateGltfJson(const uint8_t *Data, size_t Size) {
    std::stringstream ss;
    ss << R"({
  "asset": {
    "version": "2.0"
  },
  "buffers": [
    {
      "uri": "data:application/octet-stream;base64,"
    }
  ],
  "bufferViews": [
    {
      "buffer": 0,
      "byteLength": 0
    }
  ],
  "accessors": [
    {
      "bufferView": 0,
      "componentType": 5126,
      "count": 0,
      "type": "SCALAR"
    }
  ],
  "meshes": [],
  "nodes": [],
  "scenes": [],
  "scene": 0
})";

    // Append raw data as base64-like string to buffer URI to test parsing
    // We just append the raw bytes as hex to make it a valid string for the fuzzer
    // In a real scenario, proper base64 encoding would be better, but for fuzzing
    // raw hex or binary injection is often sufficient to trigger edge cases.
    for (size_t i = 0; i < Size; ++i) {
        ss << std::hex << (int)Data[i];
    }
    return ss.str();
}

static std::string GenerateGlb(const uint8_t *Data, size_t Size) {
    // Minimal glb header: Magic (0x46546C67), Version (2), Length
    // Chunk 1: JSON (Type 0x4E4F534A), Length, Data
    // Chunk 2: Binary (Type 0x004E4942), Length, Data (optional)
    
    std::vector<unsigned char> glb;
    
    // Header
    glb.push_back(0x46); glb.push_back(0x46); glb.push_back(0x4C); glb.push_back(0x67); // Magic
    glb.push_back(0x02); glb.push_back(0x00); glb.push_back(0x00); glb.push_back(0x00); // Version
    
    // Total length placeholder (will fill later)
    size_t total_len = 12; // Header
    
    // JSON Chunk
    std::string json_str = R"({"asset":{"version":"2.0"}})";
    // Pad to 4-byte alignment
    while (json_str.size() % 4 != 0) json_str += '\0';
    
    size_t json_len = json_str.size();
    total_len += 8 + json_len; // Chunk header + data
    
    // Binary Chunk (optional, using fuzzer data)
    size_t bin_len = Size;
    if (bin_len > 0) {
        // Align binary data to 4 bytes
        while (bin_len % 4 != 0) bin_len++;
        total_len += 8 + bin_len;
    }

    // Write header
    glb.resize(12);
    uint32_t len = static_cast<uint32_t>(total_len);
    glb[8] = len & 0xFF; glb[9] = (len >> 8) & 0xFF;
    glb[10] = (len >> 16) & 0xFF; glb[11] = (len >> 24) & 0xFF;

    // Write JSON Chunk Header
    glb.push_back(0x4E); glb.push_back(0x4F); glb.push_back(0x53); glb.push_back(0x4A); // Type
    uint32_t jlen = static_cast<uint32_t>(json_len);
    glb.push_back(jlen & 0xFF); glb.push_back((jlen >> 8) & 0xFF);
    glb.push_back((jlen >> 16) & 0xFF); glb.push_back((jlen >> 24) & 0xFF);
    
    // Write JSON Data
    glb.insert(glb.end(), json_str.begin(), json_str.end());

    // Write Binary Chunk if data exists
    if (Size > 0) {
        glb.push_back(0x00); glb.push_back(0x4E); glb.push_back(0x49); glb.push_back(0x42); // Type
        uint32_t blen = static_cast<uint32_t>(bin_len);
        glb.push_back(blen & 0xFF); glb.push_back((blen >> 8) & 0xFF);
        glb.push_back((blen >> 16) & 0xFF); glb.push_back((blen >> 24) & 0xFF);
        
        // Pad data
        std::vector<unsigned char> padded_data(Size, 0);
        for(size_t i=0; i<Size; ++i) padded_data[i] = Data[i];
        glb.insert(glb.end(), padded_data.begin(), padded_data.end());
    }

    return std::string(glb.begin(), glb.end());
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. SetImagesAsIs
    loader.SetImagesAsIs(true);

    // 2. LoadASCIIFromFile
    {
        std::string ascii_content = GenerateGltfJson(Data, Size);
        std::ofstream ascii_file("./dummy_file.gltf");
        ascii_file << ascii_content;
        ascii_file.close();

        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf");
        // Clean up
        std::remove("./dummy_file.gltf");
    }

    // 3. WriteGltfSceneToFile (ASCII)
    {
        std::ofstream ascii_out("./dummy_file_out.gltf");
        ascii_out.close();
        loader.WriteGltfSceneToFile(&model, "./dummy_file_out.gltf", false, false, false, false);
        std::remove("./dummy_file_out.gltf");
    }

    // 4. WriteGltfSceneToFile (Binary)
    {
        std::ofstream bin_out("./dummy_file_out.glb");
        bin_out.close();
        loader.WriteGltfSceneToFile(&model, "./dummy_file_out.glb", false, false, false, true);
        std::remove("./dummy_file_out.glb");
    }

    // 5. SetImagesAsIs
    loader.SetImagesAsIs(false);

    // 6. LoadASCIIFromFile (with different flag)
    {
        std::string ascii_content = GenerateGltfJson(Data, Size);
        std::ofstream ascii_file("./dummy_file.gltf");
        ascii_file << ascii_content;
        ascii_file.close();

        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf");
        std::remove("./dummy_file.gltf");
    }

    // 7. WriteGltfSceneToFile (Embed images/buffers)
    {
        std::ofstream ascii_out("./dummy_file_out.gltf");
        ascii_out.close();
        loader.WriteGltfSceneToFile(&model, "./dummy_file_out.gltf", true, true, true, false);
        std::remove("./dummy_file_out.gltf");
    }

    // 8. SetImagesAsIs
    loader.SetImagesAsIs(true);

    // 9. LoadBinaryFromFile
    {
        std::string glb_content = GenerateGlb(Data, Size);
        std::ofstream glb_file("./dummy_file.glb", std::ios::binary);
        glb_file.write(glb_content.c_str(), glb_content.size());
        glb_file.close();

        loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb");
        std::remove("./dummy_file.glb");
    }

    return 0;
}