// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
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

// Helper to safely cast fuzzer input to floating point
static double GetDouble(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset + sizeof(double) > Size) {
    return 0.0;
  }
  double val;
  std::memcpy(&val, Data + offset, sizeof(double));
  offset += sizeof(double);
  return val;
}

// Helper to safely cast fuzzer input to integer
static int GetInt(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset + sizeof(int) > Size) {
    return 0;
  }
  int val;
  std::memcpy(&val, Data + offset, sizeof(int));
  offset += sizeof(int);
  return val;
}

// Helper to safely cast fuzzer input to string
static std::string GetString(const uint8_t *Data, size_t Size, size_t &offset) {
  if (offset >= Size) {
    return "";
  }
  // Read length as uint32
  if (offset + sizeof(uint32_t) > Size) {
    return "";
  }
  uint32_t uLen;
  std::memcpy(&uLen, Data + offset, sizeof(uint32_t));
  offset += sizeof(uint32_t);
  
  if (uLen > 1024 || offset + uLen > Size) {
    return "";
  }
  std::string str(reinterpret_cast<const char*>(Data + offset), uLen);
  offset += uLen;
  return str;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  try {
    size_t offset = 0;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // Create a random asset
    model.asset.version = GetString(Data, Size, offset);
    model.asset.generator = GetString(Data, Size, offset);
    model.asset.copyright = GetString(Data, Size, offset);

    // Create a random sampler
    tinygltf::Sampler sampler;
    sampler.name = GetString(Data, Size, offset);
    sampler.minFilter = GetInt(Data, Size, offset);
    model.samplers.push_back(sampler);

    // Create a random spot light
    tinygltf::Light light;
    light.type = GetString(Data, Size, offset);
    light.name = GetString(Data, Size, offset);
    light.intensity = GetDouble(Data, Size, offset);
    light.color = {GetDouble(Data, Size, offset), GetDouble(Data, Size, offset), GetDouble(Data, Size, offset)};
    light.spot.innerConeAngle = GetDouble(Data, Size, offset);
    light.spot.outerConeAngle = GetDouble(Data, Size, offset);
    model.lights.push_back(light);

    // Create a random mesh
    tinygltf::Mesh mesh;
    mesh.name = GetString(Data, Size, offset);
    
    // Create a primitive
    tinygltf::Primitive prim;
    prim.material = GetInt(Data, Size, offset);
    prim.mode = GetInt(Data, Size, offset);
    prim.attributes["POSITION"] = GetInt(Data, Size, offset);
    prim.attributes["NORMAL"] = GetInt(Data, Size, offset);
    
    // Add targets
    std::map<std::string, int> target;
    target["POSITION"] = GetInt(Data, Size, offset);
    prim.targets.push_back(target);
    
    mesh.primitives.push_back(prim);
    mesh.weights.push_back(GetDouble(Data, Size, offset));
    model.meshes.push_back(mesh);

    // Create a random node
    tinygltf::Node node;
    node.name = GetString(Data, Size, offset);
    node.translation = {GetDouble(Data, Size, offset), GetDouble(Data, Size, offset), GetDouble(Data, Size, offset)};
    node.rotation = {GetDouble(Data, Size, offset), GetDouble(Data, Size, offset), GetDouble(Data, Size, offset), GetDouble(Data, Size, offset)};
    node.scale = {GetDouble(Data, Size, offset), GetDouble(Data, Size, offset), GetDouble(Data, Size, offset)};
    node.mesh = GetInt(Data, Size, offset);
    node.skin = GetInt(Data, Size, offset);
    node.camera = GetInt(Data, Size, offset);
    model.nodes.push_back(node);

    // Serialize the model to JSON
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, false, false);

  } catch (const std::exception &) {
    // Ignore exceptions during fuzzing
    return 0;
  } catch (...) {
    // Catch any other unexpected exceptions
    return 0;
  }

  return 0;
}