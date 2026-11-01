// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    aom_codec_err_t err;
    
    switch (control_id) {
        case 0: // AV1E_SET_ENABLE_RECT_TX
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, value);
            break;
        case 1: // AV1E_SET_ENABLE_TX_SIZE_SEARCH
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, value);
            break;
        case 2: // AV1E_SET_ENABLE_INTERINTRA_COMP
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
            break;
        case 3: // AV1E_SET_ENABLE_GLOBAL_MOTION
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, value);
            break;
        case 4: // AV1E_SET_CDF_UPDATE_MODE
            err = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, value);
            break;
        case 5: // AV1E_SET_ENABLE_DIAGONAL_INTRA
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
            break;
        default:
            return 0;
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    int control_id = Data[0] % 6;
    int value = (int)(Data[1] & 0xFF);
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    fuzz_codec_control(&ctx, control_id, value);
    
    if (Size > 2) {
        int value2 = (int)(Data[2] & 0xFF);
        fuzz_codec_control(&ctx, control_id, value2);
    }
    
    aom_codec_destroy(&ctx);
    
    return 0;
}