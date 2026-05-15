// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"

static aom_image_t* create_test_image(const uint8_t *data, size_t size) {
    // Create a simple test image with minimal valid parameters
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!img) return NULL;
    
    // Set some basic values
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = img->d_w = img->r_w = 16;
    img->h = img->d_h = img->r_h = 16;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->monochrome = 0;
    
    return img;
}

static aom_codec_ctx_t* create_test_codec_context(void) {
    // Create a minimal codec context for testing
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t*)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    // Initialize with some dummy values
    ctx->name = "test_codec";
    ctx->err = AOM_CODEC_OK;
    ctx->init_flags = 0;
    
    return ctx;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test 1: Image metadata functions with valid image
    aom_image_t *img = create_test_image(Data, Size);
    if (img) {
        // Test aom_img_num_metadata on image with no metadata (should return 0)
        size_t num_metadata = aom_img_num_metadata(img);
        
        // Test aom_img_get_metadata with various indices
        for (size_t i = 0; i < 5; i++) { // Test indices 0-4
            const aom_metadata_t *meta = aom_img_get_metadata(img, i);
            // Should return NULL since image has no metadata
        }
        
        // Test aom_img_remove_metadata on image with no metadata
        aom_img_remove_metadata(img);
        
        // Test aom_img_free
        aom_img_free(img);
    }
    
    // Test 2: Codec frame iterator functions
    aom_codec_ctx_t *ctx = create_test_codec_context();
    if (ctx) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *frame = NULL;
        
        // Test aom_codec_get_frame with NULL iterator
        frame = aom_codec_get_frame(ctx, &iter);
        
        // Test with non-NULL iterator (should return NULL since no real decoding happened)
        iter = (aom_codec_iter_t)Data; // Use fuzzer data as dummy iterator
        frame = aom_codec_get_frame(ctx, &iter);
        
        // Clean up
        free(ctx);
    }
    
    // Test 3: Additional edge cases with NULL inputs
    aom_img_get_metadata(NULL, 0);
    aom_img_num_metadata(NULL);
    aom_img_remove_metadata(NULL);
    aom_img_metadata_free(NULL);
    aom_img_free(NULL);
    
    // Test 4: Create and free metadata struct if we have enough data
    if (Size >= sizeof(aom_metadata_t)) {
        aom_metadata_t *meta = (aom_metadata_t*)calloc(1, sizeof(aom_metadata_t));
        if (meta) {
            // Set some values from fuzzer data
            if (Size >= sizeof(uint32_t)) {
                meta->type = *(uint32_t*)Data;
            }
            
            // Try to allocate payload if we have enough data
            size_t payload_size = Size % 256; // Limit size
            if (payload_size > 0) {
                meta->payload = (uint8_t*)malloc(payload_size);
                if (meta->payload && Size >= payload_size) {
                    memcpy(meta->payload, Data, payload_size);
                    meta->sz = payload_size;
                }
            }
            
            meta->insert_flag = AOM_MIF_ANY_FRAME;
            
            // Test aom_img_metadata_free
            aom_img_metadata_free(meta);
        }
    }
    
    return 0;
}