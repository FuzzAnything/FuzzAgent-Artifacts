// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int get_int(const uint8_t *d, size_t s, int def, int min, int max) {
    if (s < 4) return def;
    int val = *(int32_t*)(d);
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    // Use the correct function name aom_codec_av1_cx
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    // Use fuzzing data to configure width/height within safe bounds
    if (size >= 2) {
        uint16_t w = data[0] % 2048;
        uint16_t h = data[1] % 2048;
        if (w == 0) w = 16;
        if (h == 0) h = 16;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    // Fix: Use AOM_ERRRES_NONE is not valid in this context, use 0 or correct enum
    // AOM_ERRRES_NONE is typically defined in older versions or specific headers. 
    // In standard aom_encoder.h, g_error_resilient is an int. 0 is usually NONE.
    cfg.g_error_resilient = 0; 

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder
    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS (0 or 1)
    int val1 = get_int(Data, Size, 0, 0, 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val1);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    // Valid range typically 8 to 256
    int val2 = get_int(Data + 4, Size > 4 ? Size - 4 : 0, 8, 8, 256);
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, val2);

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET (0 or 1)
    int val3 = get_int(Data + 8, Size > 8 ? Size - 8 : 0, 0, 0, 1);
    aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val3);

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS (0 or 1)
    int val4 = get_int(Data + 12, Size > 12 ? Size - 12 : 0, 0, 0, 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val4);

    // Fuzz AV1E_SET_QUANTIZER_ONE_PASS (0 or 1)
    int val5 = get_int(Data + 16, Size > 16 ? Size - 16 : 0, 0, 0, 1);
    aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, val5);

    // Fuzz AV1E_SET_TILE_ROWS
    // Valid range 0 to 7 (or similar depending on version)
    int val6 = get_int(Data + 20, Size > 20 ? Size - 20 : 0, 0, 0, 7);
    aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val6);

    // Cleanup
    cleanup_codec(ctx);
    return 0;
}