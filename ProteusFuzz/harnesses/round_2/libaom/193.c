// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int val_int;
    aom_active_map_t active_map;

    // Initialize configuration
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic config from input to vary states
    cfg.g_w = (Data[0] % 256) + 16; // Minimum width 16
    cfg.g_h = (Data[1] % 256) + 16; // Minimum height 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Size > 2) ? (Data[2] * 1000) : 1000;

    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare active map for SET/GET commands if needed
    active_map.rows = (Size > 3) ? (Data[3] % 16) + 1 : 1;
    active_map.cols = (Size > 4) ? (Data[4] % 16) + 1 : 1;
    active_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (active_map.active_map == NULL) {
        cleanup_codec(&ctx);
        return 0;
    }
    memset(active_map.active_map, 0, active_map.rows * active_map.cols);

    // Fuzz 1: AV1E_SET_ENABLE_AB_PARTITIONS
    val_int = (Size > 5) ? (Data[5] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val_int);
    (void)ret;

    // Fuzz 2: AV1E_SET_INTER_DCT_ONLY
    val_int = (Size > 6) ? (Data[6] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val_int);
    (void)ret;

    // Fuzz 3: AOME_SET_ACTIVEMAP
    ret = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map);
    (void)ret;

    // Fuzz 4: AV1E_SET_ENABLE_MASKED_COMP
    val_int = (Size > 7) ? (Data[7] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, val_int);
    (void)ret;

    // Fuzz 5: AV1E_GET_ACTIVEMAP
    aom_active_map_t get_map;
    get_map.rows = active_map.rows;
    get_map.cols = active_map.cols;
    get_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (get_map.active_map != NULL) {
        ret = aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &get_map);
        (void)ret;
        free(get_map.active_map);
    }

    // Fuzz 6: AV1E_SET_ENABLE_INTERINTER_WEDGE
    val_int = (Size > 8) ? (Data[8] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val_int);
    (void)ret;

    // Cleanup
    free(active_map.active_map);
    cleanup_codec(&ctx);

    return 0;
}