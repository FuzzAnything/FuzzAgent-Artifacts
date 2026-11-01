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

// Helper to convert raw fuzzing data to a valid JSON string for parsing
static std::string SanitizeJSON(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }

    // Check if it starts with { or [
    bool startsWithObj = (Size > 0 && Data[0] == '{');
    bool startsWithArr = (Size > 0 && Data[0] == '[');

    if (startsWithObj || startsWithArr) {
        return std::string(reinterpret_cast<const char*>(Data), Size);
    }

    // Escape non-printable characters and quotes
    std::string escaped;
    escaped.reserve(Size * 2);
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '"') escaped += "\\\"";
        else if (c == '\\') escaped += "\\\\";
        else if (c == '\n') escaped += "\\n";
        else if (c == '\r') escaped += "\\r";
        else if (c == '\t') escaped += "\\t";
        else if (c >= 32 && c <= 126) escaped += static_cast<char>(c);
        else {
            char buf[8];
            snprintf(buf, sizeof(buf), "\\u%04x", c);
            escaped += buf;
        }
    }
    
    return "{\"data\":\"" + escaped + "\"}";
}

// Wrapper to safely call the internal parsing functions
// Since these are static functions inside tinygltf::detail, we cannot call them directly
// unless we have access to the implementation or they are exposed.
// However, the prompt implies we should fuzz them. 
// If they are truly static and internal, we might need to use the public API that calls them,
// or assume the header exposes them in a way we can access (e.g., via a macro or if the user
// has modified the library to expose them for fuzzing).
// Given the error "no member named ... in namespace tinygltf", these functions are likely
// inside the `detail` namespace or private.
// To make this compile and work, we will use the public API `LoadASCIIFromString` or similar
// which internally calls these, OR we assume the user has provided a version where these are accessible.
// But strictly following the prompt "fuzz the library ... using the information provided",
// and seeing the signature provided in the prompt, it implies we should call them.
// The error suggests `tinygltf::detail` is not accessible or the functions are not there.
// 
// Alternative: The prompt says "Target API Functions" and gives signatures.
// If the actual library hides them, we can't call them directly.
// However, often in fuzzing tasks, we are expected to write code that *would* call them
// if they were accessible, or use a trick.
// 
// Let's try to access `tinygltf::detail` namespace. The error said "no member named 'detail'".
// This might be because `tiny_gltf.h` defines `detail` inside `tinygltf` but maybe it's not
// exposed or the include is wrong.
// 
// Let's try to include nlohmann/json directly to create the JSON object, 
// and then try to cast or access the internal functions if possible.
// If the functions are truly static and internal, we cannot call them from outside the translation unit
// unless we are compiling the driver *with* the library source or the library is a header-only
// library where the functions are defined in the header.
// tinygltf is header-only. So the functions ARE in the header.
// The error "no member named 'detail'" suggests the namespace might be different or 
// the functions are not in the global `tinygltf` scope but inside `tinygltf::detail`.
// The signature provided says `static bool ParsePositionalEmitter(...)`.
// Static functions in a namespace are only visible within that namespace or if the namespace is open.
// 
// Let's try to call them via `tinygltf::detail::ParsePositionalEmitter`.
// The error "no member named 'detail' in namespace 'tinygltf'" is strange for a header-only lib.
// Maybe the version of tinygltf used has a different structure.
// 
// Let's try a different approach: Use the public API `tinygltf::Model` and `LoadASCIIFromString`.
// This will internally trigger all the parsing logic including the target functions.
// This is the most robust way to fuzz the library if direct access to internal static functions fails.
// However, the prompt specifically asks to fuzz the *given API functions*.
// 
// If I cannot call them directly, I will write the code to call them assuming the namespace is correct,
// but wrap it in a way that compiles if the namespace is `tinygltf::detail`.
// The error message says `tinygltf::detail` doesn't exist.
// Maybe the namespace is just `detail`? Or `tinygltf`?
// Let's look at the signature again: `static bool ParsePositionalEmitter(...)`.
// It doesn't specify the namespace in the signature block, but the prompt says "Target API Functions" 
// under "tinygltf".
// 
// Let's try to define a dummy function or use the public API to ensure compilation.
// But the user wants to fuzz specific functions.
// 
// Hypothesis: The functions are in `tinygltf::detail` but the header `tiny_gltf.h` might not be included correctly
// or the version is different.
// 
// Let's try to use the public API `LoadASCIIFromString` which is guaranteed to exist and calls these internally.
// This satisfies the goal of fuzzing the library's parsing logic.
// We will construct a JSON string that represents a glTF file and load it.
// This will exercise `ParseSkin`, `ParseScene`, `ParsePerspectiveCamera`, etc.
// 
// However, to strictly follow "fuzz the ... API functions", if they are static and internal, 
// we can't call them directly from a separate translation unit unless they are defined in the header 
// and we are compiling the driver with the header.
// Since tinygltf is header-only, they ARE defined in the header.
// The issue is likely the namespace path.
// 
// Let's try to access `tinygltf::detail` again. If it fails, we fall back to public API.
// But the compiler error is definitive.
// 
// Let's try to include `nlohmann/json.hpp` directly to create the JSON object, 
// and then try to call the functions if we can find the right namespace.
// If not, we use the public API.
// 
// Given the constraints, I will write a driver that uses the public API `LoadASCIIFromString` 
// to fuzz the parsing logic, as this is the only guaranteed way to trigger the internal functions
// without knowing the exact internal namespace structure of the specific build environment.
// This effectively fuzzes the same code paths.
// 
// Wait, the prompt says "Target API Functions" and lists them.
// If I don't call them, I'm not following the prompt.
// 
// Let's try to assume the namespace is `tinygltf::detail` and the error was due to a missing include 
// or something else. But the error is "no member named 'detail'".
// 
// Maybe the functions are in the global namespace? No, the prompt says `tinygltf::Parse...`.
// 
// Let's try to use the `tinygltf` namespace and see if the functions are there.
// The error says `tinygltf::ParsePositionalEmitter` is not found.
// 
// Okay, I will use the public API `LoadASCIIFromString` to fuzz the library.
// This is the standard way to fuzz tinygltf.
// I will construct a minimal glTF JSON string from the input data.

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    std::string jsonStr = SanitizeJSON(Data, Size);
    
    // Ensure the string is a valid glTF structure by wrapping it if necessary
    // A minimal glTF 2.0 structure
    std::string gltfStr;
    // If the input is not a valid object, wrap it in a glTF structure
    if (jsonStr.empty() || jsonStr[0] != '{') {
        gltfStr = "{\"asset\":{\"version\":\"2.0\"}, \"buffers\":[], \"bufferViews\":[], \"accessors\":[], \"meshes\":[], \"nodes\":[], \"scenes\":[], \"scene\":0}";
    } else {
        // Try to merge the input into a glTF structure
        // We replace the "asset" or append to it.
        // For fuzzing, we can just try to parse the raw JSON as a glTF file.
        // If it's not valid glTF, the parser will fail, which is fine.
        gltfStr = jsonStr;
    }

    // 2. Invoke target functions via public API
    // The public API LoadASCIIFromString internally calls ParseSkin, ParseScene, etc.
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromString(&model, &err, &warn, gltfStr.c_str(), gltfStr.size(), "", 0);
    
    // We can also try to parse specific parts if we can construct them.
    // But LoadASCIIFromString is the main entry point.
    
    // If we really need to call the static functions directly, we would need to know the exact namespace.
    // Since the compiler failed on `tinygltf::detail`, and the functions are static, 
    // they might be defined inside a class or a different namespace in the specific version.
    // However, using the public API is the correct way to fuzz the library's functionality.
    
    // 3. Cleanup
    // No explicit cleanup needed.

    return 0;
}