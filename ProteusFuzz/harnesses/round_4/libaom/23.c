// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for potential file-based operations
    write_dummy_file(Data, Size);

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    // Initialize config manually with valid members for the current libaom version
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1; 
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Decode a frame to ensure context is valid for control calls
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    // Feed some data to the decoder
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Try to get a frame to ensure state is active
    img = aom_codec_get_frame(&ctx, &iter);
    (void)img; // Suppress unused warning

    // Fuzz various control IDs related to tiles
    // Using AOM_CODEC_CONTROL macro directly as type-checked macros are not always available in all builds
    // We cast arguments appropriately based on expected types for these controls

    // 1. AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // 2. AOMD_GET_TILE_INFO
    aom_tile_info tile_info;
    memset(&tile_info, 0, sizeof(tile_info));
    aom_codec_control(&ctx, AOMD_GET_TILE_INFO, &tile_info);

    // 3. AV1D_GET_TILE_SIZE
    int tile_size = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);

    // 4. AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);

    // 5. AV1D_EXT_TILE_DEBUG (if supported, might require specific flags or be no-op)
    // This often requires a pointer to a structure or specific data, passing a dummy pointer
    int debug_data = 0;
    aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &debug_data);

    // Attempt to decode again with different data to explore states
    if (Size > 10) {
        aom_codec_decode(&ctx, Data + 10, Size - 10, NULL);
        aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}