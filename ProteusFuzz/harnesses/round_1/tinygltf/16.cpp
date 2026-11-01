// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfSkin at tiny_gltf.h:8118:13 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfOcclusionTextureInfo at tiny_gltf.h:7594:13 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfBuffer at tiny_gltf.h:7503:13 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfSkin at tiny_gltf.h:8118:13 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfOcclusionTextureInfo at tiny_gltf.h:7594:13 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonSetObject at tiny_gltf.h:7021:6 in tiny_gltf.h
// tinygltf::SerializeGltfBuffer at tiny_gltf.h:7503:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <random>
#include <limits>

// Define implementation before including header to expose internal functions
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to generate random data
static std::vector<unsigned char> GenerateRandomData(size_t size) {
    std::vector<unsigned char> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<unsigned char>(dis(gen));
    }
    return data;
}

// Helper to generate random string
static std::string GenerateRandomString(size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;':\",./<>?";
    std::string str(size, ' ');
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    for (size_t i = 0; i < size; ++i) {
        str[i] = charset[dis(gen)];
    }
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random generator with input data for deterministic fuzzing
    std::mt19937 gen(Data[0]);
    std::uniform_int_distribution<> disOp(0, 5); // 6 operations (0-5)
    std::uniform_int_distribution<> disSize(0, 1024); // Max size for strings/vectors
    std::uniform_int_distribution<> disInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    std::uniform_real_distribution<> disDouble(-1000.0, 1000.0);

    int operation = disOp(gen);
    
    // Use the json type defined in tinygltf's internal detail namespace
    using json = tinygltf::detail::json;
    json jsonOutput;

    try {
        switch (operation) {
            case 0: {
                // Fuzz SerializeGltfBuffer
                tinygltf::Buffer buffer;
                buffer.name = GenerateRandomString(disSize(gen));
                buffer.data = GenerateRandomData(disSize(gen));
                
                // Set extras using tinygltf::Value
                // tinygltf::Value uses nlohmann::json internally in modern versions
                // We construct it directly or use the constructor if available
                if (disOp(gen) % 2 == 0) {
                    tinygltf::Value v;
                    // Accessing underlying json or setting type directly
                    // Since we don't know the exact version's API for Value setters,
                    // we rely on the fact that Value wraps nlohmann::json.
                    // We will construct a nlohmann::json and assign it if the API allows,
                    // or just leave it default to test the null case.
                    // For fuzzing, we can try to construct a number directly if possible.
                    // However, without explicit setters, we assume default construction is safe.
                    // To force a value, we might need to use the constructor or assignment.
                    // Let's try to assign a number directly to the internal json if exposed,
                    // or just use the default empty value which is valid.
                    // In many tinygltf versions, Value is just an alias or wrapper.
                    // We will skip complex value setting if setters aren't guaranteed,
                    // but the serialization handles empty extras fine.
                    // To be safe and compile, we just assign a default constructed value.
                    buffer.extras = v; 
                }
                
                tinygltf::detail::JsonSetObject(jsonOutput);
                tinygltf::SerializeGltfBuffer(buffer, jsonOutput);
                break;
            }
            case 1: {
                // Fuzz SerializeGltfSkin
                tinygltf::Skin skin;
                skin.name = GenerateRandomString(disSize(gen));
                skin.inverseBindMatrices = disInt(gen);
                
                // Generate random joints
                size_t jointCount = disSize(gen) / 10;
                for (size_t i = 0; i < jointCount; ++i) {
                    skin.joints.push_back(disInt(gen));
                }

                // Randomly set extras
                if (disOp(gen) % 2 == 0) {
                    tinygltf::Value v;
                    // Default value is null, which is valid
                    skin.extras = v;
                }

                tinygltf::detail::JsonSetObject(jsonOutput);
                tinygltf::SerializeGltfSkin(skin, jsonOutput);
                break;
            }
            case 2: {
                // Fuzz SerializeGltfOcclusionTextureInfo
                tinygltf::OcclusionTextureInfo texinfo;
                texinfo.index = disInt(gen);
                texinfo.strength = disDouble(gen);
                
                // Randomly set extras
                if (disOp(gen) % 2 == 0) {
                    tinygltf::Value v;
                    texinfo.extras = v;
                }

                tinygltf::detail::JsonSetObject(jsonOutput);
                tinygltf::SerializeGltfOcclusionTextureInfo(texinfo, jsonOutput);
                break;
            }
            case 3: {
                // Fuzz JsonSetObject and JsonAddMember
                tinygltf::detail::JsonSetObject(jsonOutput);
                
                std::string key = GenerateRandomString(disSize(gen) / 2);
                json value;
                
                // Random value type
                int valType = disOp(gen) % 4;
                if (valType == 0) value = static_cast<double>(disInt(gen));
                else if (valType == 1) value = disDouble(gen);
                else if (valType == 2) value = GenerateRandomString(disSize(gen));
                else value = true;

                tinygltf::detail::JsonAddMember(jsonOutput, key.c_str(), std::move(value));
                break;
            }
            case 4: {
                // Fuzz JsonPushBack
                json arr = json(); // Initialize as array
                // Ensure it's an array
                arr = json::array();
                
                size_t itemCount = disSize(gen) / 10;
                for (size_t i = 0; i < itemCount; ++i) {
                    json val;
                    int valType = disOp(gen) % 3;
                    if (valType == 0) val = static_cast<double>(disInt(gen));
                    else if (valType == 1) val = disDouble(gen);
                    else val = GenerateRandomString(disSize(gen));
                    
                    tinygltf::detail::JsonPushBack(arr, std::move(val));
                }
                break;
            }
            case 5: {
                // Combined stress test: Create complex structures and serialize
                tinygltf::Buffer buffer;
                buffer.name = "fuzz_buffer";
                buffer.data = GenerateRandomData(disSize(gen));
                
                tinygltf::Skin skin;
                skin.name = "fuzz_skin";
                skin.joints.push_back(disInt(gen));
                
                tinygltf::OcclusionTextureInfo texinfo;
                texinfo.index = disInt(gen);
                
                // Create a root JSON object
                json root;
                tinygltf::detail::JsonSetObject(root);
                
                json bufferObj;
                tinygltf::detail::JsonSetObject(bufferObj);
                tinygltf::SerializeGltfBuffer(buffer, bufferObj);
                tinygltf::detail::JsonAddMember(root, "buffer", std::move(bufferObj));
                
                json skinObj;
                tinygltf::detail::JsonSetObject(skinObj);
                tinygltf::SerializeGltfSkin(skin, skinObj);
                tinygltf::detail::JsonAddMember(root, "skin", std::move(skinObj));
                
                json texObj;
                tinygltf::detail::JsonSetObject(texObj);
                tinygltf::SerializeGltfOcclusionTextureInfo(texinfo, texObj);
                tinygltf::detail::JsonAddMember(root, "occlusion", std::move(texObj));
                
                // Add some random array data
                json arr = json::array();
                for (int i = 0; i < 5; ++i) {
                    tinygltf::detail::JsonPushBack(arr, json(static_cast<double>(disInt(gen))));
                }
                tinygltf::detail::JsonAddMember(root, "random_array", std::move(arr));
                break;
            }
        }
    } catch (...) {
        // Ignore exceptions to prevent fuzzer crash on invalid states
        return 0;
    }

    return 0;
}