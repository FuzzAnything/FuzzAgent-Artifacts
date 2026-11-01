// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use fuzzer data to configure some basic parameters
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[0] * Data[1]) + 100;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size_search = Data[0] % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    // Fuzz AV1E_SET_DELTALF_MODE
    int delta_lf_mode = Data[1] % 3;
    aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, delta_lf_mode);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = Data[2] % 4;
    aom_codec_control(ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int mode_cost_upd_freq = (Data[3] % 10) + 1;
    aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    int min_gf_interval = (Data[4] % 10) + 1;
    aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, min_gf_interval);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int cdf_update_mode = Data[5] % 2;
    aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);

    // Attempt to encode a dummy frame to exercise the codec state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fixed: Use 'stride' instead of 'd_stride'
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 0, img.stride[1] * ((cfg.g_h + 1) / 2));
    memset(img.planes[2], 0, img.stride[2] * ((cfg.g_h + 1) / 2));

    // Fixed: Use AOME_SET_CPUUSED and AOME_SET_STATIC_THRESHOLD
    aom_codec_control(ctx, AOME_SET_CPUUSED, 0);
    aom_codec_control(ctx, AOME_SET_STATIC_THRESHOLD, 0);

    // Fixed: Corrected argument count for aom_codec_encode (5 arguments)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_codec(ctx);

    return 0;
}