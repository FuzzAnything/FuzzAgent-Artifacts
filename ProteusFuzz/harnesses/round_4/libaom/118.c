// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
 * - AV1E_SET_MAX_PARTITION_SIZE
 * - AV1E_SET_LOOPFILTER_CONTROL
 * - AV1E_SET_SVC_FRAME_DROP_MODE
 * - AV1E_SET_GF_CBR_BOOST_PCT
 * - AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
 */

static int fuzz_encode_init(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int width, height;

    if (size < 4) return -1;

    /* Extract width and height from input data */
    width = data[0] + 1;
    height = data[1] + 1;

    /* Ensure reasonable bounds to avoid OOM or invalid states */
    if (width > 4096 || height > 4096 || width == 0 || height == 0) {
        width = 64;
        height = 64;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR; /* CBR mode required for some controls */
    cfg.g_threads = 1;
    /* Removed cfg.g_row_mt as it does not exist in all libaom versions */

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

static void fuzz_encode_cleanup(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static void fuzz_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    (void)ret; /* Ignore return for fuzzing state exploration */
}

static void fuzz_control_int64(aom_codec_ctx_t *ctx, int control_id, int64_t value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    int ret;
    int control_id;
    int val_int;
    int64_t val_int64;
    unsigned int val_uint;
    int loopfilter_mode;
    int svc_mode;
    int gf_boost_pct;
    int max_drop_ms;
    int max_drop_cnt;
    int max_partition;

    if (Size < 1) return 0;

    /* Initialize encoder context */
    ret = fuzz_encode_init(&ctx, Data, Size);
    if (ret != 0) return 0;

    /* Use input data to select control and generate values */
    control_id = Data[0] % 6; /* 0 to 5 for 6 controls */

    if (Size >= 2) {
        val_int = Data[1];
    } else {
        val_int = 0;
    }

    if (Size >= 3) {
        val_int64 = (int64_t)Data[2];
    } else {
        val_int64 = 0;
    }

    if (Size >= 4) {
        val_uint = Data[3];
    } else {
        val_uint = 0;
    }

    /* Map control_id to actual control constants and call */
    switch (control_id) {
        case 0: /* AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR */
            max_drop_ms = (int)val_int64;
            fuzz_control_int64(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, max_drop_ms);
            break;

        case 1: /* AV1E_SET_MAX_PARTITION_SIZE */
            max_partition = (int)val_uint;
            fuzz_control_int(&ctx, AV1E_SET_MAX_PARTITION_SIZE, max_partition);
            break;

        case 2: /* AV1E_SET_LOOPFILTER_CONTROL */
            /* This control typically takes a pointer to a struct or int */
            loopfilter_mode = val_int % 4; /* Assume 0-3 modes */
            fuzz_control_int(&ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_mode);
            break;

        case 3: /* AV1E_SET_SVC_FRAME_DROP_MODE */
            svc_mode = val_int % 4;
            fuzz_control_int(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, svc_mode);
            break;

        case 4: /* AV1E_SET_GF_CBR_BOOST_PCT */
            gf_boost_pct = val_int % 101; /* 0-100 */
            fuzz_control_int(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, gf_boost_pct);
            break;

        case 5: /* AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR */
            max_drop_cnt = val_int;
            fuzz_control_int(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop_cnt);
            break;

        default:
            break;
    }

    /* Cleanup */
    fuzz_encode_cleanup(&ctx);

    return 0;
}