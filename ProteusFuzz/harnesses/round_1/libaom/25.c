// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width = 64;
    int height = 64;

    if (size > 0 && data[0] < 256) {
        width = data[0];
    }
    if (size > 1 && data[1] < 256) {
        height = data[1];
    }

    if (width < 16 || height < 16) {
        width = 16;
        height = 16;
    }

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int initialized = 0;
    int palette_val, order_hint_val, reduced_ref_val, rtc_ext_rc_val;
    int seq_level_idx;
    int render_w, render_h;
    int render_size[2];

    if (Size < 6) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    initialized = init_encoder(&ctx, Data, Size);
    if (!initialized) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    palette_val = (Data[0] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, palette_val);

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    order_hint_val = (Data[1] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, order_hint_val);

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    reduced_ref_val = (Data[2] % 2);
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref_val);

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    rtc_ext_rc_val = (Data[3] % 2);
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_ext_rc_val);

    // Fuzz AV1E_SET_RENDER_SIZE
    // AV1E_SET_RENDER_SIZE expects a pointer to an array of 2 integers (width, height)
    render_w = (Data[4] % 256) + 16;
    render_h = (Data[5] % 256) + 16;
    render_size[0] = render_w;
    render_size[1] = render_h;
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_size);

    cleanup_encoder(&ctx);

    return 0;
}