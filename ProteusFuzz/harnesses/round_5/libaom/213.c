// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some configuration parameters safely
    if (size >= 2) {
        cfg->g_w = (data[0] % 512) + 16;
        cfg->g_h = (data[1] % 512) + 16;
    }
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&codec, &cfg, Data, Size)) {
        return 0;
    }

    // Create a dummy frame to pass to the encoder
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image data with fuzzing input if possible
    if (Size > 0) {
        size_t plane_size = img.planes[0][0] * img.stride[0];
        size_t copy_size = (Size < plane_size) ? Size : plane_size;
        memcpy(img.planes[0], Data, copy_size);
    }

    // Invoke the target API functions with fuzzing data
    aom_codec_control(&codec, AOME_SET_ARNR_STRENGTH, (int)(Data[0] % 10));
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, (int)(Data[1] % 2));
    aom_codec_control(&codec, AV1E_SET_QM_MIN, (int)(Data[2] % 64));
    aom_codec_control(&codec, AV1E_SET_AQ_MODE, (int)(Data[3] % 5));
    aom_codec_control(&codec, AOME_SET_ARNR_MAXFRAMES, (int)(Data[4] % 10));
    aom_codec_control(&codec, AV1E_SET_DELTAQ_STRENGTH, (int)(Data[5] % 10));

    // Encode the frame (fixed argument count: 5 arguments)
    aom_codec_encode(&codec, &img, 0, 1, 0);

    // Get packet
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
        }
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}