// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to set some config parameters if possible
    if (size > 0) cfg.g_w = (uint32_t)data[0] + 16;
    if (size > 1) cfg.g_h = (uint32_t)data[1] + 16;
    if (size > 2) cfg.g_timebase.num = (uint32_t)data[2] + 1;
    if (size > 3) cfg.g_timebase.den = (uint32_t)data[3] + 1;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Try to invoke the target control functions with fuzzed data
    // Since signatures are unknown, we attempt to cast Data to expected types
    // These are best-effort attempts based on common patterns in libaom

    // 1. AV1E_SET_COEFF_COST_UPD_FREQ
    if (Size >= sizeof(int)) {
        int val = *(const int *)Data;
        aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    }

    // 2. AV1E_SET_TIMING_INFO_TYPE
    if (Size >= sizeof(int)) {
        int val = *(const int *)(Data + (Size % 4));
        aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, val);
    }

    // 3. AOME_SET_ENABLEAUTOBWDREF
    if (Size >= sizeof(int)) {
        int val = *(const int *)(Data + (Size % 4));
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    }

    // 4. AV1E_SET_DELTAQ_MODE
    if (Size >= sizeof(int)) {
        int val = *(const int *)(Data + (Size % 4));
        aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, val);
    }

    // 5. AOME_SET_CQ_LEVEL
    if (Size >= sizeof(int)) {
        int val = *(const int *)(Data + (Size % 4));
        aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val);
    }

    // 6. AV1E_SET_S_FRAME_MODE
    if (Size >= sizeof(int)) {
        int val = *(const int *)(Data + (Size % 4));
        aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, val);
    }

    // Attempt to encode a dummy frame to trigger more code paths
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    aom_codec_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}