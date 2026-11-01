// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_266(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide encoder/decoder, second byte for specific control
    int mode = Data[0] % 2;
    int control_idx = Data[1] % 6;
    
    // Generate a pseudo-random value for the control parameter
    int value = 0;
    if (Size > 2) {
        value = (int)(Data[2] & 0xFF);
    }
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    
    if (mode == 0) {
        // Encoder path
        iface = aom_codec_av1_cx();
        if (!iface) return 0;
        
        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }
        
        // Use data to tweak config slightly for variety
        cfg.g_w = (Data[Size > 3 ? 3 : 0] % 128) + 16;
        cfg.g_h = (Data[Size > 4 ? 4 : 0] % 128) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        
        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }
        
        int control_id = 0;
        switch (control_idx) {
            case 0: control_id = AV1E_SET_ENABLE_CFL_INTRA; break;
            case 1: control_id = AV1E_SET_AUTO_INTRA_TOOLS_OFF; break;
            case 2: control_id = AV1E_SET_CHROMA_SAMPLE_POSITION; break;
            case 3: control_id = AV1E_SET_ENABLE_INTRABC; break;
            case 4: control_id = AV1E_SET_TUNE_CONTENT; break;
            case 5: control_id = AV1E_SET_REDUCED_TX_TYPE_SET; break;
            default: control_id = 0; break;
        }
        
        fuzz_encoder_control(&ctx, control_id, value);
        
        aom_codec_destroy(&ctx);
    } else {
        // Decoder path (though controls are encoder-specific, we test the context setup)
        iface = aom_codec_av1_dx();
        if (!iface) return 0;
        
        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        
        err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            return 0;
        }
        
        // Even though these are encoder controls, we pass them to see how the library handles invalid calls on decoder
        int control_id = 0;
        switch (control_idx) {
            case 0: control_id = AV1E_SET_ENABLE_CFL_INTRA; break;
            case 1: control_id = AV1E_SET_AUTO_INTRA_TOOLS_OFF; break;
            case 2: control_id = AV1E_SET_CHROMA_SAMPLE_POSITION; break;
            case 3: control_id = AV1E_SET_ENABLE_INTRABC; break;
            case 4: control_id = AV1E_SET_TUNE_CONTENT; break;
            case 5: control_id = AV1E_SET_REDUCED_TX_TYPE_SET; break;
            default: control_id = 0; break;
        }
        
        fuzz_decoder_control(&ctx, control_id, value);
        
        aom_codec_destroy(&ctx);
    }
    
    return 0;
}