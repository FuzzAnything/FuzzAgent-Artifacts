// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"

static void init_image(aom_image_t *img, const uint8_t *data, size_t size) {
    memset(img, 0, sizeof(*img));
    
    // Use a simple format - I420
    img->fmt = AOM_IMG_FMT_I420;
    img->bit_depth = 8;
    img->w = 64;
    img->d_w = 64;
    img->h = 64;
    img->d_h = 64;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    
    // Calculate plane sizes for I420
    size_t y_plane_size = img->w * img->h;
    size_t uv_plane_size = (img->w / 2) * (img->h / 2);
    size_t total_size = y_plane_size + 2 * uv_plane_size;
    
    if (total_size > size) {
        total_size = size;
    }
    
    // Allocate and set up planes
    img->img_data = malloc(total_size);
    if (!img->img_data) return;
    
    memcpy(img->img_data, data, total_size);
    img->img_data_owner = 1;
    img->self_allocd = 1;
    
    // Set plane pointers
    img->planes[0] = img->img_data;
    img->stride[0] = img->w;
    
    if (total_size > y_plane_size) {
        img->planes[1] = img->img_data + y_plane_size;
        img->stride[1] = img->w / 2;
        
        if (total_size > y_plane_size + uv_plane_size) {
            img->planes[2] = img->img_data + y_plane_size + uv_plane_size;
            img->stride[2] = img->w / 2;
        }
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img && img->img_data_owner) {
        free(img->img_data);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some basic configuration from fuzzer data
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Create and initialize image
    aom_image_t img;
    init_image(&img, Data, Size);
    
    // Encode a frame
    aom_codec_encode(&ctx, &img, 0, 1, 0);
    
    // Try to reconfigure with modified settings
    if (Size > 200) {
        cfg.rc_target_bitrate = (Data[100] % 1000) + 100;
        aom_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Test some control codes
    if (Size > 150) {
        int cpu_used = Data[120] % 9;
        aom_codec_control(&ctx, AOME_SET_CPUUSED, cpu_used);
        
        int enable_auto_alt_ref = Data[121] & 1;
        aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    }
    
    // Set output buffer if we have enough data
    if (Size > 300) {
        aom_fixed_buf_t buf;
        size_t buf_size = 4096;
        buf.buf = malloc(buf_size);
        if (buf.buf) {
            buf.sz = buf_size;
            unsigned int pad_before = Data[200] % 16;
            unsigned int pad_after = Data[201] % 16;
            aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
            free(buf.buf);
        }
    }
    
    // Flush encoder
    aom_codec_encode(&ctx, NULL, 0, 1, 0);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process packet if needed
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame data available in pkt->data.frame.buf
            // Size in pkt->data.frame.sz
        }
    }
    
    // Clean up
    cleanup_image(&img);
    aom_codec_destroy(&ctx);
    
    return 0;
}