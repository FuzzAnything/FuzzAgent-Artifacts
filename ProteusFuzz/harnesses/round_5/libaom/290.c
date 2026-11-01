// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, aom_codec_err_t *ret, int control_id, void *data) {
    if (!ctx || !ret) return 0;
    *ret = aom_codec_control(ctx, control_id, data);
    return 1;
}

int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_err_t err;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure basic settings
    uint32_t width = (Data[0] % 64) + 16;
    uint32_t height = (Data[1] % 64) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] * 256 + Data[3]) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Fuzz control functions with derived values from input
    int idx = 4;
    
    // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (idx < Size) {
        int enable_tx = Data[idx] % 2;
        fuzz_codec_control(&ctx, &err, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &enable_tx);
        idx++;
    }

    // 2. AOME_SET_ACTIVEMAP
    if (idx + 4 <= Size) {
        aom_active_map_t amap;
        amap.rows = (Data[idx] % 32) + 1;
        amap.cols = (Data[idx + 1] % 32) + 1;
        size_t map_size = amap.rows * amap.cols;
        if (map_size > 0 && map_size <= 1024) {
            amap.active_map = (unsigned char *)malloc(map_size);
            if (amap.active_map) {
                for (size_t i = 0; i < map_size && idx + 2 + i < Size; i++) {
                    amap.active_map[i] = Data[idx + 2 + i] % 2;
                }
                fuzz_codec_control(&ctx, &err, AOME_SET_ACTIVEMAP, &amap);
                free(amap.active_map);
            }
            idx += 2 + map_size;
        } else {
            idx += 2;
        }
    }

    // 3. AOME_SET_ARNR_MAXFRAMES
    if (idx < Size) {
        int maxframes = Data[idx] % 16;
        fuzz_codec_control(&ctx, &err, AOME_SET_ARNR_MAXFRAMES, &maxframes);
        idx++;
    }

    // 4. AV1E_SET_COLOR_RANGE
    if (idx < Size) {
        int color_range = Data[idx] % 2; // 0: limited, 1: full
        fuzz_codec_control(&ctx, &err, AV1E_SET_COLOR_RANGE, &color_range);
        idx++;
    }

    // 5. AOME_SET_ARNR_STRENGTH
    if (idx < Size) {
        int strength = Data[idx] % 10;
        fuzz_codec_control(&ctx, &err, AOME_SET_ARNR_STRENGTH, &strength);
        idx++;
    }

    // 6. AV1E_SET_ENABLE_INTERINTRA_COMP
    if (idx < Size) {
        int enable_interintra = Data[idx] % 2;
        fuzz_codec_control(&ctx, &err, AV1E_SET_ENABLE_INTERINTRA_COMP, &enable_interintra);
        idx++;
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}