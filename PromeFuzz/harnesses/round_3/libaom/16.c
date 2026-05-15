// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include <stdint.h>
#include <stddef.h>

static void fuzz_obu_type_to_string(const uint8_t *Data, size_t Size) {
    if (Size > 0) {
        // Use first byte as OBU_TYPE value
        OBU_TYPE type = (OBU_TYPE)Data[0];
        const char *result = aom_obu_type_to_string(type);
        (void)result; // Avoid unused variable warning
    }
}

static void fuzz_codec_iface_name(const uint8_t *Data, size_t Size) {
    aom_codec_iface_t *iface = NULL;
    
    if (Size > 0) {
        // Use first byte to decide whether to pass NULL or arbitrary pointer
        if (Data[0] % 2 == 0) {
            iface = NULL;
        } else {
            // Create an arbitrary pointer from the data
            iface = (aom_codec_iface_t *)(uintptr_t)Data;
        }
    }
    
    const char *result = aom_codec_iface_name(iface);
    (void)result; // Avoid unused variable warning
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Call all target API functions
    
    // 1. aom_obu_type_to_string
    fuzz_obu_type_to_string(Data, Size);
    
    // 2. aom_codec_version_extra_str
    const char *extra_str = aom_codec_version_extra_str();
    (void)extra_str;
    
    // 3. aom_codec_version
    int version = aom_codec_version();
    (void)version;
    
    // 4. aom_codec_build_config
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    // 5. aom_codec_iface_name
    fuzz_codec_iface_name(Data, Size);
    
    // 6. aom_codec_version_str
    const char *version_str = aom_codec_version_str();
    (void)version_str;
    
    return 0;
}