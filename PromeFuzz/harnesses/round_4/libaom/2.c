// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 64;
    static const unsigned int height = 64;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    // Fill image planes with fuzzer data
    size_t offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        unsigned int plane_width = (plane == 0) ? width : width / 2;
        unsigned int plane_height = (plane == 0) ? height : height / 2;
        
        for (unsigned int y = 0; y < plane_height; y++) {
            size_t row_bytes = plane_width;
            size_t bytes_to_copy = (size - offset) < row_bytes ? (size - offset) : row_bytes;
            
            if (bytes_to_copy > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], 
                       data + offset, bytes_to_copy);
                offset += bytes_to_copy;
            }
            
            // If we've consumed all fuzzer data, wrap around
            if (offset >= size) offset = 0;
        }
    }
    
    img->bit_depth = 8;
    img->range = AOM_CR_STUDIO_RANGE;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    
    return img;
}

static void fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Use fuzzer data to select control IDs and values
    uint32_t control_selector = *(const uint32_t *)data;
    size_t offset = 4;
    
    // Try different control IDs based on fuzzer input
    switch (control_selector % 10) {
        case 0:
            if (size - offset >= sizeof(int)) {
                int cpu_used = *(const int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_CPUUSED, cpu_used);
            }
            break;
        case 1:
            if (size - offset >= sizeof(int)) {
                int arnr_maxframes = *(const int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
            }
            break;
        case 2:
            if (size - offset >= sizeof(int)) {
                int enable_auto_alt_ref = *(const int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
            }
            break;
        case 3:
            if (size - offset >= sizeof(int)) {
                int arnr_strength = *(const int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
            }
            break;
        case 4:
            if (size - offset >= sizeof(unsigned int)) {
                unsigned int static_thresh = *(const unsigned int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_STATIC_THRESHOLD, static_thresh);
            }
            break;
        case 5:
            if (size - offset >= sizeof(int)) {
                int noise_sensitivity = *(const int *)(data + offset);
                aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
            }
            break;
        case 6:
            if (size - offset >= sizeof(int)) {
                int denoise_level = *(const int *)(data + offset);
                aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, denoise_level);
            }
            break;
        case 7:
            if (size - offset >= sizeof(int)) {
                int tile_columns = *(const int *)(data + offset);
                aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, tile_columns);
            }
            break;
        case 8:
            if (size - offset >= sizeof(int)) {
                int tile_rows = *(const int *)(data + offset);
                aom_codec_control(ctx, AV1E_SET_TILE_ROWS, tile_rows);
            }
            break;
        case 9:
            if (size - offset >= sizeof(int)) {
                int cq_level = *(const int *)(data + offset);
                aom_codec_control(ctx, AOME_SET_CQ_LEVEL, cq_level);
            }
            break;
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
    if (size < 16) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;
    
    // Initialize with default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some configuration parameters from fuzzer data
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Apply some encoder controls based on fuzzer data
    fuzz_encoder_controls(&ctx, data, size);
    
    // Create test image
    aom_image_t *img = create_test_image(data, size);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Prepare output buffer
    uint8_t output_buffer[4096];
    aom_fixed_buf_t buf;
    buf.buf = output_buffer;
    buf.sz = sizeof(output_buffer);
    
    // Set output buffer with random padding from fuzzer data
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    if (size >= 8) {
        pad_before = *(const unsigned int *)data % 64;
        pad_after = *(const unsigned int *)(data + 4) % 64;
    }
    aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
    
    // Encode frames with varying parameters from fuzzer data
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    // Use fuzzer data to determine encoding parameters
    if (size >= 12) {
        duration = *(const unsigned long *)(data + 8) % 100 + 1;
        flags = *(const aom_enc_frame_flags_t *)(data + 12) & 0xFFFF;
    }
    
    // Encode first frame
    aom_codec_encode(&ctx, img, pts, duration, flags);
    
    // Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process different packet types
        switch (pkt->kind) {
            case AOM_CODEC_CX_FRAME_PKT:
                // Frame data available in pkt->data.frame.buf
                break;
            case AOM_CODEC_STATS_PKT:
                // Stats data available
                break;
            case AOM_CODEC_PSNR_PKT:
                // PSNR data available
                break;
            default:
                break;
        }
    }
    
    // Encode more frames or flush based on fuzzer data
    if (size >= 16) {
        uint32_t frame_count = *(const uint32_t *)(data + 16) % 4;
        
        for (uint32_t i = 0; i < frame_count; i++) {
            pts += duration;
            aom_codec_encode(&ctx, img, pts, duration, flags);
            
            // Get encoded data
            iter = NULL;
            while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
                // Just iterate through packets
            }
        }
    }
    
    // Flush encoder
    aom_codec_encode(&ctx, NULL, pts, duration, flags);
    
    // Get remaining data
    iter = NULL;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Process remaining packets
    }
    
    // Test with NULL buffer (restore default behavior)
    aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}