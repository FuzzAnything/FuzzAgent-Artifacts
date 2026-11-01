// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure some parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Ensure minimum width
    cfg.g_h = (Data[1] % 256) + 16; // Ensure minimum height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder context
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions
    uint8_t *ptr = (uint8_t *)Data;
    size_t offset = 0;

    // 1. AV1E_SET_INTRA_DCT_ONLY
    if (offset + 1 <= Size) {
        int intra_dct_only = ptr[offset] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);
        offset++;
    }

    // 2. AV1E_SET_DENOISE_BLOCK_SIZE
    if (offset + 1 <= Size) {
        int denoise_block_size = ptr[offset] % 4; // Valid values: 0-3
        ret = aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);
        offset++;
    }

    // 3. AV1E_SET_ENABLE_CDEF
    if (offset + 1 <= Size) {
        int enable_cdef = ptr[offset] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
        offset++;
    }

    // 4. AV1E_SET_ENABLE_KEYFRAME_FILTERING
    if (offset + 1 <= Size) {
        int enable_keyframe_filtering = ptr[offset] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, enable_keyframe_filtering);
        offset++;
    }

    // 5. AV1E_SET_LOOPFILTER_CONTROL
    if (offset + 1 <= Size) {
        int loopfilter_control = ptr[offset] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_control);
        offset++;
    }

    // 6. AV1E_SET_SKIP_POSTPROC_FILTERING
    if (offset + 1 <= Size) {
        int skip_postproc_filtering = ptr[offset] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc_filtering);
        offset++;
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}