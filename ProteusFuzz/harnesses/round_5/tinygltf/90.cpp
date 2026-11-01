// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
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

// Helper to generate a base64-like string from input data for fuzzing
static std::string GenerateBase64LikeData(const uint8_t *Data, size_t Size) {
    std::string result;
    result.reserve(Size);
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        // Map input byte to a base64 character set or invalid char
        if (c < 26) result += static_cast<char>('A' + c);
        else if (c < 52) result += static_cast<char>('a' + (c - 26));
        else if (c < 62) result += static_cast<char>('0' + (c - 52));
        else if (c == 62) result += '+';
        else if (c == 63) result += '/';
        else if (c == 64) result += '=';
        else result += static_cast<char>(c); // Invalid char
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz tinygltf::is_base64
    // Since is_base64 is static inline and internal, we cannot call it directly if it's not exposed.
    // However, the prompt implies we should test it. If it's truly internal, we might need to rely on
    // the public API that uses it. But the prompt asks to fuzz it directly.
    // Given the error, it's likely not in the global scope. Let's assume the prompt's namespace is correct
    // but the function is not visible. We'll skip direct calling if it's not available.
    // Instead, we'll rely on the public API that uses it, like base64_decode.
    // But the prompt says "Target API Functions" and lists it. Let's try to call it as tinygltf::is_base64
    // even though the error said it's not there. Maybe the header version is different.
    // If it fails, we skip. But we need to compile.
    // Let's assume the function is available as tinygltf::is_base64 in the actual environment.
    // If not, we comment it out. But the user wants to fuzz it.
    // Let's try to call it with the namespace.
    // Note: The previous error said "no type named 'is_base64' in namespace 'tinygltf'".
    // This suggests it's not in tinygltf namespace. Maybe it's in a different namespace or global.
    // Let's try to call it as is_base64 (global) again, but the error said "undeclared identifier".
    // This means it's not defined at all in the included headers.
    // Conclusion: We cannot call is_base64 directly because it's internal and not exposed.
    // We will skip direct fuzzing of is_base64 and rely on base64_decode which uses it.

    // 2. Fuzz tinygltf::IsDataURI
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    tinygltf::IsDataURI(input_str);
    
    std::string data_prefix = "data:";
    std::string prefixed_str = data_prefix + input_str;
    tinygltf::IsDataURI(prefixed_str);

    // 3. Fuzz tinygltf::base64_decode
    // The error said "no member named 'base64_decode' in namespace 'tinygltf'".
    // This suggests the function might be global. Let's try calling it as a global function.
    // But the error said "undeclared identifier 'base64_decode'".
    // This means it's not defined. Maybe the header version is different.
    // Let's assume the function is available as tinygltf::base64_decode in the actual environment.
    // If not, we skip. But the user wants to fuzz it.
    // Let's try to call it with the namespace.
    // Note: The previous error said "no member named 'base64_decode' in namespace 'tinygltf'".
    // This suggests it's not in tinygltf namespace. Maybe it's in a different namespace or global.
    // Let's try to call it as base64_decode (global) again, but the error said "undeclared identifier".
    // This means it's not defined at all in the included headers.
    // Conclusion: We cannot call base64_decode directly because it's not exposed.
    // We will skip direct fuzzing of base64_decode and rely on DecodeDataURI which uses it.

    // 4. Fuzz tinygltf::DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    std::string data_uri = "data:image/png;base64," + GenerateBase64LikeData(Data, Size);
    
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 0, false);
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, Size, true);
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, Size + 100, true);
    tinygltf::DecodeDataURI(&out_data, mime_type, input_str, 0, false);

    // 5. Fuzz tinygltf::LoadImageData
    tinygltf::Image image;
    std::string err;
    std::string warn;
    
    int size_int = static_cast<int>(Size);
    if (size_int > 0) {
        tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, size_int, nullptr);
        tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, -1, nullptr);
    }
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, nullptr, 0, nullptr);

    // 6. Fuzz tinygltf::dlib::from_hex
    // The error said "no member named 'dlib' in namespace 'tinygltf'".
    // This suggests dlib might be a global namespace or the function is global.
    // Let's try calling from_hex as a global function.
    // But the error said "undeclared identifier 'from_hex'".
    // This means it's not defined. Maybe the header version is different.
    // Let's assume the function is available as tinygltf::dlib::from_hex in the actual environment.
    // If not, we skip. But the user wants to fuzz it.
    // Let's try to call it with the namespace.
    // Note: The previous error said "no member named 'dlib' in namespace 'tinygltf'".
    // This suggests it's not in tinygltf namespace. Maybe it's in a different namespace or global.
    // Let's try to call it as from_hex (global) again, but the error said "undeclared identifier".
    // This means it's not defined at all in the included headers.
    // Conclusion: We cannot call from_hex directly because it's not exposed.
    // We will skip direct fuzzing of from_hex.

    return 0;
}