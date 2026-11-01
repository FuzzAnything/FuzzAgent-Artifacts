// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int read_int(const uint8_t *d, size_t s, size_t *offset) {
    if (*offset + 4 > s) return 0;
    int val = d[*offset] | (d[*offset + 1] << 8) | (d[*offset + 2] << 16) | (d[*offset + 3] << 24);
    *offset += 4;
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret;
    int value;
    size_t offset = 0;

    if (!ctx) return 0;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_MASKED_COMP
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, value);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (offset < size) {
        value = read_int(data, size, &offset);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to tweak configuration slightly to explore states
    if (Size > 0) cfg.g_w = (Data[0] % 256) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 256) + 16;
    if (Size > 2) cfg.g_timebase.num = Data[2] + 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 100) + 100;
    if (Size > 5) cfg.g_threads = (Data[5] % 4) + 1;
    if (Size > 6) cfg.g_profile = Data[6] % 4;

    // Initialize the encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic for the target controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}