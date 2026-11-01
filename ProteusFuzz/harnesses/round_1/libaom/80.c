// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;

    // Attempt to invoke target control functions directly on the initialized context
    // 1. aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL
    int col_val = (Size > 0) ? (int)(Data[0] % 16) : 0;
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, col_val);

    // 2. aom_codec_control_typechecked_AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // 3. aom_codec_control_typechecked_AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);

    // 4. aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW
    int row_val = (Size > 1) ? (int)(Data[1] % 16) : 0;
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, row_val);

    // 5. aom_codec_control_typechecked_AV1D_GET_TILE_SIZE
    int tile_size = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size);

    // 6. aom_codec_control_typechecked_AV1D_EXT_TILE_DEBUG
    int debug_val = (Size > 2) ? (int)(Data[2]) : 0;
    aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &debug_val);

    // Try to decode with the fuzz data to trigger more states
    err = aom_codec_decode(&ctx, Data, Size, &iter);
    if (err == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet if needed, currently just iterating
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Optional: Access frame data if needed for further testing
                // aom_image_t *img = pkt->data.frame;
            }
        }
    }

    aom_codec_destroy(&ctx);

    return 0;
}