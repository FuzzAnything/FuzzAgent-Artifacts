// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_204(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Safe bounds checking for config initialization
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_profile = Data[4] % 4;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    char key[64];
    char value[64];

    // Ensure we have enough data for key and value
    size_t max_key_len = 63;
    size_t max_val_len = 63;
    
    size_t key_len = (Size > max_key_len + 1) ? max_key_len : Size - 1;
    size_t val_len = (Size - key_len > max_val_len + 1) ? max_val_len : Size - key_len - 1;

    if (key_len > 0) {
        memcpy(key, Data, key_len);
        key[key_len] = '\0';
    } else {
        strcpy(key, "dummy_key");
    }

    if (val_len > 0) {
        memcpy(value, Data + key_len, val_len);
        value[val_len] = '\0';
    } else {
        strcpy(value, "dummy_value");
    }

    ret = aom_codec_set_option(&ctx, key, value);
    (void)ret;

    const char *known_options[] = {
        "enable-intrabc",
        "skip-postproc-filtering",
        "tier-mask",
        "enable-cdef",
        "denoise-block-size"
    };
    const char *known_values[] = {
        "1", "0", "1", "1", "32"
    };

    for (int i = 0; i < 5; i++) {
        if (Size > (size_t)(i + 10) && Data[i + 10] % 2 == 0) {
            ret = aom_codec_set_option(&ctx, known_options[i], known_values[i]);
            (void)ret;
        }
    }

    cleanup_codec(&ctx);

    return 0;
}