// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int auto_tiles = 0;
    int tile_rows = 0;
    int single_tile = 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz configuration parameters
    if (size > 0) cfg.g_w = data[0] + 16;
    if (size > 1) cfg.g_h = data[1] + 16;
    if (size > 2) cfg.g_timebase.num = data[2] + 1;
    if (size > 3) cfg.g_timebase.den = data[3] + 1;
    if (size > 4) cfg.g_threads = data[4] % 16 + 1;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz AV1E_SET_AUTO_TILES
    if (size > 5) auto_tiles = data[5] ? 1 : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_AUTO_TILES, auto_tiles);
    (void)ret;

    // Fuzz AV1E_SET_TILE_ROWS
    if (size > 6) tile_rows = data[6] % 16;
    ret = aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
    (void)ret;

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    if (size > 7) single_tile = data[7] ? 1 : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);
    (void)ret;

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int tile_count = 0;
    int tile_size = 0;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz AV1D_GET_TILE_COUNT
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    (void)ret;

    // Fuzz AV1D_GET_TILE_SIZE
    ret = aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);
    (void)ret;

    // Fuzz AV1D_EXT_TILE_DEBUG
    // This control typically takes a pointer to a structure or specific flags.
    // We pass a dummy value based on input to trigger different paths.
    int debug_flag = 0;
    if (size > 8) debug_flag = data[8] ? 1 : 0;
    ret = aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, debug_flag);
    (void)ret;

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Randomly choose between encoder and decoder fuzzing paths
    if (Data[0] % 2 == 0) {
        fuzz_encoder(Data, Size);
    } else {
        fuzz_decoder(Data, Size);
    }

    return 0;
}