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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;
    // Removed cpu_used as it is not a member of aom_codec_dec_cfg in newer versions
    // cfg.cpu_used = 0; 

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy bitstream packet from input data
    aom_codec_iter_t iter = NULL;
    
    // Ensure we have some data to decode, even if minimal
    size_t data_len = Size;
    const uint8_t *data_ptr = Data;

    // Decode one frame to set up internal state for tile queries
    aom_codec_decode(&ctx, data_ptr, data_len, &iter);

    // Try to get decoded frames to ensure internal state is populated
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame decoded successfully, state is ready for control queries
            break;
        }
    }

    // Fuzz Control 1: AV1D_GET_TILE_SIZE
    int tile_size_result = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size_result);

    // Fuzz Control 2: AV1_INVERT_TILE_DECODE_ORDER
    int invert_flag = (Size > 0) ? (Data[0] & 0x1) : 0;
    aom_codec_control(&ctx, AV1_INVERT_TILE_DECODE_ORDER, &invert_flag);

    // Fuzz Control 3: AV1_SET_DECODE_TILE_ROW
    int tile_row = (Size > 1) ? (Data[1] % 16) : 0;
    aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &tile_row);

    // Fuzz Control 4: AOMD_GET_TILE_INFO
    aom_tile_info tile_info;
    memset(&tile_info, 0, sizeof(tile_info));
    aom_codec_control(&ctx, AOMD_GET_TILE_INFO, &tile_info);

    // Fuzz Control 5: AV1D_GET_FRAME_SIZE
    int frame_dims[2] = {0, 0};
    aom_codec_control(&ctx, AV1D_GET_FRAME_SIZE, frame_dims);

    // Fuzz Control 6: AV1D_GET_TILE_DATA
    aom_tile_data tile_data;
    memset(&tile_data, 0, sizeof(tile_data));
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}