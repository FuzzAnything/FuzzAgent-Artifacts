// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"

static aom_image_t* create_test_image(void) {
    aom_image_t* img = calloc(1, sizeof(aom_image_t));
    if (!img) return NULL;
    
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    img->w = 64;
    img->h = 64;
    img->bit_depth = 8;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->sz = 64 * 64 * 3 / 2;
    img->bps = 8;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
    
    return img;
}

static void cleanup_test_image(aom_image_t* img) {
    if (img) {
        aom_img_remove_metadata(img);
        free(img);
    }
}

static aom_codec_ctx_t* create_test_codec_ctx(void) {
    aom_codec_ctx_t* ctx = calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    ctx->name = "test";
    ctx->iface = NULL;
    ctx->err = AOM_CODEC_OK;
    ctx->err_detail = NULL;
    ctx->init_flags = 0;
    ctx->config.raw = NULL;
    ctx->priv = NULL;
    
    return ctx;
}

static void cleanup_test_codec_ctx(aom_codec_ctx_t* ctx) {
    if (ctx) {
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    uint32_t mode = Data[0] % 4;
    uint32_t type = Data[1] | (Data[2] << 8) | (Data[3] << 16) | (Data[0] << 24);
    const uint8_t* payload = Data + 4;
    size_t payload_size = (Size > 4) ? Size - 4 : 0;
    
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    if (payload_size > 0) {
        uint8_t flag_val = payload[0] % 6;
        switch (flag_val) {
            case 0: insert_flag = AOM_MIF_NON_KEY_FRAME; break;
            case 1: insert_flag = AOM_MIF_KEY_FRAME; break;
            case 2: insert_flag = AOM_MIF_ANY_FRAME; break;
            case 3: insert_flag = AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC; break;
            case 4: insert_flag = AOM_MIF_KEY_FRAME_LAYER_SPECIFIC; break;
            case 5: insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC; break;
        }
    }
    
    switch (mode) {
        case 0: {
            aom_image_t* img = create_test_image();
            if (!img) break;
            
            int add_result = aom_img_add_metadata(img, type, payload, payload_size, insert_flag);
            
            if (add_result == 0) {
                size_t num_meta = aom_img_num_metadata(img);
                for (size_t i = 0; i < num_meta; i++) {
                    const aom_metadata_t* meta = aom_img_get_metadata(img, i);
                    if (meta) {
                        volatile uint32_t dummy = meta->type;
                        volatile size_t dummy2 = meta->sz;
                        (void)dummy;
                        (void)dummy2;
                    }
                }
            }
            
            aom_img_remove_metadata(img);
            cleanup_test_image(img);
            break;
        }
        
        case 1: {
            aom_metadata_t* meta = aom_img_metadata_alloc(type, payload, payload_size, insert_flag);
            if (meta) {
                volatile uint32_t dummy = meta->type;
                volatile size_t dummy2 = meta->sz;
                (void)dummy;
                (void)dummy2;
                free(meta->payload);
                free(meta);
            }
            break;
        }
        
        case 2: {
            aom_image_t* img = create_test_image();
            if (!img) break;
            
            for (int i = 0; i < 3; i++) {
                aom_img_add_metadata(img, type + i, payload, payload_size, insert_flag);
            }
            
            size_t num_meta = aom_img_num_metadata(img);
            for (size_t i = 0; i < num_meta; i++) {
                const aom_metadata_t* meta = aom_img_get_metadata(img, i);
                if (meta) {
                    volatile uint32_t dummy = meta->type;
                    (void)dummy;
                }
            }
            
            aom_img_remove_metadata(img);
            cleanup_test_image(img);
            break;
        }
        
        case 3: {
            aom_codec_ctx_t* ctx = create_test_codec_ctx();
            if (!ctx) break;
            
            aom_codec_iter_t iter = NULL;
            aom_image_t* frame = aom_codec_get_frame(ctx, &iter);
            if (frame) {
                volatile unsigned int dummy = frame->w;
                (void)dummy;
            }
            
            cleanup_test_codec_ctx(ctx);
            break;
        }
    }
    
    return 0;
}