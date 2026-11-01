// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

// Helper to parse a null-terminated string from fuzz input
static const char* get_string_from_data(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset + 1 > size) return NULL;
    size_t len = 0;
    while (*offset + len + 1 <= size && len < max_len && data[*offset + len] != '\0') {
        len++;
    }
    if (*offset + len + 1 > size) return NULL;
    
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data + *offset, len);
    str[len] = '\0';
    *offset += len + 1;
    return str;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Initialize context
    aom_codec_err_t init_res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Fuzz logic
    size_t offset = 0;
    
    // Test aom_codec_version (always safe, no state change)
    (void)aom_codec_version();

    // Fuzz aom_codec_set_option
    // We try to set multiple options with random keys and values from the input
    while (offset < Size) {
        const char *key = get_string_from_data(Data, Size, &offset, 256);
        const char *value = get_string_from_data(Data, Size, &offset, 256);

        if (key && value) {
            aom_codec_err_t res = aom_codec_set_option(&ctx, key, value);
            (void)res; // Ignore return for fuzzing, just exercise the path
            free((void *)key);
            free((void *)value);
        } else {
            if (key) free((void *)key);
            if (value) free((void *)value);
            break;
        }
    }

    // Note: The other functions (aom_codec_control_typechecked_*) are empty/undefined
    // in the provided context, so we cannot call them safely without implementation.
    // We focus on the valid API: aom_codec_set_option and aom_codec_version.

    // 3. Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}