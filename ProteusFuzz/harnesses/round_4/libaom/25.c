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

static int32_t get_int32(const uint8_t *d, size_t s, size_t *off) {
    if (*off + sizeof(int32_t) > s) return 0;
    int32_t val;
    memcpy(&val, d + *off, sizeof(int32_t));
    *off += sizeof(int32_t);
    return val;
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    size_t off = 0;
    int32_t val;

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
    }

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    }

    // AV1E_SET_ENABLE_INTRABC
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    // AV1E_SET_CHROMA_SAMPLE_POSITION
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
    }

    // AV1E_SET_ENABLE_CFL_INTRA
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    }

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    if (off < size) {
        val = get_int32(data, size, &off);
        aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Get the encoder interface first
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder config using the interface pointer, not the string name
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Set some basic config to ensure valid initialization
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}