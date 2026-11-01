// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int ret = 0;

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (size >= 1) {
        int val = data[0] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    if (size >= 2) {
        int val = data[1] % 22; // Limit to reasonable range for transfer characteristics
        ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
        (void)ret;
    }

    // Fuzz AV1E_GET_ACTIVEMAP
    if (size >= 3) {
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        ret = aom_codec_control(ctx, AV1E_GET_ACTIVEMAP, &active_map);
        (void)ret;
        // Cleanup if active_map allocated memory (implementation dependent, usually caller allocates)
        // Here we assume the struct is filled but memory ownership depends on implementation
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (size >= 4) {
        int val = data[3] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (size >= 5) {
        int val = data[4] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)ret;
    }

    // Fuzz AOME_SET_ACTIVEMAP
    if (size >= 6) {
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        // Generate a dummy active map based on input size to test boundary conditions
        unsigned int rows = (data[5] % 100) + 1;
        unsigned int cols = (size > 6 ? data[6] % 100 : 1) + 1;
        
        // Ensure we don't allocate too much in fuzzing
        if (rows > 100 || cols > 100) {
            rows = 100;
            cols = 100;
        }

        active_map.rows = rows;
        active_map.cols = cols;
        active_map.active_map = (unsigned char *)malloc(rows * cols);
        
        if (active_map.active_map) {
            // Fill with random data from input
            size_t map_size = rows * cols;
            size_t data_avail = size - 7;
            size_t copy_len = (map_size < data_avail) ? map_size : data_avail;
            memcpy(active_map.active_map, data + 7, copy_len);
            if (copy_len < map_size) {
                memset(active_map.active_map + copy_len, 0, map_size - copy_len);
            }

            ret = aom_codec_control(ctx, AOME_SET_ACTIVEMAP, &active_map);
            (void)ret;
            
            free(active_map.active_map);
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder context
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Size > 1 ? Data[1] % 512 : 16) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 2 ? Data[2] % 60 : 30) + 1;
    cfg.rc_target_bitrate = (Size > 3 ? Data[3] * 1000 : 1000);

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzzing logic on encoder controls
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}