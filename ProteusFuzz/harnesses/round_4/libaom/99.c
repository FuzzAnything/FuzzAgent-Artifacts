// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

// Helper to initialize an encoder context
static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Use aom_codec_av1_cx() to get the interface, passing NULL to config_default
    // since we will set specific fields manually or rely on defaults.
    // The first argument to aom_codec_enc_config_default is the codec ID.
    // In newer libaom, AOM_CODEC_AV1 might not be directly defined as an enum value
    // in all contexts, but AOM_CODEC_AV1 is usually the ID.
    // If AOM_CODEC_AV1 is missing, we rely on the interface pointer directly in init,
    // but config_default needs an ID.
    // Let's try to use the interface pointer directly for init if config_default fails or ID is missing.
    // However, standard practice is:
    // aom_codec_iface_t *iface = aom_codec_av1_cx();
    // aom_codec_enc_config_default(iface, cfg, 0);
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    
    // Use fuzzed data to configure width/height if possible, with safety checks
    if (size >= 8) {
        uint32_t width = *(const uint32_t *)data;
        uint32_t height = *(const uint32_t *)(data + 4);
        // Safety checks to prevent huge allocations
        if (width > 0 && width < 8192 && height > 0 && height < 8192) {
            cfg->g_w = width;
            cfg->g_h = height;
        }
    }
    
    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

// Helper to initialize a decoder context
static int init_decoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) return -1;

    if (aom_codec_dec_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int ret;

    // Randomly choose between encoder and decoder context initialization
    // to exercise both encoder and decoder specific control functions
    int mode = Data[0] % 2;

    if (mode == 0) {
        // Encoder Mode
        if (init_encoder(&codec, &cfg, Data + 1, Size > 1 ? Size - 1 : 0) != 0) {
            return 0;
        }

        // 1. AOME_SET_NUMBER_SPATIAL_LAYERS
        if (Size > 1) {
            int layers = Data[1] % 12; // Limit to reasonable range
            ret = aom_codec_control(&codec, AOME_SET_NUMBER_SPATIAL_LAYERS, layers);
            (void)ret;
        }

        // 2. AV1E_SET_ENABLE_FILTER_INTRA
        if (Size > 2) {
            int enable = (Data[2] % 2) ? 1 : 0;
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_FILTER_INTRA, enable);
            (void)ret;
        }

        // 3. AV1E_SET_SVC_FRAME_DROP_MODE
        if (Size > 3) {
            int mode_val = Data[3] % 3; // Assuming 0-2 range for modes
            ret = aom_codec_control(&codec, AV1E_SET_SVC_FRAME_DROP_MODE, mode_val);
            (void)ret;
        }

        // 4. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
        if (Size > 4) {
            int enable = (Data[4] % 2) ? 1 : 0;
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, enable);
            (void)ret;
        }

        // 5. AOME_SET_SPATIAL_LAYER_ID
        if (Size > 5) {
            int layer_id = Data[5] % 8;
            ret = aom_codec_control(&codec, AOME_SET_SPATIAL_LAYER_ID, layer_id);
            (void)ret;
        }

        // Cleanup encoder
        aom_codec_destroy(&codec);

    } else {
        // Decoder Mode
        if (init_decoder(&codec, Data + 1, Size > 1 ? Size - 1 : 0) != 0) {
            return 0;
        }

        // 6. AV1D_SET_OPERATING_POINT
        if (Size > 1) {
            int op_point = Data[1]; // Use raw byte, library should handle invalid
            ret = aom_codec_control(&codec, AV1D_SET_OPERATING_POINT, op_point);
            (void)ret;
        }

        // Cleanup decoder
        aom_codec_destroy(&codec);
    }

    return 0;
}