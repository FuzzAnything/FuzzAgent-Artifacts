// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE at aomcx.h:2165:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_ARNR_STRENGTH at aomcx.h:1911:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DENOISE_NOISE_LEVEL at aomcx.h:2183:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_SHARPNESS at aomcx.h:1896:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_DISABLE_TRELLIS_QUANT at aomcx.h:2029:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_ARNR_MAXFRAMES at aomcx.h:1908:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    // Use the correct function name: aom_codec_av1_cx()
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use fuzzing data to tweak config slightly to ensure valid initialization
    if (Size > 1) cfg.g_w = (Data[0] % 1024) + 16;
    if (Size > 2) cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    size_t offset = 0;
    int val;

    // Fuzz AV1E_SET_DELTALF_MODE (expects unsigned int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE(ctx, AV1E_SET_DELTALF_MODE, (unsigned int)val);
    }

    // Fuzz AOME_SET_ARNR_STRENGTH (expects unsigned int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AOME_SET_ARNR_STRENGTH(ctx, AOME_SET_ARNR_STRENGTH, (unsigned int)val);
    }

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL (expects int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_DENOISE_NOISE_LEVEL(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val);
    }

    // Fuzz AOME_SET_SHARPNESS (expects unsigned int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AOME_SET_SHARPNESS(ctx, AOME_SET_SHARPNESS, (unsigned int)val);
    }

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT (expects unsigned int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_DISABLE_TRELLIS_QUANT(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, (unsigned int)val);
    }

    // Fuzz AOME_SET_ARNR_MAXFRAMES (expects unsigned int)
    if (offset + sizeof(int) <= Size) {
        val = extract_int(Data, Size, &offset);
        aom_codec_control_typechecked_AOME_SET_ARNR_MAXFRAMES(ctx, AOME_SET_ARNR_MAXFRAMES, (unsigned int)val);
    }

    cleanup_codec(ctx);
    return 0;
}