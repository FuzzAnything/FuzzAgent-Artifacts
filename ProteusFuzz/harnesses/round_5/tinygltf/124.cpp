// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::FindMember at tiny_gltf.h:3592:6 in tiny_gltf.h
// tinygltf::ParseExtensionsProperty at tiny_gltf.h:4220:13 in tiny_gltf.h
// tinygltf::detail::ForEachInArray at tiny_gltf.h:5963:6 in tiny_gltf.h
// tinygltf::ParseAsset at tiny_gltf.h:4280:13 in tiny_gltf.h
// tinygltf::ParseAsset at tiny_gltf.h:4280:13 in tiny_gltf.h
// tinygltf::detail::JsonParse at tiny_gltf.h:1874:6 in tiny_gltf.h
// tinygltf::detail::JsonParse at tiny_gltf.h:1874:6 in tiny_gltf.h
// tinygltf::detail::JsonParse at tiny_gltf.h:1874:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Include the tinygltf library
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

// Helper to create a dummy file if needed
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Helper to read dummy file if needed
static std::string ReadDummyFile() {
    std::ifstream file("./dummy_file", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return "";
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string contents(size, '\0');
    if (size > 0 && file.read(&contents[0], size)) {
        return contents;
    }
    return "";
}

// Helper to safely parse JSON string into tinygltf::detail::json
static bool ParseJsonString(const std::string &jsonStr, tinygltf::detail::json &doc) {
    try {
        tinygltf::detail::JsonParse(doc, jsonStr.c_str(), jsonStr.size(), false);
        return true;
    } catch (...) {
        return false;
    }
}

extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file for potential file-based operations if needed
    WriteDummyFile(Data, Size);

    // Convert input to string for JSON parsing attempts
    std::string inputStr(reinterpret_cast<const char*>(Data), Size);

    // Target 1: tinygltf::detail::JsonParse
    {
        tinygltf::detail::json doc;
        // Try parsing with throwExc = false
        try {
            tinygltf::detail::JsonParse(doc, inputStr.c_str(), inputStr.size(), false);
        } catch (...) {
            // Ignore exceptions
        }
        // Try parsing with throwExc = true
        try {
            tinygltf::detail::JsonParse(doc, inputStr.c_str(), inputStr.size(), true);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Target 2: tinygltf::detail::IsArray
    {
        tinygltf::detail::json doc;
        if (ParseJsonString(inputStr, doc)) {
            bool isArray = tinygltf::detail::IsArray(doc);
            (void)isArray; // Use result to avoid unused warning
        }
    }

    // Target 3: tinygltf::detail::FindMember
    {
        tinygltf::detail::json doc;
        if (ParseJsonString(inputStr, doc)) {
            tinygltf::detail::json_iterator it;
            // Try finding various members
            const char *members[] = {"version", "generator", "minVersion", "copyright", "extensions", "extras", "null", ""};
            for (const char *member : members) {
                bool found = tinygltf::detail::FindMember(doc, member, it);
                (void)found;
            }
        }
    }

    // Target 4: tinygltf::ParseExtensionsProperty
    {
        tinygltf::detail::json doc;
        if (ParseJsonString(inputStr, doc)) {
            tinygltf::ExtensionMap extMap;
            std::string err;
            bool result = tinygltf::ParseExtensionsProperty(&extMap, &err, doc);
            (void)result;
        }
    }

    // Target 5: tinygltf::detail::ForEachInArray
    {
        tinygltf::detail::json doc;
        if (ParseJsonString(inputStr, doc)) {
            // Define a simple callback that always returns true
            auto callback = [](const tinygltf::detail::json &item) {
                (void)item;
                return true;
            };
            // Try iterating over various members
            const char *members[] = {"nodes", "meshes", "materials", "textures", "samplers", "images", "buffers", "bufferViews", "accessors", "skins", "cameras", "scene"};
            for (const char *member : members) {
                bool result = tinygltf::detail::ForEachInArray(doc, member, callback);
                (void)result;
            }
        }
    }

    // Target 6: tinygltf::ParseAsset
    {
        tinygltf::detail::json doc;
        if (ParseJsonString(inputStr, doc)) {
            tinygltf::Asset asset;
            std::string err;
            // Try with store_original_json_for_extras_and_extensions = false
            bool result1 = tinygltf::ParseAsset(&asset, &err, doc, false);
            (void)result1;
            // Try with store_original_json_for_extras_and_extensions = true
            bool result2 = tinygltf::ParseAsset(&asset, &err, doc, true);
            (void)result2;
        }
    }

    return 0;
}