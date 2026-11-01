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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    (void)err;
    return 1;
}

static int32_t read_int32(const uint8_t *data, size_t size, size_t offset) {
    if (size < offset + 4) return 0;
    int32_t val;
    memcpy(&val, data + offset, sizeof(int32_t));
    return val;
}

int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    int32_t val = read_int32(Data, Size, 2);

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val & 1);

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    int chroma_pos = val % 4;
    fuzz_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

    // Fuzz AV1E_SET_QUANT_B_ADAPT
    int quant_b_adapt = val & 1;
    fuzz_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, quant_b_adapt);

    // Fuzz AV1E_SET_TUNE_CONTENT
    int tune_content = val % 2;
    fuzz_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    int enable_rect_tx = val & 1;
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    int transfer_chars = val % 10;
    fuzz_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_chars);

    aom_codec_destroy(&ctx);
    return 0;
}