// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING at aomcx.h:2321:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DENOISE_NOISE_LEVEL at aomcx.h:2183:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AOME_GET_LOOPFILTER_LEVEL at aomcx.h:2300:1 in aomcx.h
// aom_codec_control_typechecked_AV1_SET_SKIP_LOOP_FILTER at aomdx.h:513:1 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t res;
    unsigned int width, height;

    if (!ctx || !data || size < 4) {
        return 0;
    }

    width = data[0] | (data[1] << 8);
    height = data[2] | (data[3] << 8);

    if (width == 0 || height == 0 || width > 8192 || height > 8192) {
        return 0;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
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
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_pass = 0;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_dec_cfg cfg;
    aom_codec_err_t res;

    if (!ctx || !data || size < 4) {
        return 0;
    }

    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int safe_read_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_318(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;
    int use_encoder;
    int loopfilter_level;

    if (Size < 1) {
        return 0;
    }

    use_encoder = safe_read_byte(Data, Size, 0) % 2;

    memset(&ctx, 0, sizeof(ctx));

    if (use_encoder) {
        if (!init_encoder(&ctx, Data, Size)) {
            return 0;
        }

        val = safe_read_byte(Data, Size, 1) % 2;
        ret = aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, (unsigned int)val);
        (void)ret;

        val = safe_read_byte(Data, Size, 2) % 256;
        ret = aom_codec_control_typechecked_AV1E_SET_DENOISE_NOISE_LEVEL(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, (int)val);
        (void)ret;

        val = safe_read_byte(Data, Size, 3) % 2;
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(&ctx, AV1E_SET_ENABLE_INTRABC, (int)val);
        (void)ret;

        val = safe_read_byte(Data, Size, 4) % 256;
        ret = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&ctx, AV1E_SET_LOOPFILTER_CONTROL, (int)val);
        (void)ret;

        ret = aom_codec_control_typechecked_AOME_GET_LOOPFILTER_LEVEL(&ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);
        (void)ret;

        cleanup_encoder(&ctx);
    } else {
        if (!init_decoder(&ctx, Data, Size)) {
            return 0;
        }

        val = safe_read_byte(Data, Size, 1) % 2;
        ret = aom_codec_control_typechecked_AV1_SET_SKIP_LOOP_FILTER(&ctx, AV1_SET_SKIP_LOOP_FILTER, (int)val);
        (void)ret;

        cleanup_decoder(&ctx);
    }

    return 0;
}