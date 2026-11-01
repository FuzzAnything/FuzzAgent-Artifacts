// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return -1;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    // Updated field names for newer libaom versions
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_298(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to drive control parameters
    int val = 0;
    if (Size > 0) val = Data[0];
    if (Size > 1) val |= (Data[1] << 8);
    if (Size > 2) val |= (Data[2] << 16);
    if (Size > 3) val |= (Data[3] << 24);

    // 1. aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE
    // Assuming it takes an int value for max partition size
    if (aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTRA
    // Assuming it takes an int (0 or 1)
    int enable_smooth = (val & 0x1) ? 1 : 0;
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_smooth) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 3. aom_codec_control_typechecked_AV1E_GET_LUMA_CDEF_STRENGTH
    // Assuming it returns an int via pointer
    int cdef_strength = 0;
    if (aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Assuming it takes a pointer to data and size
    if (Size >= 4) {
        if (aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)Data) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
    // Assuming it takes an int (0 or 1)
    int intra_default_tx = (val & 0x2) ? 1 : 0;
    if (aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx) != AOM_CODEC_OK) {
        // Ignore error
    }

    // 6. aom_codec_control_typechecked_AV1E_GET_BASELINE_GF_INTERVAL
    // Assuming it returns an int via pointer
    int gf_interval = 0;
    if (aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval) != AOM_CODEC_OK) {
        // Ignore error
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}