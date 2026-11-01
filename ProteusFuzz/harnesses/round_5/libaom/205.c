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
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_integer.h"

/* 
 * Note: The provided function signatures in the prompt were empty.
 * This fuzzer assumes the standard libaom control command macros/definitions
 * which are typically defined in aom_codec.h or aomdx.h/aomcx.h.
 * We use the standard aom_codec_control() interface with the corresponding 
 * command constants.
 */

/* Fallback definitions for command IDs if not present in headers */
#ifndef AOME_SET_ENABLEAUTOALTREF
#define AOME_SET_ENABLEAUTOALTREF 0x00000001
#endif
#ifndef AV1D_SET_EXT_REF_PTR
#define AV1D_SET_EXT_REF_PTR 0x00000002
#endif
#ifndef AV1E_SET_ERROR_RESILIENT_MODE
#define AV1E_SET_ERROR_RESILIENT_MODE 0x00000003
#endif
#ifndef AV1_GET_REFERENCE
#define AV1_GET_REFERENCE 0x00000004
#endif
#ifndef AV1_SET_REFERENCE
#define AV1_SET_REFERENCE 0x00000005
#endif
#ifndef AV1_COPY_REFERENCE
#define AV1_COPY_REFERENCE 0x00000006
#endif

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder(aom_codec_ctx_t *ctx) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    /* aom_codec_dec_init takes the interface directly, not from cfg */
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    return 1;
}

static int init_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_205(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    
    if (Size < 1) return 0;

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));

    /* 
     * Strategy:
     * 1. Use the first byte to decide between Encoder and Decoder context.
     * 2. Use subsequent bytes to drive the parameters for the control commands.
     */
    
    int is_decoder = (Data[0] & 0x01);
    int is_encoder = (!is_decoder);
    
    if (is_decoder) {
        if (!init_decoder(&ctx)) {
            return 0;
        }
    } else if (is_encoder) {
        if (!init_encoder(&ctx)) {
            return 0;
        }
    } else {
        return 0;
    }

    /* 
     * Fuzzing the specific control commands.
     */

    /* 1. AOME_SET_ENABLEAUTOALTREF (Encoder only) */
    if (is_encoder && Size > 1) {
        int val = (int)(Data[1] & 0xFF);
        res = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, val);
        (void)res;
    }

    /* 2. AV1D_SET_EXT_REF_PTR (Decoder only) */
    if (is_decoder && Size > 2) {
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));
        
        ext_ref.num = (int)(Data[1] & 0xFF);
        
        if (ext_ref.num > 0 && ext_ref.num < 10) {
            static aom_image_t dummy_img;
            static uint8_t dummy_data[256];
            
            memset(&dummy_img, 0, sizeof(dummy_img));
            memset(dummy_data, 0, sizeof(dummy_data));
            
            dummy_img.w = 16;
            dummy_img.h = 16;
            dummy_img.planes[0] = dummy_data;
            dummy_img.stride[0] = 16;
            dummy_img.planes[1] = dummy_data + 16*16;
            dummy_img.stride[1] = 16;
            dummy_img.planes[2] = dummy_data + 16*16*2;
            dummy_img.stride[2] = 16;
            dummy_img.fmt = AOM_IMG_FMT_I420;
            
            ext_ref.img = &dummy_img;
        } else {
            ext_ref.num = 0;
            ext_ref.img = NULL;
        }

        res = aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
        (void)res;
    }

    /* 3. AV1E_SET_ERROR_RESILIENT_MODE (Encoder only) */
    if (is_encoder && Size > 3) {
        int val = (int)(Data[2] & 0xFF);
        res = aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val);
        (void)res;
    }

    /* 4. AV1_GET_REFERENCE (Decoder/Encoder) */
    if (Size > 4) {
        av1_ref_frame_t ref;
        memset(&ref, 0, sizeof(ref));
        
        ref.idx = (int)(Data[3] & 0xFF);
        ref.use_external_ref = (int)(Data[4] & 0xFF);
        
        static aom_image_t out_img;
        static uint8_t out_data[256];
        memset(&out_img, 0, sizeof(out_img));
        memset(out_data, 0, sizeof(out_data));
        out_img.w = 16; out_img.h = 16;
        out_img.planes[0] = out_data; out_img.stride[0] = 16;
        out_img.fmt = AOM_IMG_FMT_I420;
        
        ref.img = out_img;

        res = aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref);
        (void)res;
    }

    /* 5. AV1_SET_REFERENCE (Decoder/Encoder) */
    if (Size > 5) {
        av1_ref_frame_t ref;
        memset(&ref, 0, sizeof(ref));
        
        ref.idx = (int)(Data[4] & 0xFF);
        ref.use_external_ref = (int)(Data[5] & 0xFF);
        
        static aom_image_t in_img;
        static uint8_t in_data[256];
        memset(&in_img, 0, sizeof(in_img));
        memset(in_data, 0, sizeof(in_data));
        in_img.w = 16; in_img.h = 16;
        in_img.planes[0] = in_data; in_img.stride[0] = 16;
        in_img.fmt = AOM_IMG_FMT_I420;
        
        ref.img = in_img;

        res = aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref);
        (void)res;
    }

    /* 6. AV1_COPY_REFERENCE (Decoder/Encoder) */
    if (Size > 6) {
        av1_ref_frame_t ref;
        memset(&ref, 0, sizeof(ref));
        
        ref.idx = (int)(Data[5] & 0xFF);
        ref.use_external_ref = (int)(Data[6] & 0xFF);
        
        static aom_image_t copy_img;
        static uint8_t copy_data[256];
        memset(&copy_img, 0, sizeof(copy_img));
        memset(copy_data, 0, sizeof(copy_data));
        copy_img.w = 16; copy_img.h = 16;
        copy_img.planes[0] = copy_data; copy_img.stride[0] = 16;
        copy_img.fmt = AOM_IMG_FMT_I420;
        
        ref.img = copy_img;

        res = aom_codec_control(&ctx, AV1_COPY_REFERENCE, &ref);
        (void)res;
    }

    /* Cleanup */
    cleanup_codec(&ctx);
    
    return 0;
}