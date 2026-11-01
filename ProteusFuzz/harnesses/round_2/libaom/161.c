// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int get_int_from_data(const uint8_t *d, size_t s) {
    if (s < 4) return 0;
    int val = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    if (val < 0) val = 0;
    if (val > 100) val = 100;
    return val;
}

static int try_fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int idx = 0;
    int val;

    // AV1E_SET_DENOISE_BLOCK_SIZE
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val);
    }

    // AV1E_SET_NOISE_SENSITIVITY
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    }

    // AV1E_SET_ENABLE_RESTORATION
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        val = (val != 0) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, val);
    }

    // AOME_SET_SHARPNESS
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        aom_codec_control(ctx, AOME_SET_SHARPNESS, val);
    }

    // AV1E_SET_SKIP_POSTPROC_FILTERING
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        val = (val != 0) ? 1 : 0;
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
    }

    // AV1E_SET_DENOISE_NOISE_LEVEL
    if (idx + 4 <= size) {
        val = get_int_from_data(data + idx, size - idx);
        idx += 4;
        aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to tweak config slightly
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    try_fuzz_encoder(&ctx, Data + 4, Size - 4);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}