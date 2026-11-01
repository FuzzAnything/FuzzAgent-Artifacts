// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_external_partition.h"
#include "aom_integer.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    } else {
        val = 0;
    }

    // AV1E_SET_NOISE_SENSITIVITY
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);

    // AV1E_SET_AUTO_TILES
    aom_codec_control(ctx, AV1E_SET_AUTO_TILES, val);

    // AV1E_SET_SINGLE_TILE_DECODING
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);

    return 0;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    } else {
        val = 0;
    }

    // AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // AV1D_GET_TILE_SIZE
    int tile_size = 0;
    aom_codec_control(ctx, AV1D_GET_TILE_SIZE, &tile_size);

    // AV1D_EXT_TILE_DEBUG
    aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, val);

    return 0;
}

int LLVMFuzzerTestOneInput_303(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide encoder vs decoder
    int use_encoder = Data[0] & 1;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;

    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Create a dummy frame to encode to exercise state
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1);
        memset(img.planes[0], 128, 128 * 128);
        memset(img.planes[1], 128, 64 * 64);
        memset(img.planes[2], 128, 64 * 64);

        // Fixed: aom_codec_encode takes 5 arguments
        aom_codec_encode(&ctx, &img, 0, 1, 0);

        fuzz_encoder_controls(&ctx, fuzz_data, fuzz_size);

        aom_codec_destroy(&ctx);
        aom_img_free(&img);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        // Initialize config manually since cpu_used might not exist in all versions
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 0;
        // cfg.cpu_used = 0; // Removed to fix compilation error

        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        // Write dummy data to a file if needed
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(fuzz_data, 1, fuzz_size, f);
            fclose(f);
        }

        // Feed some data to decoder to exercise state
        // aom_codec_decode signature: aom_codec_decode(ctx, data, sz, frame)
        aom_codec_decode(&ctx, fuzz_data, fuzz_size, NULL);

        fuzz_decoder_controls(&ctx, fuzz_data, fuzz_size);

        aom_codec_destroy(&ctx);
    }

    return 0;
}