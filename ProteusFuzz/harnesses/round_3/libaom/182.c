// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (Size > 0) cfg.g_w = (Data[0] % 4096) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 4096) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.g_threads = (Data[4] % 8) + 1;
    if (Size > 5) cfg.g_profile = (Data[5] % 4);
    if (Size > 6) cfg.g_bit_depth = (Data[6] % 2) + 8; // 8 or 10
    if (Size > 7) cfg.g_input_bit_depth = (Data[7] % 2) + 8;
    if (Size > 8) cfg.g_usage = (Data[8] % 2);
    if (Size > 9) cfg.rc_target_bitrate = (Data[9] % 10000) + 100;
    if (Size > 10) cfg.rc_buf_initial_sz = (Data[10] % 2000) + 100;
    if (Size > 11) cfg.rc_buf_optimal_sz = (Data[11] % 2000) + 100;
    if (Size > 12) cfg.rc_buf_sz = (Data[12] % 2000) + 100;

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame for encoding to ensure context is active
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image with fuzzed data if available
    if (Size > 13) {
        size_t bytes_per_plane = (cfg.g_w * cfg.g_h) / 2;
        size_t total_size = cfg.g_w * cfg.g_h + bytes_per_plane * 2;
        size_t copy_size = (Size - 13) < total_size ? (Size - 13) : total_size;
        if (img.planes[0]) {
            memcpy(img.planes[0], Data + 13, copy_size);
        }
    }

    // Invoke target API functions with fuzzed arguments
    // 1. AV1E_SET_ENABLE_INTERINTER_WEDGE
    int wedge_val = (Size > 0) ? (Data[0] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, wedge_val);

    // 2. AV1E_SET_ENABLE_PALETTE
    int palette_val = (Size > 1) ? (Data[1] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, palette_val);

    // 3. AV1E_SET_RENDER_SIZE
    // The crash indicates this API expects an array of two integers (width, height).
    // Passing a single integer caused a NULL pointer dereference in the library.
    if (Size > 2) {
        int render_w = (Data[2] % 4096) + 16;
        int render_h = (Size > 3) ? ((Data[3] % 4096) + 16) : render_w;
        int render_size[2] = {render_w, render_h};
        aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_size);
    }

    // 4. AV1E_GET_SEQ_LEVEL_IDX
    // This is a getter, usually takes a pointer to store the result.
    int level_idx = 0;
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

    // 5. AV1E_SET_MIN_PARTITION_SIZE
    int min_part = (Size > 4) ? (Data[4] % 128) + 4 : 4;
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part);

    // 6. AV1E_SET_ENABLE_PAETH_INTRA
    int paeth_val = (Size > 5) ? (Data[5] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, paeth_val);

    // Attempt to encode a frame to exercise the state changes
    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}