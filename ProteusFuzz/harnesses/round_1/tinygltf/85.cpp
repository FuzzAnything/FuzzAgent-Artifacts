// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::ObjectBegin at tiny_gltf.h:3552:29 in tiny_gltf.h
// tinygltf::detail::ObjectEnd at tiny_gltf.h:3560:29 in tiny_gltf.h
// tinygltf::detail::GetValue at tiny_gltf.h:3622:21 in tiny_gltf.h
// tinygltf::detail::GetKey at tiny_gltf.h:3570:13 in tiny_gltf.h
// tinygltf::detail::ArrayBegin at tiny_gltf.h:3528:35 in tiny_gltf.h
// tinygltf::detail::ArrayEnd at tiny_gltf.h:3536:35 in tiny_gltf.h
// tinygltf::detail::GetValue at tiny_gltf.h:3622:21 in tiny_gltf.h
// tinygltf::detail::ObjectBegin at tiny_gltf.h:3552:29 in tiny_gltf.h
// tinygltf::detail::ObjectEnd at tiny_gltf.h:3560:29 in tiny_gltf.h
// tinygltf::detail::GetValue at tiny_gltf.h:3622:21 in tiny_gltf.h
// tinygltf::detail::GetKey at tiny_gltf.h:3570:13 in tiny_gltf.h
// tinygltf::detail::ArrayBegin at tiny_gltf.h:3528:35 in tiny_gltf.h
// tinygltf::detail::ArrayEnd at tiny_gltf.h:3536:35 in tiny_gltf.h
// tinygltf::detail::GetValue at tiny_gltf.h:3622:21 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

// Define implementation before including the header
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        
        // Create a tinygltf::detail::json object
        // We assume the default backend (nlohmann::json) for parsing
        // If TINYGLTF_USE_RAPIDJSON is defined, the type alias changes but the API remains similar
        // However, construction might differ. We try to parse, and fallback to empty object on failure.
        
        tinygltf::detail::json j;
        
        // Attempt to parse the input as JSON
        // This relies on the backend having a parse method (nlohmann::json does)
        // If RapidJSON is used, this line might fail to compile if not handled via macros,
        // but typically fuzzers are built with specific flags. 
        // Given the error was about 'detail' namespace, defining TINYGLTF_IMPLEMENTATION 
        // should expose the internal types.
        
        try {
            // Try to parse using the standard nlohmann::json API
            // If the backend is RapidJSON, this specific call might not exist directly on the alias
            // without helper functions, but tinygltf's internal logic usually wraps this.
            // Since we are fuzzing internal functions, we assume the environment allows direct access.
            j = tinygltf::detail::json::parse(input_str);
        } catch (...) {
            // If parsing fails or method doesn't exist (e.g. RapidJSON without wrapper),
            // fallback to an empty object.
            j = tinygltf::detail::json::object();
        }

        // Test ObjectBegin and ObjectEnd
        if (j.is_object()) {
            auto it = tinygltf::detail::ObjectBegin(j);
            auto end = tinygltf::detail::ObjectEnd(j);
            while (it != end) {
                // Test GetValue
                const tinygltf::detail::json &val = tinygltf::detail::GetValue(it);
                // Test GetKey
                std::string key = tinygltf::detail::GetKey(it);
                ++it;
            }
        }

        // Test ArrayBegin and ArrayEnd
        if (j.is_array()) {
            auto it = tinygltf::detail::ArrayBegin(j);
            auto end = tinygltf::detail::ArrayEnd(j);
            while (it != end) {
                // Test GetValue
                const tinygltf::detail::json &val = tinygltf::detail::GetValue(it);
                ++it;
            }
        }

        // Additional exploration: Try to force object/array creation from input if possible
        // to ensure we hit different code paths in the iteration logic.
        try {
            tinygltf::detail::json obj = tinygltf::detail::json::parse(input_str);
            if (obj.is_object()) {
                auto it = tinygltf::detail::ObjectBegin(obj);
                auto end = tinygltf::detail::ObjectEnd(obj);
                while (it != end) {
                    const tinygltf::detail::json &val = tinygltf::detail::GetValue(it);
                    std::string key = tinygltf::detail::GetKey(it);
                    ++it;
                }
            }
        } catch (...) {
            // Ignore
        }

        try {
            tinygltf::detail::json arr = tinygltf::detail::json::parse(input_str);
            if (arr.is_array()) {
                auto it = tinygltf::detail::ArrayBegin(arr);
                auto end = tinygltf::detail::ArrayEnd(arr);
                while (it != end) {
                    const tinygltf::detail::json &val = tinygltf::detail::GetValue(it);
                    ++it;
                }
            }
        } catch (...) {
            // Ignore
        }

    } catch (...) {
        // Catch any exceptions to prevent fuzzer crashes
        return 0;
    }

    return 0;
}