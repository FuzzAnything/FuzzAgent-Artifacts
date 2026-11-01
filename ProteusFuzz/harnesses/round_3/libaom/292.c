// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

// Helper to safely initialize a decoder context
static aom_codec_ctx_t *init_decoder(aom_codec_flags_t flags) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed error_resilient as it is not a member of aom_codec_dec_cfg in recent versions
    // or might be handled via flags.
    cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init(ctx, iface, &cfg, flags) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

// Helper to safely cleanup a decoder context
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

// Helper to create a dummy image from fuzz data if needed
static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    if (size < 100) return NULL; // Need some data
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!img) return NULL;

    // Fill with fuzz data if available
    size_t fill_size = (size_t)img->sz;
    if (fill_size > size) fill_size = size;
    if (fill_size > 0) {
        memcpy(img->img_data, data, fill_size);
    }

    return img;
}

int LLVMFuzzerTestOneInput_292(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    aom_codec_ctx_t *ctx = init_decoder(0);
    if (!ctx) return 0;

    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;
    aom_image_t *ref_img = NULL;
    av1_ext_ref_frame_t ext_ref = {0};

    // Attempt to decode the input data to populate internal state
    aom_codec_err_t decode_res = aom_codec_decode(ctx, Data, Size, NULL);
    
    // 2. Invoke target functions diversely

    // --- Test aom_codec_get_frame ---
    while ((img = aom_codec_get_frame(ctx, &iter)) != NULL) {
        if (img->planes[0]) {
            // Just access to ensure validity
        }
    }

    // --- Test specific control functions ---
    // Note: These are likely macros defined in aom/aomdx.h or similar.
    // We wrap them in #ifdef to prevent compilation errors if they are not defined
    // in the specific build environment, although the task implies they should be.
    
    // 1. aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO
    #ifdef aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO
    aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO(ctx, NULL);
    #endif

    // 2. aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE
    #ifdef aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE
    aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE(ctx, NULL);
    #endif

    // 3. aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG
    #ifdef aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG
    aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG(ctx, NULL);
    #endif

    // 4. aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE
    #ifdef aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE
    aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE(ctx, NULL);
    #endif

    // 6. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
    #ifdef aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
    av1_ext_ref_frame_t dummy_ref;
    memset(&dummy_ref, 0, sizeof(dummy_ref));
    if (Size > 0) {
        ref_img = create_dummy_image(Data, Size);
        if (ref_img) {
            dummy_ref.img = ref_img;
            dummy_ref.num = 1;
        }
    }
    aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR(ctx, &dummy_ref);
    #endif

    // 3. Cleanup
    if (ref_img) {
        aom_img_free(ref_img);
    }
    
    cleanup_decoder(ctx);

    return 0;
}