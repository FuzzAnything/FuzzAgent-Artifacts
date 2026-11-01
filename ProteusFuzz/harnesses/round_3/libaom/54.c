// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_decoder.h"
#include "aom_encoder.h"
#include "aom_codec.h"
#include "aom_image.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    aom_codec_err_t res;
    
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // AV1D_GET_FRAME_SIZE
    res = aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, &img);
    (void)res;

    // AV1D_GET_DISPLAY_SIZE
    res = aom_codec_control(ctx, AV1D_GET_DISPLAY_SIZE, &img);
    (void)res;

    // AV1D_GET_IMG_FORMAT
    aom_img_fmt_t fmt = AOM_IMG_FMT_NONE;
    res = aom_codec_control(ctx, AV1D_GET_IMG_FORMAT, &fmt);
    (void)res;

    // AV1D_SET_OUTPUT_ALL_LAYERS
    int output_all = size > 0 ? data[0] % 2 : 0;
    res = aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, &output_all);
    (void)res;

    // AV1D_SET_OPERATING_POINT
    int op = size > 1 ? data[1] % 32 : 0;
    res = aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, &op);
    (void)res;

    return 0;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_ENABLE_FILTER_INTRA
    int enable_filter = size > 2 ? data[2] % 2 : 0;
    aom_codec_err_t res = aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, &enable_filter);
    (void)res;

    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 0;
    // Fixed: Removed invalid members 'allow_highbitdepth' and 'allow_hp'
    dec_cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        cleanup_codec(&dec_ctx);
        return 0;
    }

    aom_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&dec_ctx);
        return 0;
    }

    // Run decoder controls
    fuzz_decoder_controls(&dec_ctx, Data, Size);

    // Run encoder controls
    fuzz_encoder_controls(&enc_ctx, Data, Size);

    // Cleanup
    cleanup_codec(&dec_ctx);
    cleanup_codec(&enc_ctx);

    return 0;
}