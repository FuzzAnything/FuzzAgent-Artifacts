// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int initialize_encoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t err;
    int width = 64, height = 64;

    if (size >= 4) {
        width = *(const uint16_t *)(data);
        height = *(const uint16_t *)(data + 2);
    }

    /* Sanity checks for dimensions */
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;

    err = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int initialize_decoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_dec_cfg cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    /* Initialize config with default values manually if helper is missing or unavailable */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* Removed invalid members: allow_highbitdepth, allow_multiref, allow_screencontenttools */
    /* These are not present in aom_codec_dec_cfg in this version of libaom */

    err = aom_codec_dec_init(codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    int ret;
    
    /* Initialize codec context */
    memset(&codec, 0, sizeof(codec));

    /*
     * We need to test both encoder and decoder controls.
     * We use a simple heuristic from the input data to decide which to initialize.
     * If Size is even, try encoder; if odd, try decoder.
     * This ensures coverage of both code paths.
     */

    if (Size % 2 == 0) {
        /* Encoder Path */
        if (!initialize_encoder(&codec, Data, Size)) {
            return 0;
        }

        /*
         * Fuzzing the specific control constants.
         * Since the exact typechecked function signatures were empty in the prompt,
         * we use the standard aom_codec_control with the AV1 specific control IDs.
         * These IDs correspond to the functionality requested.
         */

        /* 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH */
        int enable_tx_search = (Size > 0) ? (Data[0] % 2) : 1;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_search);
        (void)ret;

        /* 3. AV1E_SET_ENABLE_DIFF_WTD_COMP */
        int enable_diff_wtd = (Size > 1) ? (Data[1] % 2) : 1;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd);
        (void)ret;

        /* 4. AV1E_SET_ENABLE_GLOBAL_MOTION */
        int enable_global_motion = (Size > 2) ? (Data[2] % 2) : 1;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
        (void)ret;

        /* 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY */
        int intra_default_tx = (Size > 3) ? (Data[3] % 2) : 1;
        ret = aom_codec_control(&codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx);
        (void)ret;

        /* 6. AV1E_SET_TUNE_CONTENT */
        int tune_content = (Size > 4) ? (Data[4] % 3) : 0; // 0, 1, or 2
        ret = aom_codec_control(&codec, AV1E_SET_TUNE_CONTENT, tune_content);
        (void)ret;

        cleanup_codec(&codec);

    } else {
        /* Decoder Path */
        if (!initialize_decoder(&codec, Data, Size)) {
            return 0;
        }

        /* 2. AV1D_SET_EXT_REF_PTR */
        /* This control requires a pointer to av1_ext_ref_frame_t */
        /* We construct a dummy reference frame structure using the input data */
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));

        /* Create a dummy image if we have enough data */
        if (Size >= 10) {
            aom_image_t dummy_img;
            memset(&dummy_img, 0, sizeof(dummy_img));
            dummy_img.w = (Data[0] % 64) + 1;
            dummy_img.h = (Data[1] % 64) + 1;
            dummy_img.planes[0] = (unsigned char *)Data + 2;
            dummy_img.stride[0] = dummy_img.w;
            dummy_img.fmt = AOM_IMG_FMT_I420;
            
            ext_ref.img = &dummy_img;
            ext_ref.num = (Data[2] % 4) + 1;
        } else {
            ext_ref.img = NULL;
            ext_ref.num = 0;
        }

        ret = aom_codec_control(&codec, AV1D_SET_EXT_REF_PTR, &ext_ref);
        (void)ret;

        cleanup_codec(&codec);
    }

    return 0;
}