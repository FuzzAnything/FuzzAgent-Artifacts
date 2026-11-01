// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
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

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    // Need at least 4 bytes to safely read an int32_t
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = NULL;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_enc_cfg_t enc_cfg;
    int ret;

    // Select interface based on input data to explore both encoder and decoder paths
    uint8_t mode = Data[0] % 2;
    
    if (mode == 0) {
        // Decoder path
        iface = aom_codec_av1_dx();
        if (!iface) return 0;
        
        // Initialize decoder config
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        // Use fuzzing data to tweak config slightly if possible, but keep valid
        if (Size > 1) dec_cfg.threads = Data[1] % 16;
        else dec_cfg.threads = 1;
        
        ret = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;
    } else {
        // Encoder path
        iface = aom_codec_av1_cx();
        if (!iface) return 0;
        
        // Initialize encoder config using the correct API
        ret = aom_codec_enc_config_default(iface, &enc_cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;
        
        if (Size > 1) enc_cfg.g_w = (Data[1] % 256) * 16;
        if (Size > 2) enc_cfg.g_h = (Data[2] % 256) * 16;
        
        ret = aom_codec_enc_init(&ctx, iface, &enc_cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;
    }

    // Fuzzing logic for the target control functions
    int32_t arg_val = 0;
    
    // Safe read of 4 bytes for arg_val
    memcpy(&arg_val, Data, sizeof(int32_t));
    
    // Attempt to invoke the controls.
    // We assume the macros for control IDs are defined in the included headers.
    // If they are not defined, the preprocessor guards will skip the calls.
    
    // 1. AV1D_SET_OUTPUT_ALL_LAYERS
    #ifdef AV1D_SET_OUTPUT_ALL_LAYERS
    aom_codec_control(&ctx, AV1D_SET_OUTPUT_ALL_LAYERS, &arg_val);
    #endif

    // 2. AOME_SET_SPATIAL_LAYER_ID
    #ifdef AOME_SET_SPATIAL_LAYER_ID
    aom_codec_control(&ctx, AOME_SET_SPATIAL_LAYER_ID, &arg_val);
    #endif

    // 3. AV1D_SET_SKIP_FILM_GRAIN
    #ifdef AV1D_SET_SKIP_FILM_GRAIN
    aom_codec_control(&ctx, AV1D_SET_SKIP_FILM_GRAIN, &arg_val);
    #endif

    // 4. AV1D_GET_DISPLAY_SIZE
    // This is a GET operation, likely needs a struct pointer.
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    #ifdef AV1D_GET_DISPLAY_SIZE
    aom_codec_control(&ctx, AV1D_GET_DISPLAY_SIZE, &img);
    #endif

    // 5. AV1E_SET_SVC_FRAME_DROP_MODE
    #ifdef AV1E_SET_SVC_FRAME_DROP_MODE
    aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &arg_val);
    #endif

    // 6. AV1D_SET_OPERATING_POINT
    #ifdef AV1D_SET_OPERATING_POINT
    aom_codec_control(&ctx, AV1D_SET_OPERATING_POINT, &arg_val);
    #endif

    // Cleanup
    cleanup_codec(&ctx);
    
    return 0;
}