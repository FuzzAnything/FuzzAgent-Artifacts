// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aomdx.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx) return 0;

    // AV1E_SET_FP_MT_UNIT_TEST
    // Assuming it takes an int or similar flag based on naming convention
    if (Size >= sizeof(int)) {
        int val = *(const int *)Data;
        aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, val);
    }

    // AV1E_SET_POSTENCODE_DROP_RTC
    // Assuming it takes an int flag
    if (Size >= sizeof(int) + 1) {
        int val = *(const int *)(Data + sizeof(int));
        aom_codec_control(ctx, AV1E_SET_POSTENCODE_DROP_RTC, val);
    }

    // AV1_SET_REFERENCE
    // Requires av1_ref_frame_t
    if (Size >= sizeof(av1_ref_frame_t)) {
        av1_ref_frame_t ref;
        memcpy(&ref, Data, sizeof(av1_ref_frame_t));
        // Sanitize image pointer to avoid invalid memory access if fuzzer provides garbage
        ref.img.planes[0] = NULL;
        ref.img.planes[1] = NULL;
        ref.img.planes[2] = NULL;
        ref.img.stride[0] = 0;
        ref.img.stride[1] = 0;
        ref.img.stride[2] = 0;
        ref.img.img_data = NULL;
        ref.img.user_priv = NULL;
        ref.img.metadata = NULL;
        ref.img.fb_priv = NULL;
        
        aom_codec_control(ctx, AV1_SET_REFERENCE, &ref);
    }

    // AV1_COPY_REFERENCE
    // Requires av1_ref_frame_t
    if (Size >= sizeof(av1_ref_frame_t) * 2) {
        av1_ref_frame_t src_ref, dst_ref;
        memcpy(&src_ref, Data, sizeof(av1_ref_frame_t));
        memcpy(&dst_ref, Data + sizeof(av1_ref_frame_t), sizeof(av1_ref_frame_t));
        
        // Sanitize
        src_ref.img.planes[0] = NULL; src_ref.img.planes[1] = NULL; src_ref.img.planes[2] = NULL;
        dst_ref.img.planes[0] = NULL; dst_ref.img.planes[1] = NULL; dst_ref.img.planes[2] = NULL;
        
        aom_codec_control(ctx, AV1_COPY_REFERENCE, &src_ref);
        // Note: Copy reference might take src and dst, but API usually takes one struct or pointer. 
        // Based on typical libaom patterns, it might just take the source or a combined struct.
        // If the API expects a specific pointer type, the fuzzer data is passed as a pointer.
        // Here we try passing the address of the second struct as a potential target.
        aom_codec_control(ctx, AV1_COPY_REFERENCE, &dst_ref);
    }

    return 0;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx) return 0;

    // AV1D_SET_EXT_REF_PTR
    // Requires av1_ext_ref_frame_t
    if (Size >= sizeof(av1_ext_ref_frame_t)) {
        av1_ext_ref_frame_t ext_ref;
        memcpy(&ext_ref, Data, sizeof(av1_ext_ref_frame_t));
        
        // Sanitize: Ensure img pointer is valid or NULL
        // If fuzzer provides a pointer, it's likely invalid for the process. 
        // We must set it to NULL or a valid dummy image to prevent crash.
        ext_ref.img = NULL;
        ext_ref.num = 0;
        
        // If we want to test with a valid image, we'd need to allocate one, 
        // but for fuzzing control logic, NULL is often sufficient to test the setter.
        aom_codec_control(ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
    }

    // AV1_GET_REFERENCE
    // Requires av1_ref_frame_t
    if (Size >= sizeof(av1_ref_frame_t)) {
        av1_ref_frame_t ref;
        memset(&ref, 0, sizeof(ref));
        // Use fuzzer data to set idx and use_external_ref
        if (Size >= sizeof(int)) {
            ref.idx = *(const int *)Data;
        }
        if (Size >= sizeof(int) + sizeof(int)) {
            ref.use_external_ref = *(const int *)(Data + sizeof(int));
        }
        
        // Ensure image struct is clean
        ref.img.planes[0] = NULL;
        ref.img.planes[1] = NULL;
        ref.img.planes[2] = NULL;
        
        aom_codec_control(ctx, AV1_GET_REFERENCE, &ref);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Determine if we are fuzzing encoder or decoder based on first byte
    int is_encoder = (Data[0] % 2) == 0;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    memset(&ctx, 0, sizeof(ctx));

    if (is_encoder) {
        // Initialize Encoder
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        err = aom_codec_enc_config_default(iface, &cfg, 0);
        if (err != AOM_CODEC_OK) return 0;

        // Set some basic config to avoid immediate failure
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_encoder_control(&ctx, fuzz_data, fuzz_size);

    } else {
        // Initialize Decoder
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        // Fixed: Changed allow_highbitdepth to allow_lowbitdepth as per error message
        dec_cfg.allow_lowbitdepth = 1;

        err = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_decoder_control(&ctx, fuzz_data, fuzz_size);
    }

    cleanup_codec(&ctx);
    return 0;
}