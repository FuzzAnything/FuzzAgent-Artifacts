// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <stdio.h>

#include "aom.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;

    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        // Ensure val is within reasonable bounds for boolean/flag settings
        val = (val != 0) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_AUTO_TILES, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) + 1) {
        memcpy(&val, data + sizeof(int), sizeof(int));
        val = (val != 0) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 2 + 1) {
        memcpy(&val, data + sizeof(int) * 2, sizeof(int));
        val = (val != 0) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 3 + 1) {
        memcpy(&val, data + sizeof(int) * 3, sizeof(int));
        val = (val != 0) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 4 + 1) {
        memcpy(&val, data + sizeof(int) * 4, sizeof(int));
        // Tile columns usually range from 1 to 64, clamp for safety
        if (val < 1) val = 1;
        if (val > 64) val = 64;
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    return AOM_CODEC_OK;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;

    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        val = (val != 0) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1_INVERT_TILE_DECODE_ORDER, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    return AOM_CODEC_OK;
}

int LLVMFuzzerTestOneInput_274(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Determine mode based on first byte
    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    memset(&ctx, 0, sizeof(ctx));

    if (mode == 0) {
        // Encoder Mode
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;

        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        res = fuzz_encoder_control(&ctx, payload, payload_size);
        
        aom_codec_destroy(&ctx);
        return 0;
    } else {
        // Decoder Mode
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;

        res = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        res = fuzz_decoder_control(&ctx, payload, payload_size);

        aom_codec_destroy(&ctx);
        return 0;
    }
}