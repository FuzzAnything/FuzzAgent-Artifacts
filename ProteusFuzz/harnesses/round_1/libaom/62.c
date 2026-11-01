// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int width, height;

    // Use aom_codec_av1_cx() to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    // Check if it's an encoder interface
    if (!(aom_codec_get_caps(iface) & AOM_CODEC_CAP_ENCODER)) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Derive width/height from input data if possible, otherwise use defaults
    if (size >= 2) {
        width = data[0] ? data[0] : 16;
        height = data[1] ? data[1] : 16;
        // Ensure alignment
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    } else {
        width = 16;
        height = 16;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    int val;
    int enable_val;
    int transfer_val;
    
    // Need at least 3 bytes to safely access Data[0], Data[1], Data[2]
    if (Size < 3) return 0;

    // Initialize encoder
    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract values from fuzz input safely
    enable_val = Data[0] % 2; // 0 or 1
    transfer_val = Data[1] % 256; // 0-255
    val = Data[2] % 100; // Some int value

    // Fuzz AV1E_SET_ENABLE_PALETTE
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_val);
    (void)res;

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    res = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_val);
    (void)res;

    // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
    // This is a GET command, it expects a pointer to store the result
    int cdef_strength;
    res = aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_val);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_val);
    (void)res;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}