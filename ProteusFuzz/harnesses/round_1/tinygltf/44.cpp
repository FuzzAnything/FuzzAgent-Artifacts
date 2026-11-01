// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Helper to create dummy file
  auto write_dummy_file = [&](const std::string &filename, const uint8_t *data, size_t len) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(data), len);
    return out.good();
  };

  // Helper to generate valid-ish JSON from input
  auto generate_json = [&](const uint8_t *data, size_t len) -> std::string {
    std::string json = R"({"asset":{"version":"2.0"}})";
    if (len > 0) {
      json += ",\"extras\":";
      json += "{";
      for (size_t i = 0; i < len && i < 100; ++i) {
        if (i > 0) json += ",";
        json += "\"" + std::to_string(i) + "\":";
        json += std::to_string(data[i]);
      }
      json += "}";
    }
    return json;
  };

  // 1. Test SetURICallbacks
  {
    tinygltf::URICallbacks callbacks;
    // Valid callbacks matching expected signatures
    callbacks.encode = [](const std::string &uri, const std::string &base_dir, std::string *out, void *user_data) {
      if (out) *out = uri;
      return true;
    };
    callbacks.decode = [](const std::string &uri, std::string *out, void *user_data) {
      if (out) *out = uri;
      return true;
    };
    callbacks.user_data = nullptr;
    ctx.SetURICallbacks(callbacks, &err);

    // Invalid callbacks (null)
    tinygltf::URICallbacks invalid_callbacks;
    invalid_callbacks.encode = nullptr;
    invalid_callbacks.decode = nullptr;
    invalid_callbacks.user_data = nullptr;
    ctx.SetURICallbacks(invalid_callbacks, &err);
  }

  // 2. Test SetFsCallbacks
  {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = [](const std::string &f, void *u) { return false; };
    callbacks.ExpandFilePath = [](const std::string &f, void *u) { return f; };
    callbacks.ReadWholeFile = [](std::vector<unsigned char> *out, std::string *err, const std::string &f, void *u) {
      out->clear();
      return false;
    };
    callbacks.WriteWholeFile = [](std::string *err, const std::string &f, const std::vector<unsigned char> &c, void *u) {
      return false;
    };
    callbacks.GetFileSizeInBytes = [](size_t *size, std::string *err, const std::string &f, void *u) {
      if (size) *size = 0;
      return false;
    };
    callbacks.user_data = nullptr;
    ctx.SetFsCallbacks(callbacks, &err);
  }

  // 3. Test RemoveImageLoader
  ctx.RemoveImageLoader();

  // 4. Test LoadASCIIFromString
  {
    std::string json_str = generate_json(Data, Size);
    ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.length(), ".", 0);
    // Reset model manually
    model = tinygltf::Model();
  }

  // 5. Test LoadASCIIFromFile
  {
    std::string filename = "./dummy_ascii.gltf";
    std::string json_str = generate_json(Data, Size);
    if (write_dummy_file(filename, reinterpret_cast<const uint8_t*>(json_str.c_str()), json_str.size())) {
      ctx.LoadASCIIFromFile(&model, &err, &warn, filename, 0);
      model = tinygltf::Model();
    }
  }

  // 6. Test LoadBinaryFromFile
  {
    std::string filename = "./dummy_binary.glb";
    // Create a minimal GLB header + JSON chunk + empty bin chunk
    std::vector<unsigned char> glb_data;
    // Header: magic, version, length
    glb_data.push_back(0x46); glb_data.push_back(0x54); glb_data.push_back(0x4C); glb_data.push_back(0x66); // "glTF"
    glb_data.push_back(0x02); glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); // version 2
    // Length placeholder (will update later)
    glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00);
    
    // JSON chunk header
    size_t json_len = 20; // minimal json length
    glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); // json length
    glb_data.push_back(0x4A); glb_data.push_back(0x53); glb_data.push_back(0x4F); glb_data.push_back(0x4E); // "JSON"
    
    // Minimal JSON
    std::string json = R"({"asset":{"version":"2.0"}})";
    json_len = json.length();
    // Pad to multiple of 4
    while (json_len % 4 != 0) json += ' ';
    
    // Update JSON length in header
    glb_data[12] = json_len & 0xFF;
    glb_data[13] = (json_len >> 8) & 0xFF;
    glb_data[14] = (json_len >> 16) & 0xFF;
    glb_data[15] = (json_len >> 24) & 0xFF;
    
    glb_data.insert(glb_data.end(), json.begin(), json.end());
    
    // Bin chunk header
    size_t bin_len = 0;
    glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); glb_data.push_back(0x00); // bin length
    glb_data.push_back(0x00); glb_data.push_back(0x4E); glb_data.push_back(0x49); glb_data.push_back(0x42); // "BIN\0"
    
    // Update total length
    size_t total_len = glb_data.size();
    glb_data[8] = total_len & 0xFF;
    glb_data[9] = (total_len >> 8) & 0xFF;
    glb_data[10] = (total_len >> 16) & 0xFF;
    glb_data[11] = (total_len >> 24) & 0xFF;
    
    if (write_dummy_file(filename, glb_data.data(), glb_data.size())) {
      ctx.LoadBinaryFromFile(&model, &err, &warn, filename, 0);
      model = tinygltf::Model();
    }
  }

  return 0;
}