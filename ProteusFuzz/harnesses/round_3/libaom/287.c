// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

// Helper macro to construct control ID if not defined in headers
#ifndef AOM_CODEC_CTRL_ID
#define AOM_CODEC_CTRL_ID(x) (x)
#endif

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val = 0;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // AV1E_ENABLE_EXT_TILE_DEBUG
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_ENABLE_EXT_TILE_DEBUG), val);

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_ENABLE_RECT_PARTITIONS), val);

    // AV1E_SET_ENABLE_PALETTE
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_ENABLE_PALETTE), val);

    // AV1E_SET_SINGLE_TILE_DECODING
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_SINGLE_TILE_DECODING), val);

    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1E_SET_ENABLE_INTRABC), val);

    return 1;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val = 0;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // AV1_SET_TILE_MODE (Decoder control)
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID(AV1_SET_TILE_MODE), val);

    return 1;
}

int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide encoder vs decoder
    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int init_success = 0;

    if (mode == 0) {
        // Encoder path
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

        // Fuzz some config parameters from payload
        if (payload_size >= 4) {
            uint32_t tmp;
            memcpy(&tmp, payload, 4);
            cfg.g_w = (tmp % 4096) + 16;
            cfg.g_h = ((tmp >> 16) % 4096) + 16;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = ((tmp >> 8) % 1000) + 1;
            cfg.rc_target_bitrate = (tmp % 1000000) + 1000;
        }

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res == AOM_CODEC_OK) {
            init_success = 1;
            fuzz_encoder_controls(&ctx, payload, payload_size);
        }
    } else {
        // Decoder path
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        // Fixed: Changed allow_highbitdepth to allow_lowbitdepth as per error message
        cfg.allow_lowbitdepth = 1;

        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res == AOM_CODEC_OK) {
            init_success = 1;
            fuzz_decoder_controls(&ctx, payload, payload_size);
        }
    }

    if (init_success) {
        aom_codec_destroy(&ctx);
    }

    return 0;
}