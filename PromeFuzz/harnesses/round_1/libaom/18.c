// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

static aom_codec_ctx_t codec_ctx;
static aom_image_t img;
static aom_fixed_buf_t fixed_buf;
static uint8_t buffer[4096];
static aom_codec_enc_cfg_t cfg;

static void initialize_encoder(void) {
    // Get encoder interface
    extern aom_codec_iface_t aom_codec_av1_cx_algo;
    aom_codec_iface_t *iface = &aom_codec_av1_cx_algo;
    
    if (!iface) return;
    
    // Initialize with default configuration
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    aom_codec_enc_init_ver(&codec_ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
}

static void initialize_image(void) {
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    // Fill with some data
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            memset(img.planes[i], 128, img.stride[i] * img.h / (i ? 2 : 1));
        }
    }
}

static void cleanup(void) {
    aom_img_free(&img);
    aom_codec_destroy(&codec_ctx);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize encoder and image
    initialize_encoder();
    initialize_image();
    
    // Use first byte to select control ID
    int ctrl_id = Data[0] + 1; // Ensure non-zero
    int value = Data[1];
    
    // Try aom_codec_control with different parameters
    aom_codec_control(&codec_ctx, ctrl_id, value);
    aom_codec_control(&codec_ctx, ctrl_id, (void*)Data);
    aom_codec_control(&codec_ctx, ctrl_id, (int)Size);
    
    // Try encoding with different parameters
    aom_codec_pts_t pts = Data[2] * 1000;
    unsigned long duration = Data[3] + 1; // Ensure non-zero
    aom_enc_frame_flags_t flags = Data[1] % 4;
    
    // Encode a frame
    aom_codec_encode(&codec_ctx, &img, pts, duration, flags);
    
    // Encode with NULL to flush
    aom_codec_encode(&codec_ctx, NULL, 0, 0, 0);
    
    // Get preview frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&codec_ctx);
    (void)preview; // Suppress unused warning
    
    // Set up fixed buffer
    fixed_buf.buf = buffer;
    fixed_buf.sz = sizeof(buffer);
    unsigned int pad_before = Data[0] % 16;
    unsigned int pad_after = Data[1] % 16;
    
    // Try setting output buffer
    aom_codec_set_cx_data_buf(&codec_ctx, &fixed_buf, pad_before, pad_after);
    
    // Try with NULL buffer
    aom_codec_set_cx_data_buf(&codec_ctx, NULL, 0, 0);
    
    // Cleanup
    cleanup();
    
    return 0;
}