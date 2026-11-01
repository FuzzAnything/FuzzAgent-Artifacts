// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void safe_free_image(aom_image_t *img) {
    if (img != NULL) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    // Minimum size check to prevent out-of-bounds access
    if (Size < 9) return 0;

    // Extract parameters from fuzz data safely
    unsigned int d_w = Data[0] + ((unsigned int)Data[1] << 8);
    unsigned int d_h = Data[2] + ((unsigned int)Data[3] << 8);
    unsigned int align = (Data[4] % 65536) + 1;
    int plane_idx = (Data[5] % 3);
    int fmt_idx = (Data[6] % 10); // Limit to reasonable format indices
    int use_wrap = (Data[7] % 2);
    int use_codec = (Data[8] % 2);

    // Sanity checks for dimensions
    if (d_w == 0 || d_h == 0) {
        d_w = 16;
        d_h = 16;
    }
    if (d_w > 0x08000000) d_w = 0x08000000;
    if (d_h > 0x08000000) d_h = 0x08000000;

    // Map format index to actual enum
    aom_img_fmt_t fmts[] = {
        AOM_IMG_FMT_I420, AOM_IMG_FMT_YV12, AOM_IMG_FMT_I422, AOM_IMG_FMT_I444,
        AOM_IMG_FMT_NV12, AOM_IMG_FMT_I42016, AOM_IMG_FMT_YV1216, AOM_IMG_FMT_I42216,
        AOM_IMG_FMT_I44416, AOM_IMG_FMT_AOMI420
    };
    aom_img_fmt_t fmt = (fmt_idx < 10) ? fmts[fmt_idx] : AOM_IMG_FMT_I420;

    aom_image_t *img = NULL;
    unsigned char *user_buffer = NULL;

    // Test aom_img_alloc
    if (!use_wrap) {
        img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
        if (img != NULL) {
            // Test aom_img_plane_height
            int h = aom_img_plane_height(img, plane_idx);
            (void)h; // Use return value to suppress warnings

            // Test aom_img_num_metadata
            size_t meta_count = aom_img_num_metadata(img);
            (void)meta_count;

            // Cleanup
            safe_free_image(img);
            img = NULL;
        }
    } else {
        // Test aom_img_wrap
        // First, determine required size by wrapping a dummy pointer (1)
        aom_image_t dummy_img;
        aom_img_wrap(&dummy_img, fmt, d_w, d_h, align, (unsigned char *)1);
        
        // Calculate size based on dummy_img fields
        uint64_t align_x = (1 << dummy_img.x_chroma_shift) - 1;
        uint64_t w = (d_w + align_x) & ~align_x;
        uint64_t align_y = (1 << dummy_img.y_chroma_shift) - 1;
        uint64_t h = (d_h + align_y) & ~align_y;
        
        uint64_t s = (fmt & AOM_IMG_FMT_PLANAR) ? w : (uint64_t)dummy_img.bps * w / 8;
        s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s * 2 : s;
        s = (s + align - 1) & ~((uint64_t)align - 1);
        s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s / 2 : s;
        
        uint64_t alloc_size = (fmt & AOM_IMG_FMT_PLANAR) ? (uint64_t)h * s * dummy_img.bps / 8 : (uint64_t)h * s;

        if (alloc_size > 0 && alloc_size < (1ULL << 30)) {
            user_buffer = (unsigned char *)malloc(alloc_size);
            if (user_buffer != NULL) {
                img = aom_img_wrap(NULL, fmt, d_w, d_h, align, user_buffer);
                if (img != NULL) {
                    int h_val = aom_img_plane_height(img, plane_idx);
                    (void)h_val;
                    size_t meta_cnt = aom_img_num_metadata(img);
                    (void)meta_cnt;
                    
                    safe_free_image(img);
                }
                free(user_buffer);
            }
        }
    }

    // Test Codec Functions (aom_codec_get_frame, aom_codec_get_preview_frame)
    if (use_codec) {
        aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (ctx != NULL) {
            memset(ctx, 0, sizeof(aom_codec_ctx_t));
            
            // Initialize decoder context
            aom_codec_dec_cfg_t cfg;
            memset(&cfg, 0, sizeof(cfg));
            cfg.threads = 1;
            cfg.w = d_w;
            cfg.h = d_h;
            
            aom_codec_iface_t *iface = aom_codec_av1_dx();
            if (iface != NULL) {
                aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
                if (err == AOM_CODEC_OK) {
                    // Call get_frame (should return NULL if no frames decoded)
                    aom_codec_iter_t iter = NULL;
                    aom_image_t *frame = aom_codec_get_frame(ctx, &iter);
                    (void)frame;

                    // Call get_preview_frame (should return NULL for decoder or if no preview)
                    const aom_image_t *preview = aom_codec_get_preview_frame(ctx);
                    (void)preview;

                    // Cleanup decoder
                    aom_codec_destroy(ctx);
                }
            }
            
            // Try Encoder context for preview frame
            // Re-initialize ctx for encoder if needed, or use a new one. 
            // Here we reuse the same ctx pointer but ensure it's zeroed or destroyed first.
            // Since we destroyed it above, we need to ensure we don't double free or use invalid state.
            // Let's just try to init encoder on the same pointer if it was successfully destroyed.
            // Note: aom_codec_destroy sets ctx->priv to NULL usually, but let's be safe.
            memset(ctx, 0, sizeof(aom_codec_ctx_t));

            if (iface != NULL) {
                aom_codec_enc_cfg_t enc_cfg;
                aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
                enc_cfg.g_w = d_w;
                enc_cfg.g_h = d_h;
                enc_cfg.rc_target_bitrate = 1000;
                
                aom_codec_err_t err_enc = aom_codec_enc_init(ctx, aom_codec_av1_cx(), &enc_cfg, 0);
                if (err_enc == AOM_CODEC_OK) {
                    // Try to get preview frame from encoder (might be NULL if not encoded yet)
                    const aom_image_t *enc_preview = aom_codec_get_preview_frame(ctx);
                    (void)enc_preview;
                    
                    // Cleanup encoder
                    aom_codec_destroy(ctx);
                }
            }
            
            free(ctx);
        }
    }

    return 0;
}