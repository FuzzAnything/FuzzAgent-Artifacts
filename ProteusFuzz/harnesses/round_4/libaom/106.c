// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE at aomcx.h:2153:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS at aomcx.h:2060:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY at aomcx.h:2150:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA at aomcx.h:2144:1 in aomcx.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/*
 * Helper function to initialize an AV1 encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (!ctx || !cfg) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

/*
 * Helper function to create a dummy input frame.
 * Returns NULL on failure.
 */
static aom_image_t *create_dummy_frame(int width, int height) {
    aom_image_t *img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    if (width <= 0 || height <= 0) width = 16, height = 16;
    
    img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;

    // Fill with dummy data using correct member name 'stride'
    memset(img->planes[0], 0x55, img->stride[0] * height);
    memset(img->planes[1], 0xAA, img->stride[1] * (height / 2));
    memset(img->planes[2], 0xAA, img->stride[2] * (height / 2));

    return img;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    aom_image_t *img = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;
    
    // Minimum size check to avoid trivial inputs causing immediate crashes in config
    if (Size < 10) return 0;

    // Extract configuration values from fuzz input
    int width = (Data[0] % 128) + 16;
    int height = (Data[1] % 128) + 16;
    int bit_depth = (Data[2] % 2) == 0 ? 8 : 10;
    int fps_num = (Data[3] % 60) + 1;
    int fps_den = (Data[3] % 2) + 1;

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) return 0;

    // Set basic config based on input
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    // Fixed member name: g_timebase instead of g_time_base
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = (Data[0] * Data[1]) + 100;
    cfg.g_error_resilient = (Data[2] % 3); // 0, 1, or 2

    // Re-init with new config
    aom_codec_destroy(&ctx);
    if (init_encoder(&ctx, &cfg) != 0) return 0;

    // Create a dummy frame to ensure encoder is in a valid state for controls
    img = create_dummy_frame(width, height);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Submit a frame to ensure encoder is active
    // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
    err = aom_codec_encode(&ctx, img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        // Continue to test controls even if encode failed
    }

    // Extract control values from input data
    int ctrl_val_1 = (int)(Data[4] & 0xFF);
    int ctrl_val_2 = (int)(Data[5] & 0xFF);
    int ctrl_val_3 = (int)(Data[6] & 0xFF);
    int ctrl_val_4 = (int)(Data[7] & 0xFF);
    int ctrl_val_5 = (int)(Data[8] & 0xFF);
    int ctrl_val_6 = (int)(Data[9] & 0xFF);

    // Invoke Target API Functions
    // The typechecked macros expand to functions taking (ctx, ctrl_id, value)
    // 1. AV1E_SET_ENABLE_PALETTE
    err = aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE(&ctx, AV1E_SET_ENABLE_PALETTE, ctrl_val_1);
    (void)err; 

    // 2. AV1E_SET_ENABLE_WARPED_MOTION
    err = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, ctrl_val_2);
    (void)err;

    // 3. AV1E_SET_ENABLE_RECT_PARTITIONS
    err = aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, ctrl_val_3);
    (void)err;

    // 4. AV1E_SET_LOOPFILTER_CONTROL
    err = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&ctx, AV1E_SET_LOOPFILTER_CONTROL, ctrl_val_4);
    (void)err;

    // 5. AV1E_SET_ENABLE_OVERLAY
    err = aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY(&ctx, AV1E_SET_ENABLE_OVERLAY, ctrl_val_5);
    (void)err;

    // 6. AV1E_SET_ENABLE_CFL_INTRA
    err = aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA(&ctx, AV1E_SET_ENABLE_CFL_INTRA, ctrl_val_6);
    (void)err;

    // Try to encode again to trigger usage of the new settings
    if (img) {
        err = aom_codec_encode(&ctx, img, 1, 1, 0);
        if (err == AOM_CODEC_OK) {
            // Drain the encoder
            // Fixed: Removed non-existent aom_codec_destroy_cx_data call.
            // The packet data is managed internally or freed via aom_codec_destroy.
            while ((pkt = aom_codec_get_cx_data(&ctx, &iter))) {
                // No explicit destroy needed for the packet pointer itself in this loop
                // as the context manages the memory until aom_codec_destroy is called.
                (void)pkt; 
            }
        }
    }

    // Cleanup
    if (img) aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}