// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int32_t val_int;
    uint8_t val_uint8;
    
    // AV1E_SET_QM_U: Pass a pointer to fuzz data if size allows
    if (size >= 16) {
        aom_codec_control(ctx, AV1E_SET_QM_U, (void*)data);
    }
    
    // AV1E_SET_ENABLE_INTRABC: Boolean flag (0 or 1)
    if (size > 0) {
        val_uint8 = data[0] % 2;
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, (void*)(intptr_t)val_uint8);
    }
    
    // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR: Integer value (milliseconds)
    if (size >= 4) {
        memcpy(&val_int, data, sizeof(int32_t));
        if (val_int < 0) val_int = 0;
        if (val_int > 10000) val_int = 10000;
        aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, (void*)(intptr_t)val_int);
    }
    
    // AOME_SET_TUNING: Tuning mode (integer)
    if (size > 1) {
        val_uint8 = data[1] % 10;
        aom_codec_control(ctx, AOME_SET_TUNING, (void*)(intptr_t)val_uint8);
    }
    
    // AV1E_SET_ENABLE_PAETH_INTRA: Boolean flag
    if (size > 2) {
        val_uint8 = data[2] % 2;
        aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, (void*)(intptr_t)val_uint8);
    }
    
    // AV1E_SET_ENABLE_CHROMA_DELTAQ: Boolean flag
    if (size > 3) {
        val_uint8 = data[3] % 2;
        aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, (void*)(intptr_t)val_uint8);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    // Initialize encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz some config parameters based on input
    if (Size > 0) cfg.g_w = (Data[0] % 256) + 16; 
    if (Size > 1) cfg.g_h = (Data[1] % 256) + 16; 
    if (Size > 2) cfg.g_timebase.num = Data[2] % 100 + 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] % 100 + 1;
    if (Size > 4) cfg.g_threads = Data[4] % 8 + 1;
    if (Size > 5) cfg.rc_target_bitrate = (Data[5] * 256 + (Size > 6 ? Data[6] : 0)) * 1000;

    // Initialize codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}