// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
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

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    std::string err;
    std::string warn;

    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    
    bool store_flag = (Size > 0 && Data[0] % 2 == 0);
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_flag);

    // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir, check_sections
    // We provide empty base_dir and default check_sections
    bool ret = ctx.LoadASCIIFromString(
        &model, 
        &err, 
        &warn, 
        input_str.c_str(), 
        static_cast<unsigned int>(input_str.length()), 
        "", 
        tinygltf::REQUIRE_VERSION
    );

    if (ret && !model.skins.empty()) {
        // WriteGltfSceneToStream requires an ostream, not a string directly
        std::ostringstream o_stream;
        std::ostringstream e_stream;
        
        // We only need to call it to trigger serialization logic. 
        // The function signature is: WriteGltfSceneToStream(const Model*, std::ostream&, bool, bool)
        // We ignore the return value for fuzzing purposes.
        ctx.WriteGltfSceneToStream(&model, o_stream, true, false);
        ctx.WriteGltfSceneToStream(&model, e_stream, true, true);
    }

    return 0;
}