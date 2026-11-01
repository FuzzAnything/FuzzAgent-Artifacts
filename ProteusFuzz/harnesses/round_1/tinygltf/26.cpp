// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include "tiny_gltf.h"

// Helper to write dummy image data to file for file-based tests
static void WriteDummyFile(const std::string &filename, const std::vector<unsigned char> &data) {
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        ofs.close();
    }
}

// Replicating the logic of WriteToMemory_stbi since it's static in the library
static void WriteToMemory_stbi_impl(void *context, void *data, int size) {
    std::vector<unsigned char> *buffer = reinterpret_cast<std::vector<unsigned char>*>(context);
    if (buffer == nullptr || data == nullptr || size <= 0) return;
    buffer->insert(buffer->end(), reinterpret_cast<unsigned char*>(data), reinterpret_cast<unsigned char*>(data) + size);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz WriteToMemory_stbi
    {
        std::vector<unsigned char> buffer;
        buffer.reserve(Size + 100);
        int safe_size = (Size > static_cast<size_t>(std::numeric_limits<int>::max())) ? 
                        std::numeric_limits<int>::max() : static_cast<int>(Size);
        
        WriteToMemory_stbi_impl(&buffer, const_cast<uint8_t*>(Data), safe_size);
    }

    // 2. Fuzz LoadImageData (both overloads)
    {
        tinygltf::Image image;
        std::string err, warn;
        int image_idx = 0;
        int req_width = 0;
        int req_height = 0;
        void *user_data = nullptr;

        tinygltf::LoadImageData(&image, image_idx, &err, &warn, req_width, req_height, Data, Size, user_data);
        tinygltf::LoadImageData(&image, image_idx, &err, &warn, req_width, req_height, Data, Size, nullptr);
    }

    // 3. Fuzz base64_decode
    // Note: base64_decode is often in the tinygltf namespace but might be internal.
    // We try to call it if it exists. If the compiler fails, it means it's not exposed in this build.
    // We wrap it in a try-catch or just call it and let the linker handle it if it's a symbol issue.
    // However, since we are compiling, if it's not in the header, we can't call it.
    // Given the error, it seems the function is not in the public namespace.
    // We will skip this part for now to avoid compilation errors, as the task requires a compiling driver.
    // If the function is available, we would do:
    // std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // std::string decoded = tinygltf::base64_decode(input_str);
    // But since it's not available, we skip it.
    // We can try to use the internal namespace if available, but the error says no member named 'detail' in tinygltf.
    // Therefore, we cannot call base64_decode directly.
    // We will skip this part for now.

    // 4. Fuzz ParseImage
    // Since we cannot create a tinygltf::detail::json object directly due to namespace issues,
    // we will skip the ParseImage fuzzing for now as it requires a valid JSON object.
    // We can try to use the nlohmann::json if it's included, but the error suggests it's not available.
    // Therefore, we skip this part for now.

    // 5. Fuzz WriteImageData
    {
        tinygltf::Image image;
        image.image.assign(Data, Data + Size);
        image.width = (Size > 0) ? static_cast<int>(Data[0]) : 0;
        image.height = (Size > 1) ? static_cast<int>(Data[1]) : 0;
        image.name = "fuzz_image";
        
        std::string basepath = ".";
        std::string filename = "dummy_file";
        bool embedImages = (Size > 2 && Data[2] % 2 == 0);
        
        tinygltf::FsCallbacks fs;
        tinygltf::URICallbacks uri_cb;
        fs = {};
        uri_cb = {};
        
        std::string out_uri;
        
        if (!embedImages) {
            WriteDummyFile(filename, image.image);
        }

        bool result = tinygltf::WriteImageData(&basepath, &filename, &image, embedImages, &fs, &uri_cb, &out_uri, nullptr);
        (void)result;
    }

    return 0;
}