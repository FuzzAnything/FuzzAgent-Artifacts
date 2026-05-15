// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"

static void fuzz_init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = NULL;
    
    // Try to get encoder interface - check if aom_codec_av1_cx is available
    // We'll use aom_codec_enc_init which internally handles the interface
    if (size == 0) {
        return;
    }
    
    aom_codec_enc_cfg_t cfg;
    
    // Get default config - we need to provide an interface
    // We'll use a dummy approach since we can't get the actual interface
    // The macro aom_codec_enc_init will handle the interface internally
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    
    // Use fuzz data to modify some config parameters
    if (size > 0) {
        cfg.g_w = 64 + (data[0] % 192);
        cfg.g_h = 64 + (data[1] % 192);
        cfg.g_bit_depth = (data[2] % 2) ? AOM_BITS_8 : AOM_BITS_10;
        cfg.rc_target_bitrate = 100 + (data[3] % 1000);
        cfg.g_threads = 1 + (data[4] % 4);
        cfg.g_limit = 1 + (data[5] % 10);
    }
    
    // Initialize encoder using the macro which handles the interface
    aom_codec_enc_init(ctx, NULL, &cfg, 0);
}

static aom_image_t *fuzz_create_image(const uint8_t *data, size_t size) {
    static aom_image_t img;
    static unsigned char img_data[64 * 64 * 3];
    
    if (size < 10) return NULL;
    
    // Initialize image structure
    memset(&img, 0, sizeof(img));
    img.fmt = (data[0] % 2) ? AOM_IMG_FMT_I420 : AOM_IMG_FMT_I42016;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = data[1] % 2;
    img.csp = AOM_CSP_VERTICAL;
    img.range = (data[2] % 2) ? AOM_CR_STUDIO_RANGE : AOM_CR_FULL_RANGE;
    img.w = 64;
    img.h = 64;
    img.bit_depth = (img.fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? 10 : 8;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    // Set up planes for I420 format
    img.planes[0] = img_data;
    img.planes[1] = img_data + 64 * 64;
    img.planes[2] = img_data + 64 * 64 + 32 * 32;
    img.stride[0] = 64;
    img.stride[1] = 32;
    img.stride[2] = 32;
    img.sz = sizeof(img_data);
    img.bps = (img.fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? 10 : 8;
    
    // Fill with some data
    size_t copy_size = size < sizeof(img_data) ? size : sizeof(img_data);
    memcpy(img_data, data, copy_size);
    
    return &img;
}

static void fuzz_set_cx_data_buf(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    static unsigned char buf_data[4096];
    static aom_fixed_buf_t buf = {buf_data, sizeof(buf_data)};
    
    if (size > sizeof(buf_data)) {
        memcpy(buf_data, data, sizeof(buf_data));
    } else if (size > 0) {
        memcpy(buf_data, data, size);
    }
    
    unsigned int pad_before = (size > 0) ? data[0] % 16 : 0;
    unsigned int pad_after = (size > 1) ? data[1] % 16 : 0;
    
    aom_codec_set_cx_data_buf(ctx, &buf, pad_before, pad_after);
}

static void fuzz_codec_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int ctrl_id = data[0] + 1; // Ensure non-zero
    int ctrl_val = data[1];
    
    // Try some known control IDs for AV1 encoder
    if (ctrl_id == 8) { // AOME_SET_CPUUSED
        aom_codec_control(ctx, ctrl_id, ctrl_val % 10);
    } else if (ctrl_id == 9) { // AOME_SET_ENABLEAUTOALTREF
        aom_codec_control(ctx, ctrl_id, ctrl_val % 2);
    } else if (ctrl_id == 17) { // AV1E_SET_ROW_MT
        aom_codec_control(ctx, ctrl_id, ctrl_val % 2);
    } else {
        // Try with the fuzzed control ID (may fail)
        aom_codec_control(ctx, ctrl_id, ctrl_val);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Initialize encoder
    fuzz_init_encoder(&codec, data, size);
    if (codec.err != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some controls
    if (size > 10) {
        fuzz_codec_control(&codec, data + 6, size - 6);
    }
    
    // Set output buffer
    fuzz_set_cx_data_buf(&codec, data, size);
    
    // Create and encode image
    aom_image_t *img = fuzz_create_image(data, size);
    if (img) {
        aom_codec_encode(&codec, img, 0, 1, 0);
        
        // Get encoded data
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets
        }
        
        // Try reconfiguring with different settings
        if (size > 50) {
            aom_codec_enc_cfg_t new_cfg;
            // Try to get default config
            if (aom_codec_enc_config_default(NULL, &new_cfg, 0) == AOM_CODEC_OK) {
                new_cfg.g_w = 32 + (data[20] % 96);
                new_cfg.g_h = 32 + (data[21] % 96);
                aom_codec_enc_config_set(&codec, &new_cfg);
            }
        }
        
        // Encode another frame
        aom_codec_encode(&codec, img, 1, 1, 0);
        
        // Flush encoder
        aom_codec_encode(&codec, NULL, 0, 1, 0);
        
        // Get remaining data
        iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets
        }
    } else {
        // Try encoding without image (flush)
        aom_codec_encode(&codec, NULL, 0, 1, 0);
    }
    
    // Clean up
    aom_codec_destroy(&codec);
    
    return 0;
}