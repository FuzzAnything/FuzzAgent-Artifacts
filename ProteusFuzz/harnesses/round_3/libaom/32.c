// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t *img;
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t res;
    int val;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder parameters
    if (size > 0) cfg.g_w = (data[0] % 256) + 16;
    if (size > 1) cfg.g_h = (data[1] % 256) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 256) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 256) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;

    res = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fill image with fuzzing data
    memset(img->planes[0], 128, img->d_w * img->d_h);
    memset(img->planes[1], 128, img->d_w * img->d_h / 4);
    memset(img->planes[2], 128, img->d_w * img->d_h / 4);

    // Invoke target control functions with fuzzed values
    if (size > 5) {
        val = data[5] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    }

    if (size > 6) {
        val = data[6] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    }

    if (size > 7) {
        val = data[7] % 100;
        aom_codec_control(&codec, AV1E_SET_MODE_COST_UPD_FREQ, val);
    }

    if (size > 8) {
        val = data[8] % 63;
        aom_codec_control(&codec, AOME_SET_CQ_LEVEL, val);
    }

    if (size > 9) {
        val = data[9] % 100;
        aom_codec_control(&codec, AOME_SET_MAX_INTRA_BITRATE_PCT, val);
    }

    if (size > 10) {
        val = data[10] % 2;
        aom_codec_control(&codec, AV1E_SET_BITRATE_ONE_PASS_CBR, val);
    }

    // Encode a frame. Fixed to 5 arguments: ctx, img, pts, duration, flags
    res = aom_codec_encode(&codec, img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // Drain the encoder
    // aom_codec_get_cx_data returns the packet pointer directly, not an error code
    while ((pkt = aom_codec_get_cx_data(&codec, NULL)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet
        }
    }

    aom_img_free(img);
    aom_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    fuzz_encoder(Data, Size);
    return 0;
}