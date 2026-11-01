// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/* Helper function to initialize encoder context */
static int init_encoder(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

/* Helper function to initialize decoder context */
static int init_decoder(aom_codec_ctx_t *ctx) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

/* Helper to generate dummy video data */
static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize contexts */
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    int enc_initialized = 0;
    int dec_initialized = 0;

    /* Prepare Encoder Configuration */
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) == AOM_CODEC_OK) {
        /* Fuzz configuration values with bounds checking */
        cfg.g_w = (Size > 0 ? Data[0] : 0) % 256 + 16;
        cfg.g_h = (Size > 1 ? Data[1] : 0) % 256 + 16;
        cfg.g_timebase.num = (Size > 2 ? Data[2] : 0) % 255 + 1;
        cfg.g_timebase.den = (Size > 3 ? Data[3] : 0) % 255 + 1;
        cfg.rc_target_bitrate = (Size > 4 ? Data[4] : 0) % 100 + 1;
        
        if (init_encoder(&enc_ctx, &cfg)) {
            enc_initialized = 1;
        }
    }

    /* Prepare Decoder */
    if (init_decoder(&dec_ctx)) {
        dec_initialized = 1;
    }

    /* Fuzz AV1E_SET_AUTO_TILES */
    if (enc_initialized) {
        int auto_tiles = (Size > 5 ? Data[5] : 0) % 2;
        aom_codec_control(&enc_ctx, AV1E_SET_AUTO_TILES, auto_tiles);
    }

    /* Fuzz AV1E_SET_TILE_ROWS */
    if (enc_initialized) {
        int tile_rows = (Size > 6 ? Data[6] : 0) % 16 + 1;
        aom_codec_control(&enc_ctx, AV1E_SET_TILE_ROWS, tile_rows);
    }

    /* Fuzz AV1E_SET_TILE_COLUMNS */
    if (enc_initialized) {
        int tile_cols = (Size > 7 ? Data[7] : 0) % 16 + 1;
        aom_codec_control(&enc_ctx, AV1E_SET_TILE_COLUMNS, tile_cols);
    }

    /* Fuzz AV1E_SET_SINGLE_TILE_DECODING */
    if (enc_initialized) {
        int single_tile = (Size > 8 ? Data[8] : 0) % 2;
        aom_codec_control(&enc_ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);
    }

    /* Fuzz AV1E_ENABLE_EXT_TILE_DEBUG */
    if (enc_initialized) {
        int ext_debug = (Size > 9 ? Data[9] : 0) % 2;
        aom_codec_control(&enc_ctx, AV1E_ENABLE_EXT_TILE_DEBUG, ext_debug);
    }

    /* Fuzz AV1D_GET_TILE_COUNT */
    if (dec_initialized) {
        int tile_count = 0;
        aom_codec_control(&dec_ctx, AV1D_GET_TILE_COUNT, &tile_count);
        (void)tile_count; /* Suppress unused warning */
    }

    /* Cleanup */
    if (enc_initialized) {
        aom_codec_destroy(&enc_ctx);
    }
    if (dec_initialized) {
        aom_codec_destroy(&dec_ctx);
    }

    /* Optional: Write dummy file if data is large enough to simulate file I/O usage */
    if (Size > 10) {
        write_dummy_file(Data, Size);
    }

    return 0;
}