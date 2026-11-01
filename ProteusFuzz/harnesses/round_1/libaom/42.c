// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use input data to determine configuration
    int width = (Data[0] % 32) + 16;
    int height = (Data[1] % 32) + 16;
    int fps_num = (Data[2 % Size] % 60) + 1;
    int fps_den = (Data[(2 + 1) % Size] % 60) + 1;
    int threads = (Data[(2 + 2) % Size] % 8) + 1;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = threads;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_lag_in_frames = (Data[(2 + 5) % Size] % 10);
    cfg.rc_target_bitrate = (Data[(2 + 6) % Size] * 256) + 1000;
    cfg.g_pass = 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);

    // Fill image with fuzzing data
    for (int i = 0; i < img.h * img.stride[0]; i++) {
        img.planes[0][i] = Data[i % Size];
    }
    // For I420, chroma planes are half height and half stride typically
    int chroma_h = img.h / 2;
    if (img.h > 0 && img.stride[1] > 0) {
        for (int i = 0; i < (img.h / 2) * img.stride[1]; i++) {
            img.planes[1][i] = Data[(i + 1) % Size];
        }
    }
    if (img.h > 0 && img.stride[2] > 0) {
        for (int i = 0; i < (img.h / 2) * img.stride[2]; i++) {
            img.planes[2][i] = Data[(i + 2) % Size];
        }
    }

    // Encode frame
    aom_enc_frame_flags_t flags = 0;
    if (Size > 10) flags = Data[10] % 4;

    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Extract packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process encoded data
            (void)pkt->data.frame.sz;
        }
    }

    // Fuzz control functions with various values
    int control_values[] = {0, 1, -1, 100, -100, 255, 128, -128, 65535, -65535};
    int num_controls = sizeof(control_values) / sizeof(control_values[0]);

    // AV1E_SET_COEFF_COST_UPD_FREQ
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, control_values[i]);
    }

    // AV1E_SET_ENABLE_FILTER_INTRA
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, control_values[i]);
    }

    // AV1E_SET_ENABLE_REF_FRAME_MVS
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, control_values[i]);
    }

    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, control_values[i]);
    }

    // AV1E_SET_RTC_EXTERNAL_RC
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, control_values[i]);
    }

    // AV1E_SET_FRAME_PERIODIC_BOOST
    for (int i = 0; i < num_controls; i++) {
        fuzz_encoder_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, control_values[i]);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}