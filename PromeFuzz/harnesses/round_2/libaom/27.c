// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static void fuzz_aom_codec_build_config(void) {
    const char *config = aom_codec_build_config();
    (void)config;
}

static void fuzz_aom_codec_version_str(void) {
    const char *version = aom_codec_version_str();
    (void)version;
}

static void fuzz_aom_obu_type_to_string(const uint8_t *data, size_t size) {
    if (size > 0) {
        OBU_TYPE obu_type = (OBU_TYPE)(data[0]);
        const char *obu_str = aom_obu_type_to_string(obu_type);
        (void)obu_str;
    }
}

static void fuzz_aom_codec_iface_name(const uint8_t *data, size_t size) {
    if (size == 0) {
        const char *name = aom_codec_iface_name(NULL);
        (void)name;
    } else {
        // Use a fixed-size buffer instead of trying to determine the size of aom_codec_iface_t
        // The actual size isn't needed for fuzzing since we're just passing a pointer
        char buffer[256];
        size_t copy_size = size < 256 ? size : 256;
        memcpy(buffer, data, copy_size);
        
        aom_codec_iface_t *iface = (aom_codec_iface_t *)buffer;
        const char *name = aom_codec_iface_name(iface);
        (void)name;
    }
}

static void fuzz_aom_codec_version(void) {
    int version = aom_codec_version();
    (void)version;
}

static void fuzz_aom_codec_version_extra_str(void) {
    const char *extra = aom_codec_version_extra_str();
    (void)extra;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    fuzz_aom_codec_build_config();
    fuzz_aom_codec_version_str();
    fuzz_aom_obu_type_to_string(data, size);
    fuzz_aom_codec_iface_name(data, size);
    fuzz_aom_codec_version();
    fuzz_aom_codec_version_extra_str();
    
    return 0;
}