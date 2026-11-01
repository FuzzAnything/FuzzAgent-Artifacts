// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy input data for decoding
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    // Pass the fuzz input directly to the decoder
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Try to get frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        aom_img_free(img);
    }

    // Fuzz AV1_SET_DECODE_TILE_COL
    int col = (Data[0] % 64);
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &col);

    // Fuzz AV1_SET_DECODE_TILE_ROW
    int row = (Data[1] % 64);
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &row);

    // Fuzz AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // Fuzz AV1D_GET_TILE_SIZE
    int tile_size = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);

    // Fuzz AV1D_GET_TILE_DATA
    aom_tile_data tile_data = {0};
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);

    // Fuzz AV1D_EXT_TILE_DEBUG
    void *debug_info = NULL;
    aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &debug_info);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}