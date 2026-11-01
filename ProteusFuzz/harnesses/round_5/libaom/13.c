// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/*
 * Fuzz driver for libaom AV1 codec control functions.
 * Fixed AddressSanitizer stack-buffer-overflow by allocating sufficient buffer for CDEF strengths.
 * Updated buffer size to 16 to safely handle potential 64-byte writes from the library.
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    struct aom_codec_enc_cfg cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzz data to configure width/height within safe bounds
    cfg.g_w = (data[0] % 1024) + 16;
    cfg.g_h = (data[1] % 1024) + 16;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 0;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    struct aom_codec_dec_cfg cfg;
    // Initialize struct to zero to avoid undefined behavior
    memset(&cfg, 0, sizeof(cfg));
    
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize Encoder
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    
    if (init_encoder(&enc_ctx, Data, Size)) {
        // Attempt to call encoder-specific control functions
        
        // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
        int val_tx = (Data[0] % 2);
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val_tx);

        // 2. AV1E_SET_FILM_GRAIN_TEST_VECTOR
        if (Size >= 16) {
            aom_codec_control(&enc_ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)Data);
        }

        // 4. AV1E_SET_FILM_GRAIN_TABLE
        if (Size >= 16) {
             aom_codec_control(&enc_ctx, AV1E_SET_FILM_GRAIN_TABLE, (void*)Data);
        }

        // 5. AV1E_SET_ENABLE_GLOBAL_MOTION
        int val_gm = (Data[1] % 2);
        aom_codec_control(&enc_ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val_gm);

        // 6. AV1E_GET_LUMA_CDEF_STRENGTH
        // FIX: Increased buffer size from 8 to 16 to prevent stack-buffer-overflow.
        // The library may write up to 64 bytes (16 ints) depending on configuration.
        int cdef_strengths[16]; 
        memset(cdef_strengths, 0, sizeof(cdef_strengths));
        aom_codec_control(&enc_ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);

        cleanup_encoder(&enc_ctx);
    }

    // Initialize Decoder
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));

    if (init_decoder(&dec_ctx, Data, Size)) {
        // 3. AV1D_SET_SKIP_FILM_GRAIN
        int skip_fg = (Size >= 3) ? (Data[2] % 2) : 0;
        aom_codec_control(&dec_ctx, AV1D_SET_SKIP_FILM_GRAIN, skip_fg);

        cleanup_decoder(&dec_ctx);
    }

    return 0;
}