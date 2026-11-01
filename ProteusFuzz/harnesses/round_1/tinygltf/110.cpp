// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <stdexcept>

#include "tiny_gltf.h"

// Helper function to extract a valid UTF-8 string from fuzzing data
static std::string ExtractString(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset >= Size) return "";
  size_t len = 0;
  // Limit length to avoid huge allocations
  size_t max_len = 10000; 
  while (offset + len < Size && Data[offset + len] != 0 && len < max_len) {
    len++;
  }
  
  // Basic validation: ensure we have a valid UTF-8 sequence or just ASCII
  // For fuzzing, we can try to sanitize or just return empty if invalid
  // A simple approach: only accept ASCII printable or known safe chars to avoid UTF-8 errors in JSON
  std::string str;
  str.reserve(len);
  for (size_t i = 0; i < len; ++i) {
    uint8_t c = Data[offset + i];
    // Allow ASCII printable and common control chars, or valid UTF-8 start bytes
    if (c < 128) {
        // Basic ASCII check
        if (c < 32 && c != 9 && c != 10 && c != 13) continue; // Skip control chars except tab, newline, CR
        str.push_back(c);
    } else {
        // Simple UTF-8 validation: if high bit set, expect valid sequence
        // For robustness in fuzzing, if we can't validate easily, skip or replace
        // Here we just skip non-ASCII to prevent JSON parser crashes on invalid UTF-8
        // Alternatively, we could replace with '?'
        // str.push_back('?'); 
        // But let's just skip to keep it simple and safe
        continue;
    }
  }
  
  offset += len + 1; // skip null terminator if present
  return str;
}

// Helper function to extract a byte vector from fuzzing data
static std::vector<unsigned char> ExtractVector(const uint8_t *Data, size_t Size, size_t &offset) {
  std::vector<unsigned char> vec;
  if (offset >= Size) return vec;
  size_t len = 0;
  // Read length as a simple integer (first byte)
  len = Data[offset];
  offset++;
  if (offset + len > Size) {
    len = Size - offset;
  }
  vec.assign(Data + offset, Data + offset + len);
  offset += len;
  return vec;
}

// Safe helper to get a boolean from data
static bool GetBool(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset >= Size) return false;
  bool val = (Data[offset] % 2) != 0;
  offset++;
  return val;
}

// Helper to safely get an integer
static int GetInt(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset >= Size) return 0;
  int val = static_cast<int>(Data[offset]);
  offset++;
  return val;
}

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string filename = "./dummy_file";

  // Parse data to determine which API to test
  size_t offset = 0;
  int op = 0;
  if (offset < Size) {
    op = Data[offset];
    offset++;
  }

  // Ensure we don't go out of bounds for subsequent reads
  if (offset >= Size) return 0;

  try {
    switch (op % 5) {
      case 0: {
        // Test SetSerializeDefaultValues and GetSerializeDefaultValues
        bool enabled = GetBool(Data, Size, offset);
        ctx.SetSerializeDefaultValues(enabled);
        bool current = ctx.GetSerializeDefaultValues();
        (void)current; // Use to avoid unused warning
        break;
      }
      case 1: {
        // Test WriteGltfFile (static function)
        // Note: This function is often internal, but if exposed in the header we try it.
        // Based on the error, WriteGltfFile is not in the global namespace or tinygltf namespace directly accessible.
        // We will skip direct fuzzing of this static internal function as it requires internal JSON types not exposed.
        // Instead, we rely on the higher level functions.
        break;
      }
      case 2: {
        // Test WriteGltfSceneToFile
        // Populate model with some fuzzed data to make it interesting
        model.asset.version = ExtractString(Data, Size, offset);
        
        // Add a dummy buffer
        tinygltf::Buffer buf;
        buf.data = ExtractVector(Data, Size, offset);
        model.buffers.push_back(buf);

        // Add a dummy buffer view
        tinygltf::BufferView bv;
        bv.buffer = 0;
        bv.byteOffset = 0;
        bv.byteLength = static_cast<size_t>(buf.data.size());
        model.bufferViews.push_back(bv);

        // Add a dummy accessor
        tinygltf::Accessor acc;
        acc.bufferView = 0;
        acc.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        acc.count = 3;
        // Accessor::Type is an enum class or int. Using SCALAR (0).
        acc.type = 0; 
        model.accessors.push_back(acc);

        // Add a dummy mesh
        tinygltf::Mesh mesh;
        mesh.primitives.emplace_back();
        mesh.primitives[0].attributes["POSITION"] = 0;
        model.meshes.push_back(mesh);

        // Add a dummy node
        tinygltf::Node node;
        node.mesh = 0;
        model.nodes.push_back(node);

        // Add a dummy scene
        tinygltf::Scene scene;
        scene.nodes.push_back(0);
        model.scenes.push_back(scene);
        model.defaultScene = 0;

        bool embedImages = GetBool(Data, Size, offset);
        bool embedBuffers = GetBool(Data, Size, offset);
        bool prettyPrint = GetBool(Data, Size, offset);
        bool writeBinary = GetBool(Data, Size, offset);

        ctx.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
        break;
      }
      case 3: {
        // Test WriteGltfSceneToStream
        // Reuse model population logic similar to case 2 but simpler
        model.asset.version = ExtractString(Data, Size, offset);
        
        bool prettyPrint = GetBool(Data, Size, offset);
        bool writeBinary = GetBool(Data, Size, offset);

        std::ostringstream stream;
        ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
        break;
      }
      case 4: {
        // Combined test: SetSerializeDefaultValues then WriteGltfSceneToFile
        bool enabled = GetBool(Data, Size, offset);
        ctx.SetSerializeDefaultValues(enabled);
        
        // Create a minimal valid model
        model.asset.version = "2.0";
        
        bool embedImages = GetBool(Data, Size, offset);
        bool embedBuffers = GetBool(Data, Size, offset);
        bool prettyPrint = GetBool(Data, Size, offset);
        bool writeBinary = GetBool(Data, Size, offset);

        ctx.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
        break;
      }
    }
  } catch (...) {
    // Catch any exceptions to prevent fuzzer crash
    return 0;
  }

  return 0;
}