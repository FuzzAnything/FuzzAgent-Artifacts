// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

// Helper to initialize a dummy image for testing
static int init_dummy_image(aom_image_t *img, unsigned int w, unsigned int h) {
    if (!img) return 0;
    img->fmt = AOM_IMG_FMT_I420;
    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->stride[0] = w;
    img->stride[1] = (w + 1) / 2;
    img->stride[2] = (w + 1) / 2;
    
    // Allocate planes
    size_t y_size = w * h;
    size_t uv_size = ((w + 1) / 2) * ((h + 1) / 2);
    
    img->planes[0] = malloc(y_size);
    img->planes[1] = malloc(uv_size);
    img->planes[2] = malloc(uv_size);
    
    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        return 0;
    }
    
    memset(img->planes[0], 128, y_size);
    memset(img->planes[1], 128, uv_size);
    memset(img->planes[2], 128, uv_size);
    
    img->sz = y_size + 2 * uv_size;
    return 1;
}

// Helper to free a dummy image
static void free_dummy_image(aom_image_t *img) {
    if (!img) return;
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
}

// Helper to create and initialize a decoder context
static aom_codec_ctx_t *create_decoder_ctx(void) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cpu_used as it is not in the struct in newer versions
    
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

// Helper to create and initialize an encoder context
static aom_codec_ctx_t *create_encoder_ctx(void) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use data to determine which API to fuzz
    int mode = Data[0] % 6;
    
    aom_codec_ctx_t *ctx = NULL;
    aom_image_t img;
    av1_ext_ref_frame_t ext_ref;
    aom_s_frame_info s_frame_info;
    int quantizer = 0;
    int luma_cdef = 0;
    int last_ref_used = 0;
    
    // Initialize dummy image if needed
    if (init_dummy_image(&img, 64, 64) == 0) return 0;
    
    switch (mode) {
        case 0: // aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO
            ctx = create_decoder_ctx();
            if (ctx) {
                // Prepare a dummy packet to decode first to have a frame
                if (Size > 1) {
                    aom_codec_dec_cfg_t cfg;
                    memset(&cfg, 0, sizeof(cfg));
                    // Try to decode some data (likely invalid, but tests error handling)
                    aom_codec_decode(ctx, Data, Size, NULL);
                }
                
                memset(&s_frame_info, 0, sizeof(s_frame_info));
                aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
            
        case 1: // aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP
            ctx = create_encoder_ctx();
            if (ctx) {
                // Enable or disable based on data
                int enable = (Size > 1 && Data[1] > 127) ? 1 : 0;
                aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable);
                
                // Encode a frame to trigger usage
                aom_image_t enc_img;
                if (init_dummy_image(&enc_img, 64, 64)) {
                    aom_codec_encode(ctx, &enc_img, 0, 1, 0);
                    free_dummy_image(&enc_img);
                }
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
            
        case 2: // aom_codec_control_typechecked_AV1E_GET_LUMA_CDEF_STRENGTH
            ctx = create_encoder_ctx();
            if (ctx) {
                // Encode a frame first to have data
                aom_image_t enc_img;
                if (init_dummy_image(&enc_img, 64, 64)) {
                    aom_codec_encode(ctx, &enc_img, 0, 1, 0);
                    free_dummy_image(&enc_img);
                }
                
                aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &luma_cdef);
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
            
        case 3: // aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
            ctx = create_decoder_ctx();
            if (ctx) {
                // Setup external reference
                ext_ref.img = &img;
                ext_ref.num = 1;
                
                aom_codec_control(ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
                
                // Try to decode
                if (Size > 1) {
                    aom_codec_decode(ctx, Data, Size, NULL);
                }
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
            
        case 4: // aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED
            ctx = create_decoder_ctx();
            if (ctx) {
                // Decode a frame first
                if (Size > 1) {
                    aom_codec_decode(ctx, Data, Size, NULL);
                }
                
                aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
            
        case 5: // aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
            ctx = create_decoder_ctx();
            if (ctx) {
                // Decode a frame first
                if (Size > 1) {
                    aom_codec_decode(ctx, Data, Size, NULL);
                }
                
                aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
                
                aom_codec_destroy(ctx);
                free(ctx);
            }
            break;
    }
    
    free_dummy_image(&img);
    return 0;
}