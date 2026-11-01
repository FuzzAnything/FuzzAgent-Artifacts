// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

// Helper to initialize a decoder context
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid members allow_highbitdepth and allow_intrabc
    // cfg.allow_highbitdepth = 1; 
    // cfg.allow_intrabc = 1;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Feed some data to initialize internal state if needed
    if (data && size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return ctx;
}

// Helper to initialize an encoder context
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Create a dummy image to encode to warm up the context
    if (data && size > 0) {
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        img.fmt = AOM_IMG_FMT_I420;
        img.w = cfg.g_w;
        img.h = cfg.g_h;
        img.bit_depth = 8;
        
        // Allocate planes
        int plane_size = cfg.g_w * cfg.g_h;
        img.planes[0] = malloc(plane_size);
        img.planes[1] = malloc(plane_size / 4);
        img.planes[2] = malloc(plane_size / 4);
        img.stride[0] = cfg.g_w;
        img.stride[1] = cfg.g_w / 2;
        img.stride[2] = cfg.g_w / 2;

        if (img.planes[0] && img.planes[1] && img.planes[2]) {
            // Fill with fuzz data
            size_t copy_size = (size < plane_size) ? size : plane_size;
            memcpy(img.planes[0], data, copy_size);
            memset(img.planes[1], 0, plane_size / 4);
            memset(img.planes[2], 0, plane_size / 4);
            
            // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
            // Removed the 6th argument
            aom_codec_encode(ctx, &img, 0, 1, 0);
            
            free(img.planes[0]);
            free(img.planes[1]);
            free(img.planes[2]);
        }
    }

    return ctx;
}

static void cleanup_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_342(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz Decoder-related controls
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        int32_t val_int = 0;
        int64_t val_long = 0;

        // 2. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
        if (aom_codec_control(dec_ctx, AOMD_GET_LAST_QUANTIZER, &val_int) != AOM_CODEC_OK) {
            // Ignore errors, just testing reachability
        }

        // 6. aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX
        if (aom_codec_control(dec_ctx, AOMD_GET_BASE_Q_IDX, &val_int) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 3. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
        // Prepare external reference structure
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));
        
        // Create a dummy image for the reference
        aom_image_t dummy_img;
        memset(&dummy_img, 0, sizeof(dummy_img));
        dummy_img.fmt = AOM_IMG_FMT_I420;
        dummy_img.w = 64;
        dummy_img.h = 64;
        dummy_img.bit_depth = 8;
        dummy_img.planes[0] = malloc(64 * 64);
        dummy_img.planes[1] = malloc(64 * 64 / 4);
        dummy_img.planes[2] = malloc(64 * 64 / 4);
        dummy_img.stride[0] = 64;
        dummy_img.stride[1] = 32;
        dummy_img.stride[2] = 32;
        
        if (dummy_img.planes[0] && dummy_img.planes[1] && dummy_img.planes[2]) {
            ext_ref.img = &dummy_img;
            ext_ref.num = 1;
            if (aom_codec_control(dec_ctx, AV1D_SET_EXT_REF_PTR, &ext_ref) != AOM_CODEC_OK) {
                // Ignore errors
            }
            free(dummy_img.planes[0]);
            free(dummy_img.planes[1]);
            free(dummy_img.planes[2]);
        }

        cleanup_ctx(dec_ctx);
    }

    // Fuzz Encoder-related controls
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        int32_t val_int = 0;
        int64_t val_long = 0;
        int32_t cq_level = 0;

        // 1. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER
        if (aom_codec_control(enc_ctx, AOME_GET_LAST_QUANTIZER, &val_int) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 4. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64
        if (aom_codec_control(enc_ctx, AOME_GET_LAST_QUANTIZER_64, &val_long) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 5. aom_codec_control_typechecked_AOME_SET_CQ_LEVEL
        // Use fuzz data to set a random CQ level, clamped to reasonable range
        if (Size > 0) {
            cq_level = Data[0] % 64; // 0-63
        }
        if (aom_codec_control(enc_ctx, AOME_SET_CQ_LEVEL, &cq_level) != AOM_CODEC_OK) {
            // Ignore errors
        }

        cleanup_ctx(enc_ctx);
    }

    return 0;
}