// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>

static int fuzzer_init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_219(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (fuzzer_init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Interpret input data to drive fuzzing logic
    uint8_t mode = Data[0] % 10;
    int32_t val1 = 0;
    int32_t val2 = 0;
    int32_t val3 = 0;
    int32_t val4 = 0;
    int32_t val5 = 0;

    if (Size >= 2) {
        val1 = (Data[1] << 8) | (Data[0] & 0xFF);
        if (Size >= 4) val1 = (val1 << 8) | Data[2];
        if (Size >= 5) val1 = (val1 << 8) | Data[3];
    }
    if (Size >= 6) {
        val2 = (Data[4] << 8) | Data[5];
        if (Size >= 8) val2 = (val2 << 8) | Data[6];
        if (Size >= 9) val2 = (val2 << 8) | Data[7];
    }
    if (Size >= 10) {
        val3 = (Data[8] << 8) | Data[9];
        if (Size >= 12) val3 = (val3 << 8) | Data[10];
        if (Size >= 13) val3 = (val3 << 8) | Data[11];
    }
    if (Size >= 14) {
        val4 = (Data[12] << 8) | Data[13];
        if (Size >= 16) val4 = (val4 << 8) | Data[14];
        if (Size >= 17) val4 = (val4 << 8) | Data[15];
    }
    if (Size >= 18) {
        val5 = (Data[16] << 8) | Data[17];
        if (Size >= 20) val5 = (val5 << 8) | Data[18];
        if (Size >= 21) val5 = (val5 << 8) | Data[19];
    }

    // Clamp values to reasonable ranges to avoid crashes from extreme values
    if (val1 > 10000) val1 = 10000;
    if (val1 < 0) val1 = 0;
    if (val2 > 255) val2 = 255;
    if (val2 < 0) val2 = 0;
    if (val3 > 1) val3 = 1;
    if (val3 < 0) val3 = 0;
    if (val4 > 10) val4 = 10;
    if (val4 < 0) val4 = 0;
    if (val5 > 10) val5 = 10;
    if (val5 < 0) val5 = 0;

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    if (mode == 0) {
        aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val1);
    }
    // Fuzz AV1E_SET_QM_Y
    else if (mode == 1) {
        aom_codec_control(&ctx, AV1E_SET_QM_Y, val2);
    }
    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    else if (mode == 2) {
        aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);
    }
    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    else if (mode == 3) {
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val3);
    }
    // Fuzz AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    else if (mode == 4) {
        aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val4);
    }
    // Fuzz AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    else if (mode == 5) {
        aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val5);
    }
    // Fuzz all sequentially
    else if (mode == 6) {
        aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val1);
        aom_codec_control(&ctx, AV1E_SET_QM_Y, val2);
        aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val3);
        aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val4);
        aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val5);
    }
    // Fuzz with random combinations based on data
    else {
        if (Data[0] % 2 == 0) aom_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val1);
        if (Data[0] % 3 == 0) aom_codec_control(&ctx, AV1E_SET_QM_Y, val2);
        if (Data[0] % 4 == 0) aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val3);
        if (Data[0] % 5 == 0) aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val3);
        if (Data[0] % 6 == 0) aom_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val4);
        if (Data[0] % 7 == 0) aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val5);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}