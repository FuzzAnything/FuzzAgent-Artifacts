// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"

static void test_version_functions(void) {
    const char *extra_str = aom_codec_version_extra_str();
    (void)extra_str;
    
    int version = aom_codec_version();
    (void)version;
    
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    const char *version_str = aom_codec_version_str();
    (void)version_str;
}

static void test_iface_name(const uint8_t *data, size_t size) {
    if (size == 0) {
        const char *name = aom_codec_iface_name(NULL);
        (void)name;
        return;
    }
    
    const char *name = aom_codec_iface_name(NULL);
    (void)name;
}

static void test_obu_type_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        const char *str = aom_obu_type_to_string(0);
        (void)str;
        return;
    }
    
    for (size_t i = 0; i < size && i < 8; i++) {
        OBU_TYPE type = (OBU_TYPE)data[i];
        const char *str = aom_obu_type_to_string(type);
        (void)str;
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    test_version_functions();
    
    test_iface_name(Data, Size);
    
    test_obu_type_string(Data, Size);
    
    return 0;
}