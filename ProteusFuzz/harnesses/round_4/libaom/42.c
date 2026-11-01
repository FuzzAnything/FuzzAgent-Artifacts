// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec != NULL) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;

    // Try to initialize as encoder first
    // Use aom_codec_av1_cx() as the standard AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface != NULL) {
        err = aom_codec_enc_config_default(iface, &cfg, 0);
        if (err == AOM_CODEC_OK) {
            cfg.g_w = Data[0] ? Data[0] : 64;
            cfg.g_h = Data[1] ? Data[1] : 64;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.g_threads = 1;
            cfg.g_lag_in_frames = 0;
            cfg.rc_target_bitrate = 1000;

            err = aom_codec_enc_init(&codec, iface, &cfg, 0);
            if (err == AOM_CODEC_OK) {
                // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
                int level_idx = Data[2] % 10; // Limit to reasonable range
                aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);

                // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
                int enable_ref_mvs = (Data[3] % 2) ? 1 : 0;
                aom_codec_control(&codec, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_mvs);

                // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
                int allow_warped = (Data[4] % 2) ? 1 : 0;
                aom_codec_control(&codec, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped);

                // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
                int enable_filter = (Data[5] % 2) ? 1 : 0;
                aom_codec_control(&codec, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter);

                // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
                int enable_wedge = (Data[6] % 2) ? 1 : 0;
                aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_wedge);

                // Attempt to encode a dummy frame
                aom_image_t img;
                aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
                memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
                memset(img.planes[1], 128, (cfg.g_w * cfg.g_h) / 4);
                memset(img.planes[2], 128, (cfg.g_w * cfg.g_h) / 4);

                aom_enc_frame_flags_t flags = 0;
                // Corrected call: 5 arguments
                aom_codec_encode(&codec, &img, 0, 1, flags);

                aom_img_free(&img);
            }
            cleanup_codec(&codec);
        }
    }

    // Try to initialize as decoder
    iface = aom_codec_av1_dx();
    if (iface != NULL) {
        aom_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        // Fixed: use allow_lowbitdepth instead of allow_highbitdepth
        dec_cfg.allow_lowbitdepth = 1;

        err = aom_codec_dec_init(&codec, iface, &dec_cfg, 0);
        if (err == AOM_CODEC_OK) {
            // Fuzz AV1_SET_DECODE_TILE_COL
            int tile_cols = (Data[7] % 8) + 1; // 1 to 8
            aom_codec_control(&codec, AV1_SET_DECODE_TILE_COL, tile_cols);

            // Attempt to decode dummy data
            // Just feed some data to see if it handles it gracefully
            aom_codec_decode(&codec, Data, Size, NULL);

            cleanup_codec(&codec);
        }
    }

    return 0;
}