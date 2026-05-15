// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"

static volatile int sink;

static void exercise_version_functions(void) {
    int version = aom_codec_version();
    sink = version;
    
    const char *version_str = aom_codec_version_str();
    if (version_str) {
        sink += version_str[0];
    }
    
    const char *version_extra = aom_codec_version_extra_str();
    if (version_extra) {
        sink += version_extra[0];
    }
    
    const char *build_config = aom_codec_build_config();
    if (build_config) {
        sink += build_config[0];
    }
}

static void exercise_obu_type_conversion(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        OBU_TYPE type = (OBU_TYPE)data[i];
        const char *str = aom_obu_type_to_string(type);
        if (str) {
            sink += str[0];
        }
    }
}

static void exercise_error_function(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    
    if (size > 0) {
        ctx.name = "test";
        ctx.err = (aom_codec_err_t)(data[0] % 256);
        ctx.err_detail = "test error";
    }
    
    const char *error_str = aom_codec_error(&ctx);
    if (error_str) {
        sink += error_str[0];
    }
    
    const char *null_error_str = aom_codec_error(NULL);
    if (null_error_str) {
        sink += null_error_str[0];
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *data, size_t size) {
    exercise_version_functions();
    exercise_obu_type_conversion(data, size);
    exercise_error_function(data, size);
    
    return 0;
}