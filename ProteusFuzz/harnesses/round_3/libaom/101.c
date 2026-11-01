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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_decode_init(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    
    // Initialize config manually
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    // Removed invalid fields: allow_high_bitdepth, allow_intrabc, allow_screen_content_tools
    // Only using valid fields present in the struct
    cfg.allow_lowbitdepth = 1;
    
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }

    return 1;
}

static void fuzz_decode_cleanup(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!fuzz_decode_init(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);

    // Fuzz AV1_SET_DECODE_TILE_ROW
    int32_t tile_row = (int32_t)(Data[0] % 100);
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &tile_row);

    // Fuzz AV1D_GET_TILE_COUNT
    int32_t tile_count = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // Fuzz AV1_SET_DECODE_TILE_COL
    int32_t tile_col = (int32_t)(Data[1] % 100);
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &tile_col);

    // Fuzz AV1D_EXT_TILE_DEBUG
    int32_t ext_tile_debug = (int32_t)(Data[2] % 2);
    aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &ext_tile_debug);

    // Fuzz AV1D_GET_TILE_SIZE
    aom_tile_data tile_size;
    memset(&tile_size, 0, sizeof(tile_size));
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);

    fuzz_decode_cleanup(&ctx);

    return 0;
}