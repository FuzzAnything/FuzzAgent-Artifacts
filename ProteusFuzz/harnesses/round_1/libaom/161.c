// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
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

static int init_decoder(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    if (!ctx || !iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;
    // Fixed: Removed invalid members, using valid ones if necessary or defaults
    // cfg.allow_lowbitdepth = 1; // Optional, not strictly required for init usually

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (!init_decoder(&ctx, iface)) {
        return 0;
    }

    // Fuzz AV1_SET_DECODE_TILE_COL
    if (Size > 0) {
        int col = (Data[0] % 64); // Reasonable tile column range
        // Fixed: Removed 'AOM_' prefix as per error message
        aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &col);
    }

    // Fuzz AV1_SET_DECODE_TILE_ROW
    if (Size > 1) {
        int row = (Data[1] % 64); // Reasonable tile row range
        aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, &row);
    }

    // Fuzz AV1D_GET_TILE_COUNT
    int tile_count = 0;
    aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

    // Fuzz AV1D_GET_TILE_SIZE
    aom_tile_data tile_size_info;
    memset(&tile_size_info, 0, sizeof(tile_size_info));
    aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, &tile_size_info);

    // Fuzz AV1D_GET_TILE_DATA
    aom_tile_data tile_data_info;
    memset(&tile_data_info, 0, sizeof(tile_data_info));
    aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data_info);

    // Fuzz AV1D_EXT_TILE_DEBUG
    // This often requires a specific struct or integer flag. 
    // We try passing an integer 1 to enable, or 0 to disable.
    if (Size > 2) {
        int debug_flag = (Data[2] > 128) ? 1 : 0;
        aom_codec_control(&ctx, AV1D_EXT_TILE_DEBUG, &debug_flag);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}