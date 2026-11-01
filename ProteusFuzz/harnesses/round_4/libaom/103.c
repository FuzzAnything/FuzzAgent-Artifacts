// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure width/height within reasonable bounds
    if (size >= 2) {
        uint16_t w = (data[0] % 128) + 16;
        uint16_t h = (data[1] % 128) + 16;
        cfg->g_w = (w / 16) * 16;
        cfg->g_h = (h / 16) * 16;
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }

    cfg->g_threads = 1;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->rc_target_bitrate = 1000;

    res = aom_codec_enc_init(codec, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all control values
    if (Size < 6) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&codec, &cfg, Data, Size)) {
        return 0;
    }

    // Generate a dummy frame to ensure the encoder is in a valid state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill planes with data
    if (img.planes[0]) {
        memset(img.planes[0], 0, cfg.g_w * cfg.g_h);
    }
    if (img.planes[1]) {
        memset(img.planes[1], 128, (cfg.g_w / 2) * (cfg.g_h / 2));
    }
    if (img.planes[2]) {
        memset(img.planes[2], 128, (cfg.g_w / 2) * (cfg.g_h / 2));
    }

    aom_codec_control(&codec, AOME_SET_CPUUSED, 0);

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    // Expected: int value (0 or 1)
    int val1 = Data[0] % 2;
    aom_codec_control(&codec, AV1E_SET_ENABLE_1TO4_PARTITIONS, val1);

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    // Expected: int value (0, 1, or 2)
    int val2 = Data[1] % 3;
    aom_codec_control(&codec, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val2);

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    // Expected: int value (0 or 1)
    int val3 = Data[2] % 2;
    aom_codec_control(&codec, AV1E_SET_INTER_DCT_ONLY, val3);

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    // Expected: int value (0 or 1)
    int val4 = Data[3] % 2;
    aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, val4);

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    // Expected: int value (0 or 1)
    int val5 = Data[4] % 2;
    aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, val5);

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    // Expected: int value (0 or 1)
    int val6 = Data[5] % 2;
    aom_codec_control(&codec, AV1E_SET_ENABLE_ANGLE_DELTA, val6);

    // Attempt to encode a frame to exercise the state changes
    aom_codec_frame_flags_t enc_flags = 0;
    aom_codec_encode(&codec, &img, 0, 1, enc_flags);

    // Get output packets
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process encoded frame if needed
        }
    }

    // Cleanup
    aom_codec_destroy(&codec);
    aom_img_free(&img);

    return 0;
}