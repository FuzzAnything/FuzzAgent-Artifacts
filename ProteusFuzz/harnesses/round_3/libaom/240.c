// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, int width, int height) {
    if (aom_codec_enc_config_default("av1", cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(codec, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int ret = 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    // Extract parameters from fuzz input
    uint32_t width = Data[0] + 16; // Ensure minimum size
    uint32_t height = Data[1] + 16;
    uint32_t param_val = Data[2] * 256 + Data[3];

    // Limit dimensions to reasonable values to avoid OOM
    if (width > 4096 || height > 4096) {
        return 0;
    }

    if (init_encoder(&codec, &cfg, width, height) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    err = aom_codec_control(&codec, AV1E_SET_MAX_GF_INTERVAL, param_val);
    if (err != AOM_CODEC_OK) {
        ret = 1; // Mark as error state exploration
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    err = aom_codec_control(&codec, AV1E_SET_NOISE_SENSITIVITY, param_val);
    if (err != AOM_CODEC_OK) {
        ret = 1;
    }

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    err = aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, param_val);
    if (err != AOM_CODEC_OK) {
        ret = 1;
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    err = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, param_val ? 1 : 0);
    if (err != AOM_CODEC_OK) {
        ret = 1;
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    if (Size >= 100) {
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        
        // Initialize SVC params from fuzz data
        svc_params.number_spatial_layers = (Data[4] % 3) + 1;
        svc_params.number_temporal_layers = (Data[5] % 3) + 1;
        
        for (int i = 0; i < AOM_MAX_LAYERS && i < 10; i++) {
            svc_params.max_quantizers[i] = Data[6 + i] % 64;
            svc_params.min_quantizers[i] = Data[16 + i] % 64;
            svc_params.layer_target_bitrate[i] = (Data[26 + i] * 256 + Data[36 + i]) + 100;
        }
        
        for (int i = 0; i < AOM_MAX_SS_LAYERS && i < 10; i++) {
            svc_params.scaling_factor_num[i] = Data[46 + i] % 100;
            svc_params.scaling_factor_den[i] = (Data[56 + i] % 99) + 1;
        }
        
        for (int i = 0; i < AOM_MAX_TS_LAYERS && i < 10; i++) {
            svc_params.framerate_factor[i] = Data[66 + i] % 10;
        }

        err = aom_codec_control(&codec, AV1E_SET_SVC_PARAMS, &svc_params);
        if (err != AOM_CODEC_OK) {
            ret = 1;
        }
    }

    // Try to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(&img.planes[0][0], 0x80, width * height);
    memset(&img.planes[1][0], 0x80, (width * height) / 4);
    memset(&img.planes[2][0], 0x80, (width * height) / 4);

    aom_codec_encode(&codec, &img, 0, 1, 0);
    aom_img_free(&img);

    cleanup_codec(&codec);

    return ret;
}