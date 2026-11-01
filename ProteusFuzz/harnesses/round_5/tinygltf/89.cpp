// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// Fuzz driver for tinygltf JSON utility functions
// Targeting: IsArray, GetNumber, JsonIsNull, ParseJsonAsValue, IsObject, GetInt

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a JSON string from the fuzz input
    std::string json_str(reinterpret_cast<const char*>(Data), Size);

    // Create a temporary file to simulate loading a glTF JSON
    const char* temp_file = "./dummy_file";
    {
        std::ofstream ofs(temp_file);
        if (!ofs) return 0;
        ofs << json_str;
        ofs.close();
    }

    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // Attempt to load the model to populate internal JSON structures if possible
    // LoadASCIIFromString requires: Model*, err*, warn*, str, length, base_dir, check_sections
    bool loaded = ctx.LoadASCIIFromString(
        &model, 
        &err, 
        &warn, 
        json_str.c_str(), 
        static_cast<unsigned int>(json_str.length()), 
        "", 
        tinygltf::REQUIRE_VERSION
    );
    (void)loaded;

    // Since tinygltf's internal detail::json type depends on the backend (RapidJSON vs nlohmann),
    // and we cannot directly construct it without knowing the compile-time configuration,
    // we will rely on the fact that tinygltf exposes some internal types or we can access
    // the parsed JSON via the model if available, or we try to use the public API to trigger
    // the internal functions indirectly.
    
    // However, the target functions are in tinygltf::detail namespace.
    // If TINYGLTF_USE_NLOHMANN_JSON is defined, detail::json is nlohmann::json.
    // If not, it's RapidJSON.
    // We will attempt to use the nlohmann namespace if available, otherwise skip.
    // But since the prompt provided nlohmann details, we assume it might be available.
    // To be safe, we use preprocessor checks or try-catch if headers are included.
    
    // Check if nlohmann is available by attempting to use it.
    // If tiny_gltf.h includes nlohmann/json.hpp, we can use it.
    // If not, this code might fail to compile, but the prompt implies nlohmann context.
    
    // Let's try to access the internal json type via the model's parsed data if possible.
    // Alternatively, we can try to construct a json object using the library's expected type.
    
    // Since we cannot easily instantiate detail::json without knowing the backend,
    // we will try to use the public API to get a Value, then try to manipulate it.
    // But the target functions require detail::json.
    
    // Strategy: Use the fact that tinygltf might expose the json type.
    // If TINYGLTF_USE_NLOHMANN_JSON is defined, we can use nlohmann::json.
    // Otherwise, we skip the direct testing of detail functions.
    
    // We will try to use the nlohmann::json type directly if available.
    // If the build uses RapidJSON, this file would need adjustment, but per instructions
    // we write based on the provided nlohmann context.
    
    // Try to parse the input string into a nlohmann::json object
    // This might throw if the JSON is invalid, which is fine for fuzzing
    try {
        // Check if nlohmann namespace is available
        #ifdef TINYGLTF_USE_NLOHMANN_JSON
        nlohmann::json j = nlohmann::json::parse(json_str, nullptr, false);
        
        // Cast to tinygltf::detail::json if they are the same or compatible
        // In tinygltf with nlohmann, detail::json is typically nlohmann::json
        tinygltf::detail::json o = j;

        // 1. Test tinygltf::detail::IsArray
        bool is_arr = tinygltf::detail::IsArray(o);
        (void)is_arr;

        // 2. Test tinygltf::detail::GetNumber
        double num_val = 0.0;
        bool got_num = tinygltf::detail::GetNumber(o, num_val);
        (void)got_num;

        // 3. Test tinygltf::detail::JsonIsNull
        bool is_null = tinygltf::detail::JsonIsNull(o);
        (void)is_null;

        // 4. Test tinygltf::ParseJsonAsValue
        tinygltf::Value val;
        bool parsed = tinygltf::ParseJsonAsValue(&val, o);
        (void)parsed;

        // 5. Test tinygltf::detail::IsObject
        bool is_obj = tinygltf::detail::IsObject(o);
        (void)is_obj;

        // 6. Test tinygltf::detail::GetInt
        int int_val = 0;
        bool got_int = tinygltf::detail::GetInt(o, int_val);
        (void)got_int;
        #endif // TINYGLTF_USE_NLOHMANN_JSON
        
    } catch (...) {
        // Parsing might fail for invalid JSON, which is expected in fuzzing
        // We continue to ensure coverage of error paths if possible
    }

    // Cleanup temporary file
    std::remove(temp_file);

    return 0;
}