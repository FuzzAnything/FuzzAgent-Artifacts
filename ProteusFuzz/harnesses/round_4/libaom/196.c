// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (size < 1) return 0;

    // AV1E_SET_INTRA_DCT_ONLY
    val = data[0] & 1;
    ret |= aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, val);

    if (size > 1) {
        // AV1E_SET_SUPERBLOCK_SIZE
        // Valid values are typically 32, 64, 128. Fuzz with modulo to stay somewhat reasonable or test boundaries.
        int sb_size = 32 + (data[1] % 3) * 32; 
        if (sb_size > 128) sb_size = 128;
        ret |= aom_codec_control(ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
    }

    if (size > 2) {
        // AV1E_SET_ENABLE_QM
        val = data[2] & 1;
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
    }

    if (size > 3) {
        // AV1E_SET_ENABLE_CDEF
        val = data[3] & 1;
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val);
    }

    if (size > 4) {
        // AV1E_SET_ENABLE_KEYFRAME_FILTERING
        val = data[4] & 1;
        ret |= aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
    }

    if (size > 5) {
        // AV1E_SET_FORCE_VIDEO_MODE
        val = data[5] & 1;
        ret |= aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, val);
    }

    return ret;
}

int LLVMFuzzerTestOneInput_196(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int ret = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get the AV1 encoder interface
    // aom_codec_av1_cx() returns the codec interface for AV1 (encoder/decoder depending on usage)
    // For encoder, we use aom_codec_av1_cx() which is the standard way in libaom
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config from fuzz input to vary states
    cfg.g_w = 16 + (Data[0] % 100);
    cfg.g_h = 16 + (Data[0] % 100);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Data[0] % 10000);
    cfg.g_error_resilient = 1; // Enable error resilience for more robust testing

    // Initialize the encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Invoke the target control functions
    ret = fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    // Return 0 to indicate successful execution
    return 0;
}