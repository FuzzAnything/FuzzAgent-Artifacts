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
#include "tiny_gltf.h"

// Fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to string for parsing
    std::string input(reinterpret_cast<const char*>(Data), Size);

    // Initialize tinygltf structures
    tinygltf::Scene scene;
    tinygltf::Light light;
    tinygltf::Node node;
    tinygltf::NormalTextureInfo texinfo;
    tinygltf::OrthographicCamera camera;
    
    std::string err;
    std::string warn;
    bool store_original_json_for_extras_and_extensions = false;

    // tinygltf uses internal JSON types. We need to access them via the library's API.
    // The detail namespace and json type are internal, so we must use the library's parsing functions.
    // We'll create a Model and parse the input to get access to the internal JSON structure.
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json_for_extras_and_extensions);
    
    std::string dummy_path = "./dummy_file";
    
    // Write input to dummy file
    FILE* f = fopen(dummy_path.c_str(), "wb");
    if (f) {
        fwrite(input.data(), 1, input.size(), f);
        fclose(f);
        
        // Try loading as glTF
        if (ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_path.c_str(), -1)) {
            // If successful, we have a model, but we need the raw JSON for the APIs
            // We'll try to access the internal JSON via the model's extensions or extras
            // However, the target APIs require detail::json, which is internal.
            // We'll need to use the library's internal parsing functions if possible.
        }
        
        // Clean up
        remove(dummy_path.c_str());
    }

    // Since we cannot easily construct tinygltf::detail::json without backend knowledge,
    // we will try to use the library's internal parsing functions if available.
    // For now, we'll test the APIs with a null/empty approach if possible.
    
    // We'll use a try-catch block to handle any potential exceptions
    try {
        // Access the internal JSON type via the library's internal namespace
        // This requires knowing the backend (nlohmann vs rapidjson)
        // For now, we'll assume the backend is nlohmann and try to construct a null JSON object
        
        // If the backend is nlohmann, we can use nlohmann::json
        #ifdef TINYGLTF_USE_NLOHMANN_JSON
            #include <nlohmann/json.hpp>
            using json_type = nlohmann::json;
            json_type null_json = nullptr;
            
            // 1. Test JsonIsNull
            bool isNull = tinygltf::detail::JsonIsNull(null_json);
            (void)isNull;

            // 2. Test ParseScene
            tinygltf::detail::ParseScene(&scene, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 3. Test ParseLight
            tinygltf::detail::ParseLight(&light, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 4. Test ParseNode
            tinygltf::detail::ParseNode(&node, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 5. Test ParseNormalTextureInfo
            tinygltf::detail::ParseNormalTextureInfo(&texinfo, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 6. Test ParseOrthographicCamera
            tinygltf::detail::ParseOrthographicCamera(&camera, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();
        #elif defined(TINYGLTF_USE_RAPIDJSON)
            // If the backend is RapidJSON, we need to use RapidJSON's types
            #include <rapidjson/document.h>
            using json_type = rapidjson::Document;
            json_type null_json;
            null_json.SetNull();
            
            // 1. Test JsonIsNull
            bool isNull = tinygltf::detail::JsonIsNull(null_json);
            (void)isNull;

            // 2. Test ParseScene
            tinygltf::detail::ParseScene(&scene, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 3. Test ParseLight
            tinygltf::detail::ParseLight(&light, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 4. Test ParseNode
            tinygltf::detail::ParseNode(&node, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 5. Test ParseNormalTextureInfo
            tinygltf::detail::ParseNormalTextureInfo(&texinfo, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();

            // 6. Test ParseOrthographicCamera
            tinygltf::detail::ParseOrthographicCamera(&camera, &err, null_json, store_original_json_for_extras_and_extensions);
            err.clear();
        #else
            // If no backend is specified, we cannot construct the JSON object
            // We'll skip the tests for now
        #endif
        
    } catch (const std::exception &) {
        // Ignore exceptions
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}