// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_CFL_INTRA
 * - AV1E_SET_ENABLE_ONESIDED_COMP
 * - AV1E_SET_ENABLE_SMOOTH_INTRA
 * - AV1E_SET_ENABLE_DUAL_FILTER
 * - AV1E_SET_ENABLE_FILTER_INTRA
 * - AV1E_SET_ENABLE_ANGLE_DELTA
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    int width, height;
    int ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    /* Derive width and height from input data to vary states */
    if (size >= 4) {
        width = (data[0] % 64) + 16;
        height = (data[1] % 64) + 16;
    } else {
        width = 16;
        height = 16;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Initialize a dummy image for encoding if needed later */
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);

    /* Encode a single frame to ensure encoder is active */
    aom_enc_frame_flags_t flags = 0;
    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));

    ret = aom_codec_encode(ctx, &img, 0, 1, flags);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    aom_img_free(&img);
    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /* Use input data to determine flag values (0 or 1) for each control */
    int val1 = (Size > 0) ? (Data[0] & 1) : 0;
    int val2 = (Size > 1) ? (Data[1] & 1) : 0;
    int val3 = (Size > 2) ? (Data[2] & 1) : 0;
    int val4 = (Size > 3) ? (Data[3] & 1) : 0;
    int val5 = (Size > 4) ? (Data[4] & 1) : 0;
    int val6 = (Size > 5) ? (Data[5] & 1) : 0;

    /* Invoke control functions with derived values */
    /* AV1E_SET_ENABLE_CFL_INTRA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val1);

    /* AV1E_SET_ENABLE_ONESIDED_COMP */
    aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val2);

    /* AV1E_SET_ENABLE_SMOOTH_INTRA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val3);

    /* AV1E_SET_ENABLE_DUAL_FILTER */
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val4);

    /* AV1E_SET_ENABLE_FILTER_INTRA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val5);

    /* AV1E_SET_ENABLE_ANGLE_DELTA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val6);

    /* Cleanup */
    cleanup_encoder(ctx);

    return 0;
}