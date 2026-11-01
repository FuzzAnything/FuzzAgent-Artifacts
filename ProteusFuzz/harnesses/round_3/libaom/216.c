// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_TIER_MASK
 * - AV1E_SET_FORCE_VIDEO_MODE
 * - AV1E_SET_FP_MT
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_SUPERBLOCK_SIZE
 * - AV1E_SET_MTU
 */

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data to initialize configuration safely */
    if (Size < 20) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t *img = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t res;

    /* Get AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Initialize configuration */
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    /* Use fuzzing data to set some config parameters safely */
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 8) + 1;
    cfg.g_profile = (Data[6] % 4);
    cfg.g_usage = (Data[7] % 3);
    cfg.rc_end_usage = (Data[8] % 3);

    /* Initialize the encoder */
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    /* Create a dummy image frame */
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    /* Fill image data with fuzzing input if available */
    size_t plane_size = img->d_w * img->d_h;
    size_t total_size = plane_size * 3; /* Y, U, V */
    size_t copy_size = (Size - 20 < total_size) ? (Size - 20) : total_size;
    if (copy_size > 0) {
        memcpy(img->planes[0], Data + 20, copy_size);
    }

    /*
     * Fuzz various control functions.
     * We map random bytes to control IDs and values.
     */
    /* AV1E_SET_TIER_MASK */
    int tier_mask = Data[10] & 0x3; /* 0, 1, or 2 */
    try_control(&ctx, AV1E_SET_TIER_MASK, tier_mask);

    /* AV1E_SET_FORCE_VIDEO_MODE */
    int force_video = (Data[11] % 2);
    try_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video);

    /* AV1E_SET_FP_MT */
    int fp_mt = (Data[12] % 2);
    try_control(&ctx, AV1E_SET_FP_MT, fp_mt);

    /* AV1E_SET_RTC_EXTERNAL_RC */
    int rtc_ext = (Data[13] % 2);
    try_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_ext);

    /* AV1E_SET_SUPERBLOCK_SIZE */
    /* Valid values are typically 64 or 128 */
    int sb_size = (Data[14] % 2) ? 128 : 64;
    try_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);

    /* AV1E_SET_MTU */
    /* MTU is typically a value between 1 and 1000 */
    int mtu = (Data[15] % 1000) + 1;
    try_control(&ctx, AV1E_SET_MTU, mtu);

    /* Encode the frame */
    res = aom_codec_encode(&ctx, img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        /* Continue to cleanup even on error */
    }

    /* Collect packets */
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
        /* Do nothing with the packet, just consume it */
    }

    /* Cleanup */
    aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}