// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_343(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int32_t width = Data[0] + 16;
    int32_t height = Data[1] + 16;
    int32_t fps_num = Data[2] + 1;
    int32_t fps_den = Data[3] + 1;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int value = 0;
    if (Size > 4) {
        value = (Data[4] & 0x01);
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    if (Size > 5 && Data[5] & 0x01) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (Size > 6 && Data[6] & 0x01) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, value);
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (Size > 7 && Data[7] & 0x01) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, value);
    }

    // Fuzz AV1E_SET_QUANT_B_ADAPT
    if (Size > 8 && Data[8] & 0x01) {
        fuzz_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, value);
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 9 && Data[9] & 0x01) {
        fuzz_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, value);
    }

    // Fuzz AV1E_SET_TUNE_CONTENT
    if (Size > 10 && Data[10] & 0x01) {
        int tune_content = Data[10] & 0x03;
        fuzz_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);
    }

    aom_codec_destroy(&ctx);
    return 0;
}