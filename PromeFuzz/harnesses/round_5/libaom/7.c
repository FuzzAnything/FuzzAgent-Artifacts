// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_image.h"
#include "aom/aom_decoder.h"

static void dummy_image_init(aom_image_t *img) {
    memset(img, 0, sizeof(*img));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = img->d_w = img->r_w = 64;
    img->h = img->d_h = img->r_h = 64;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    
    // Don't allocate metadata array - let API functions handle it
    img->metadata = NULL;
}

static void dummy_image_cleanup(aom_image_t *img) {
    if (img) {
        aom_img_remove_metadata(img);
    }
}

static void dummy_codec_ctx_init(aom_codec_ctx_t *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->name = "dummy";
    ctx->err = AOM_CODEC_OK;
    ctx->init_flags = 0;
    ctx->config.raw = NULL;
    
    // Don't allocate private data - use NULL
    ctx->priv = NULL;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Use first byte to determine test path
    uint8_t test_case = data[0];
    size_t offset = 1;
    
    // Test aom_img_metadata_alloc
    if (offset < size) {
        uint32_t type = 0;
        if (offset + sizeof(uint32_t) <= size) {
            memcpy(&type, data + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        
        size_t payload_size = 0;
        if (size > offset) {
            payload_size = size - offset;
            if (payload_size > 1024) payload_size = 1024; // Limit size
        }
        
        const uint8_t* payload = NULL;
        if (payload_size > 0 && offset < size) {
            payload = data + offset;
        }
        
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
        if (test_case % 3 == 0) insert_flag = AOM_MIF_KEY_FRAME;
        else if (test_case % 3 == 1) insert_flag = AOM_MIF_NON_KEY_FRAME;
        else if (test_case % 3 == 2) insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC;
        
        aom_metadata_t *metadata = aom_img_metadata_alloc(type, payload, payload_size, insert_flag);
        
        // Test aom_img_add_metadata and related functions
        aom_image_t img;
        dummy_image_init(&img);
        
        if (metadata) {
            // Add metadata using the allocated struct's data
            int add_result = aom_img_add_metadata(&img, metadata->type, 
                                                 metadata->payload, metadata->sz,
                                                 metadata->insert_flag);
            
            // Test num_metadata
            size_t num_meta = aom_img_num_metadata(&img);
            
            // Test get_metadata for valid indices
            for (size_t i = 0; i < num_meta && i < 10; i++) {
                const aom_metadata_t *retrieved = aom_img_get_metadata(&img, i);
                (void)retrieved; // Use result
            }
            
            // Test get_metadata with out-of-bounds index
            const aom_metadata_t *out_of_bounds = aom_img_get_metadata(&img, num_meta + 1);
            (void)out_of_bounds;
            
            // Test remove_metadata
            aom_img_remove_metadata(&img);
            
            // Test remove_metadata again (should be safe)
            aom_img_remove_metadata(&img);
            
            // Clean up allocated metadata
            if (metadata->payload) free(metadata->payload);
            free(metadata);
        }
        
        // Also test add_metadata directly with fuzzer data
        if (payload_size > 0 && offset < size) {
            uint32_t direct_type = test_case;
            size_t direct_sz = payload_size;
            if (direct_sz > 512) direct_sz = 512;
            
            aom_img_add_metadata(&img, direct_type, payload, direct_sz, insert_flag);
            
            // Test all metadata functions again
            size_t num = aom_img_num_metadata(&img);
            for (size_t i = 0; i < num && i < 5; i++) {
                const aom_metadata_t *md = aom_img_get_metadata(&img, i);
                (void)md;
            }
        }
        
        dummy_image_cleanup(&img);
    }
    
    // Test aom_codec_get_frame with dummy context
    aom_codec_ctx_t ctx;
    dummy_codec_ctx_init(&ctx);
    
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame; // Should be NULL
    
    // Test with non-NULL iterator
    uintptr_t dummy_iter = 0x1234;
    iter = (aom_codec_iter_t)&dummy_iter;
    frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    return 0;
}