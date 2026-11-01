// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int fuzz_encoder_controls(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int val;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Ensure we have data to read for control values
    if (size > 0) {
        val = data[0] & 0xFF;
        ret = aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
        (void)ret;
    }
    if (size > 1) {
        val = data[1] & 0xFF;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OBMC, val);
        (void)ret;
    }
    if (size > 2) {
        val = data[2] & 0xFF;
        ret = aom_codec_control(&ctx, AOME_SET_SHARPNESS, val);
        (void)ret;
    }
    if (size > 3) {
        val = data[3] & 0xFF;
        ret = aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);
        (void)ret;
    }
    if (size > 4) {
        val = data[4] & 0xFF;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
        (void)ret;
    }

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_controls(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int val;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize config manually
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Try to get bit depth
    val = 0;
    ret = aom_codec_control(&ctx, AV1D_GET_BIT_DEPTH, &val);
    (void)ret;

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz encoder controls
    fuzz_encoder_controls(Data, Size);

    // Fuzz decoder controls
    fuzz_decoder_controls(Data, Size);

    return 0;
}