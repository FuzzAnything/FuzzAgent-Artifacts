// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int ret = 0;
    size_t idx = 0;

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (idx < size) {
        int val = data[idx++];
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
    }

    // Fuzz AOME_SET_ACTIVEMAP
    if (idx + 2 < size) {
        unsigned int rows = data[idx++] & 0x0F; // Limit rows to avoid huge allocs
        unsigned int cols = data[idx++] & 0x0F; // Limit cols
        if (rows > 0 && cols > 0 && rows * cols < 100) {
            unsigned char *map = malloc(rows * cols);
            if (map) {
                size_t map_size = rows * cols;
                size_t copy_len = (size - idx) < map_size ? (size - idx) : map_size;
                memcpy(map, data + idx, copy_len);
                idx += copy_len;

                aom_active_map_t amap;
                amap.active_map = map;
                amap.rows = rows;
                amap.cols = cols;

                ret |= aom_codec_control(ctx, AOME_SET_ACTIVEMAP, &amap);
                free(map);
            }
        }
    }

    // Fuzz AV1E_GET_ACTIVEMAP
    if (idx < size) {
        aom_active_map_t amap;
        amap.active_map = NULL;
        amap.rows = 0;
        amap.cols = 0;
        ret |= aom_codec_control(ctx, AV1E_GET_ACTIVEMAP, &amap);
        if (amap.active_map) {
            free(amap.active_map);
        }
    }

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    if (idx < size) {
        int val = data[idx++];
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, val);
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    if (idx < size) {
        int val = data[idx++];
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    }

    return ret;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;
    
    // Try generic control with random ID (limited range for safety)
    if (size > 2) {
        int ctrl_id = (data[0] % 100) + 1; // Avoid 0
        int val = data[1];
        aom_codec_control(ctx, ctrl_id, val);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int is_encoder = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    aom_codec_ctx_t ctx;
    aom_codec_err_t err;
    aom_codec_iface_t *iface;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        // Use small dimensions to keep fuzzing fast
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) return 0;

        fuzz_encoder_controls(&ctx, payload, payload_size);
        cleanup_codec(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        // cpu_used is not a direct member in newer versions, relying on defaults or other flags if needed

        err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) return 0;

        fuzz_decoder_controls(&ctx, payload, payload_size);
        cleanup_codec(&ctx);
    }

    return 0;
}