// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int try_fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (aom_codec_enc_config_default("libaom-av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings if size permits
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        // Fixed: g_time_base -> g_timebase
        cfg.g_timebase.num = (size > 2 ? data[2] : 1) % 100 + 1;
        cfg.g_timebase.den = (size > 3 ? data[3] : 1) % 100 + 1;
        cfg.g_threads = (size > 4 ? data[4] : 1) % 4 + 1;
        cfg.rc_target_bitrate = (size > 5 ? data[5] : 1) % 1000 + 100;
    }

    ret = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Try to invoke the target control functions
    // AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    aom_codec_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, 1);
    aom_codec_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, 0);
    if (size > 6) aom_codec_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, data[6]);

    // AV1E_SET_QM_U
    if (size > 8) aom_codec_control(&ctx, AV1E_SET_QM_U, data[8]);
    aom_codec_control(&ctx, AV1E_SET_QM_U, 0);
    aom_codec_control(&ctx, AV1E_SET_QM_U, 255);

    // AV1E_SET_MV_COST_UPD_FREQ
    if (size > 9) aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, data[9]);
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, 0);
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, 100);

    // AV1E_SET_FP_MT
    if (size > 10) aom_codec_control(&ctx, AV1E_SET_FP_MT, data[10]);
    aom_codec_control(&ctx, AV1E_SET_FP_MT, 0);
    aom_codec_control(&ctx, AV1E_SET_FP_MT, 1);

    // AV1E_SET_TIER_MASK
    if (size > 11) aom_codec_control(&ctx, AV1E_SET_TIER_MASK, data[11]);
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, 0);
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, 1);
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, 2);

    // Encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, cfg.g_w * cfg.g_h / 4);
    memset(img.planes[2], 128, cfg.g_w * cfg.g_h / 4);

    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 1;
}

static int try_fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    int ret;

    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Feed some data to the decoder
    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_codec_decode(&ctx, data, size, NULL);
        
        // Iterate through decoded frames
        // Fixed: aom_codec_get_frame takes only 2 arguments
        while (aom_codec_get_frame(&ctx, &iter) != NULL) {
            // Frame handling logic if needed, currently just iterating
        }
    }

    aom_codec_destroy(&ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_282(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Alternate between encoder and decoder fuzzing based on first byte
    if (Data[0] % 2 == 0) {
        try_fuzz_encoder(Data, Size);
    } else {
        try_fuzz_decoder(Data, Size);
    }

    return 0;
}