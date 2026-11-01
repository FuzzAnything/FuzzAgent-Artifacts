// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;

    // Initialize config with default values
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Note: cpu_used and other flags removed as they are not in this version's struct

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Initialize the decoder context
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Feed the input data to the decoder
    res = aom_codec_decode(&ctx, Data, Size, NULL);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Attempt to decode frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // --- Fuzz Control Functions ---
        
        // 1. AV1D_GET_TILE_SIZE
        int tile_size[2];
        aom_codec_control(&ctx, AV1D_GET_TILE_SIZE, tile_size);

        // 2. AV1_SET_DECODE_TILE_COL
        int col = (Size > 1) ? (int)Data[1] % 64 : 0;
        aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, col);

        // 3. AV1_SET_DECODE_TILE_ROW
        int row = (Size > 2) ? (int)Data[2] % 64 : 0;
        aom_codec_control(&ctx, AV1_SET_DECODE_TILE_ROW, row);

        // 4. AV1_SET_TILE_MODE
        int mode = (Size > 3) ? (int)Data[3] % 4 : 0;
        aom_codec_control(&ctx, AV1_SET_TILE_MODE, mode);

        // 5. AV1D_GET_TILE_COUNT
        int tile_count = 0;
        aom_codec_control(&ctx, AV1D_GET_TILE_COUNT, &tile_count);

        // 6. AV1D_GET_TILE_DATA
        aom_tile_data tile_data;
        memset(&tile_data, 0, sizeof(tile_data));
        aom_codec_control(&ctx, AV1D_GET_TILE_DATA, &tile_data);
        
        // Cleanup frame image
        aom_img_free(img);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    
    // Write dummy file as per requirements if data is substantial
    if (Size > 100) {
        setup_dummy_file(Data, Size);
    }

    return 0;
}