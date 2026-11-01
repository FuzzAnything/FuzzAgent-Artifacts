// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

static int fuzz_encoder_control_get(aom_codec_ctx_t *ctx, int control_id, int *out_value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, out_value);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface;
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int palette_val = Data[0] & 0x01;
    int chroma_pos = (Data[1] & 0x03);
    int rect_part = (Data[0] >> 1) & 0x01;
    int transfer_char = (Data[0] >> 2) & 0x07;
    int global_motion = (Data[0] >> 5) & 0x01;
    int num_ops;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // AV1E_SET_ENABLE_PALETTE
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_PALETTE, palette_val);

    // AV1E_SET_CHROMA_SAMPLE_POSITION
    fuzz_encoder_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, rect_part);

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    fuzz_encoder_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, global_motion);

    // AV1E_GET_NUM_OPERATING_POINTS
    fuzz_encoder_control_get(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);

    aom_codec_destroy(&ctx);

    return 0;
}