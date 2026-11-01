// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret;
    
    ret = aom_codec_control(ctx, control_id, value);
    
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret;
    
    ret = aom_codec_control(ctx, control_id, value);
    
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int use_encoder = Data[0] & 1;
    int control_idx = Data[1] % 6;
    
    int value = 0;
    if (Size > 2) {
        value = Data[2] & 0xFF;
        if (Size > 3) value |= (Data[3] << 8);
        if (Size > 4) value |= (Data[4] << 16);
        if (Size > 5) value |= (Data[5] << 24);
    }
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface;
    aom_codec_err_t init_ret;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;
    
    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (iface == NULL) return 0;
        
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }
        
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        
        init_ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (init_ret != AOM_CODEC_OK) {
            return 0;
        }
        
        int control_id;
        switch (control_idx) {
            case 0: control_id = AV1E_SET_REDUCED_REFERENCE_SET; break;
            case 1: control_id = AV1E_SET_CHROMA_SAMPLE_POSITION; break;
            case 2: control_id = AV1E_SET_COLOR_PRIMARIES; break;
            case 3: control_id = AV1E_SET_TRANSFER_CHARACTERISTICS; break;
            case 4: control_id = AV1E_SET_COLOR_RANGE; break;
            case 5: control_id = AV1E_SET_MATRIX_COEFFICIENTS; break;
            default: return 0;
        }
        
        fuzz_encoder_control(&ctx, control_id, value);
        
        aom_codec_destroy(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (iface == NULL) return 0;
        
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        
        init_ret = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (init_ret != AOM_CODEC_OK) {
            return 0;
        }
        
        int control_id;
        switch (control_idx) {
            case 0: control_id = AV1E_SET_REDUCED_REFERENCE_SET; break;
            case 1: control_id = AV1E_SET_CHROMA_SAMPLE_POSITION; break;
            case 2: control_id = AV1E_SET_COLOR_PRIMARIES; break;
            case 3: control_id = AV1E_SET_TRANSFER_CHARACTERISTICS; break;
            case 4: control_id = AV1E_SET_COLOR_RANGE; break;
            case 5: control_id = AV1E_SET_MATRIX_COEFFICIENTS; break;
            default: return 0;
        }
        
        fuzz_decoder_control(&ctx, control_id, value);
        
        aom_codec_destroy(&ctx);
    }
    
    return 0;
}