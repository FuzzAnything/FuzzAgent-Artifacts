// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int fuzz_codec_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return -1;
    int ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

static int fuzz_codec_control_get_int(aom_codec_ctx_t *ctx, int control_id) {
    if (!ctx) return -1;
    int value = 0;
    int ret = aom_codec_control(ctx, control_id, &value);
    (void)ret;
    (void)value;
    return 0;
}

static int fuzz_codec_control_render_size(aom_codec_ctx_t *ctx, int width, int height) {
    if (!ctx) return -1;
    aom_image_t size;
    memset(&size, 0, sizeof(size));
    size.d_w = width;
    size.d_h = height;
    int ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, &size);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_254(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    if (width > 8192 || height > 8192) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (Size > 2) {
        int val = Data[2];
        fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_SUPERRES, val);
    }
    if (Size > 3) {
        int val = Data[3];
        fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
    }
    if (Size > 4) {
        int val = Data[4];
        fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
    }
    if (Size > 5) {
        int val = Data[5];
        fuzz_codec_control_int(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    }

    if (Size > 7) {
        int render_w = Data[6] + 1;
        int render_h = Data[7] + 1;
        if (render_w > width || render_h > height) {
            render_w = width;
            render_h = height;
        }
        fuzz_codec_control_render_size(&ctx, render_w, render_h);
    }

    fuzz_codec_control_get_int(&ctx, AV1E_GET_SEQ_LEVEL_IDX);

    aom_codec_destroy(&ctx);

    return 0;
}