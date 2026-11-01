// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_decoder.h"
#include "aom_encoder.h"
#include "aom_codec.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"

// Helper function to initialize a decoder context
static aom_codec_ctx_t* init_decoder(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to cleanup decoder context
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize decoder
    aom_codec_ctx_t *ctx = init_decoder();
    if (!ctx) return 0;

    // Allocate image for decoding
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1);

    // Feed data to decoder
    aom_codec_decode(ctx, (const uint8_t *)Data, Size, &img);

    // Attempt to invoke target control functions
    // aom_codec_control expects 'int' for ctrl_id, so we cast directly to int
    
    // 1. AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(ctx, (int)AV1D_GET_TILE_DATA, &tile_data);

    // 2. AV1_SET_DECODE_TILE_ROW
    int tile_row = (Data[0] % 64);
    aom_codec_control(ctx, (int)AV1_SET_DECODE_TILE_ROW, tile_row);

    // 3. AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(ctx, (int)AV1D_GET_TILE_COUNT, &tile_count);

    // 4. AV1_SET_DECODE_TILE_COL
    int tile_col = (Data[1] % 64);
    aom_codec_control(ctx, (int)AV1_SET_DECODE_TILE_COL, tile_col);

    // 5. AV1D_EXT_TILE_DEBUG
    int debug_flag = (Data[2] % 2);
    aom_codec_control(ctx, (int)AV1D_EXT_TILE_DEBUG, debug_flag);

    // 6. AV1D_GET_TILE_SIZE
    int tile_size = 0;
    aom_codec_control(ctx, (int)AV1D_GET_TILE_SIZE, &tile_size);

    // Cleanup
    aom_img_free(&img);
    cleanup_decoder(ctx);

    return 0;
}