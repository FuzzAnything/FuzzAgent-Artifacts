// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure encoder settings safely
    if (size >= 2) {
        cfg.g_w = 16 + (data[0] % 1024);
        cfg.g_h = 16 + (data[1] % 1024);
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (size % 10000);
    cfg.g_lag_in_frames = 0;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Prepare dummy image
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    
    // Fixed: Use 'planes' instead of 'plane'
    if (img.planes[0]) memset(img.planes[0], 0, img.stride[0] * img.d_h);
    if (img.planes[1]) memset(img.planes[1], 128, img.stride[1] * img.d_h / 2);
    if (img.planes[2]) memset(img.planes[2], 128, img.stride[2] * img.d_h / 2);

    // Fuzz control commands
    int cmd_args[6];
    for (int i = 0; i < 6; i++) {
        cmd_args[i] = (size > i) ? (int)data[i] % 100 : 0;
    }

    // AOME_SET_ARNR_STRENGTH
    ret = aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, cmd_args[0]);
    (void)ret;

    // AOME_SET_MAX_INTRA_BITRATE_PCT
    ret = aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, cmd_args[1]);
    (void)ret;

    // AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, cmd_args[2] % 2);
    (void)ret;

    // AV1E_SET_DENOISE_NOISE_LEVEL
    ret = aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, cmd_args[3] % 100);
    (void)ret;

    // AOME_SET_ARNR_MAXFRAMES
    ret = aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, cmd_args[4] % 50);
    (void)ret;

    // AOME_SET_SHARPNESS
    ret = aom_codec_control(&ctx, AOME_SET_SHARPNESS, cmd_args[5] % 10);
    (void)ret;

    // Encode one frame
    // Fixed: aom_codec_encode expects 5 arguments (ctx, img, pts, duration, flags)
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        aom_img_free(&img);
        return 0;
    }

    // Drain packets
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
        }
    }

    aom_codec_destroy(&ctx);
    aom_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    return fuzz_encoder(Data, Size);
}