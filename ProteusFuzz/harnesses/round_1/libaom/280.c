// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

// Define CDEF_MAX_STRENGTHS if not defined in headers. 
// The crash indicates the library expects 16 entries (64 bytes for int array), 
// so we must ensure the allocation matches the library's internal expectation.
#ifndef CDEF_MAX_STRENGTHS
#define CDEF_MAX_STRENGTHS 16
#endif

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    // Attempt to decode some data to initialize state
    if (size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return 0;
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_image_t img;
    unsigned char *img_data = NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return -1;
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    // Initialize a dummy image to trigger encoding state
    memset(&img, 0, sizeof(img));
    img.w = 64;
    img.h = 64;
    img.stride[0] = 64;
    img.stride[1] = 32;
    img.stride[2] = 32;
    img.fmt = AOM_IMG_FMT_I420;
    img.bit_depth = 8;

    // Allocate memory for the image planes
    size_t plane_size = 64 * 64;
    img_data = (unsigned char *)malloc(plane_size * 3);
    if (!img_data) return -1;
    
    memset(img_data, 0, plane_size * 3);
    if (size > 0) {
        size_t copy_size = (size < plane_size) ? size : plane_size;
        memcpy(img_data, data, copy_size);
    }

    img.planes[0] = img_data;
    img.planes[1] = img_data + plane_size;
    img.planes[2] = img_data + plane_size * 2;

    // Encode one frame to ensure state is active
    aom_codec_encode(ctx, &img, 0, 1, 0);

    free(img_data);
    return 0;
}

int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    int quantizer;
    int64_t quantizer64;
    int base_q_idx;
    int *cdef_strengths = NULL;
    av1_ext_ref_frame_t ext_ref;

    memset(&ctx, 0, sizeof(ctx));
    memset(&ext_ref, 0, sizeof(ext_ref));

    // Determine if we should test encoder or decoder based on input size parity
    int is_encoder = (Size % 2) == 0;

    if (is_encoder) {
        if (init_encoder(&ctx, Data, Size) != 0) {
            return 0;
        }
    } else {
        if (init_decoder(&ctx, Data, Size) != 0) {
            return 0;
        }
    }

    // 1. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER
    if (is_encoder) {
        res = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
        (void)res;
    }

    // 2. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
    if (!is_encoder) {
        res = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
        (void)res;
    }

    // 3. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
    if (!is_encoder) {
        aom_image_t dummy_img;
        memset(&dummy_img, 0, sizeof(dummy_img));
        dummy_img.w = 64;
        dummy_img.h = 64;
        dummy_img.stride[0] = 64;
        dummy_img.planes[0] = (unsigned char *)Data;
        dummy_img.fmt = AOM_IMG_FMT_I420;
        dummy_img.bit_depth = 8;
        
        ext_ref.img = &dummy_img;
        ext_ref.num = 1;

        res = aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
        (void)res;
    }

    // 4. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64
    if (is_encoder) {
        res = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);
        (void)res;
    }

    // 5. aom_codec_control_typechecked_AV1E_GET_LUMA_CDEF_STRENGTH
    if (is_encoder) {
        // Fixed: Allocate based on the corrected CDEF_MAX_STRENGTHS (16) 
        // to match the library's internal memcpy size (64 bytes).
        size_t alloc_size = CDEF_MAX_STRENGTHS * sizeof(int);
        cdef_strengths = (int *)malloc(alloc_size);
        if (cdef_strengths) {
            memset(cdef_strengths, 0, alloc_size);
            res = aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
            (void)res;
        }
    }

    // 6. aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX
    if (!is_encoder) {
        res = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        (void)res;
    }

    if (cdef_strengths) {
        free(cdef_strengths);
    }

    cleanup_codec(&ctx);
    return 0;
}