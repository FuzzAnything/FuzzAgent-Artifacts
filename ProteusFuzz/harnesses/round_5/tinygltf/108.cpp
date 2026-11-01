// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
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

// Fuzz driver for tinygltf parsing functions
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Create a temporary file to hold the input data
    const char* dummy_file_path = "./dummy_file";
    std::ofstream dummy_file(dummy_file_path);
    if (!dummy_file.is_open()) {
        return 0;
    }
    dummy_file << input;
    dummy_file.close();

    // Initialize TinyGLTF context
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(true);
    
    std::string err;
    std::string warn;

    // Attempt to load the file to trigger internal parsing logic
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file_path);

    // Since we cannot directly include nlohmann/json.hpp, we rely on the fact that
    // tinygltf.h might expose the internal json type or we can use the Model's internal state.
    // However, the target functions require a `const detail::json &o`.
    // In tinygltf, `detail::json` is an alias for `nlohmann::json`.
    // We can try to access the internal JSON if the library exposes it, or we can try to
    // construct a minimal JSON object using the library's own utilities if available.
    // Since we can't easily construct it without the header, we will use a workaround:
    // We will try to parse the input string into a JSON object using the library's internal parser
    // if we can access it, but since we can't easily, we will rely on the fact that we can't 
    // easily construct `detail::json` without the header.
    
    // Instead, let's assume the environment has nlohmann/json.hpp available as it's a dependency.
    // If not, we might need to rely on the library's internal types.
    // Let's try to include nlohmann/json.hpp directly.
    
    #ifdef TINYGLTF_HAS_NLOHMANN_JSON
    #include <nlohmann/json.hpp>
    #define HAS_NLOHMANN_JSON 1
    #endif

    #if defined(HAS_NLOHMANN_JSON)
    nlohmann::json json_obj;
    try {
        json_obj = nlohmann::json::parse(input);
    } catch (const std::exception& e) {
        // If parsing fails, use an empty object
        json_obj = nlohmann::json::object();
    }

    // Test ParseAnimationChannel
    {
        tinygltf::AnimationChannel channel;
        std::string channel_err;
        tinygltf::ParseAnimationChannel(&channel, &channel_err, json_obj, true);
        (void)channel_err;
    }
    
    // Test ParseOcclusionTextureInfo
    {
        tinygltf::OcclusionTextureInfo texinfo;
        std::string texinfo_err;
        tinygltf::ParseOcclusionTextureInfo(&texinfo, &texinfo_err, json_obj, true);
        (void)texinfo_err;
    }
    
    // Test ParseJSONProperty
    {
        std::map<std::string, double> ret;
        std::string prop_err;
        tinygltf::ParseJSONProperty(&ret, &prop_err, json_obj, "test_property", false);
        (void)prop_err;
    }
    
    // Test ParseExtensionsProperty
    {
        tinygltf::ExtensionMap ext_map;
        std::string ext_err;
        tinygltf::ParseExtensionsProperty(&ext_map, &ext_err, json_obj);
        (void)ext_err;
    }
    
    // Test ParseIntegerProperty
    {
        int int_ret;
        std::string int_err;
        tinygltf::ParseIntegerProperty(&int_ret, &int_err, json_obj, "test_int", false);
        (void)int_err;
    }
    
    // Test ParseMaterial (Permissive)
    {
        tinygltf::Material material;
        std::string mat_err;
        std::string mat_warn;
        tinygltf::ParseMaterial(&material, &mat_err, &mat_warn, json_obj, true, tinygltf::Permissive);
        (void)mat_err;
        (void)mat_warn;
    }
    
    // Test ParseMaterial (Strict)
    {
        tinygltf::Material material;
        std::string mat_err;
        std::string mat_warn;
        tinygltf::ParseMaterial(&material, &mat_err, &mat_warn, json_obj, true, tinygltf::Strict);
        (void)mat_err;
        (void)mat_warn;
    }
    #endif

    // Clean up
    std::remove(dummy_file_path);
    
    return 0;
}