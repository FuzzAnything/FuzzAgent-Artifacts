// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID at aomcx.h:1887:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_1TO4_PARTITIONS at aomcx.h:2066:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS at aomcx.h:1923:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING at aomcx.h:2267:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_CONFIG at aomcx.h:2244:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE at aomcx.h:2069:1 in aomcx.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Use fuzzing data to configure width/height if possible, but clamp to valid ranges
    if (size >= 8) {
        uint32_t w = *(uint32_t *)(data);
        uint32_t h = *(uint32_t *)(data + 4);
        if (w > 0 && w <= 4096) cfg->g_w = w;
        if (h > 0 && h <= 4096) cfg->g_h = h;
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }
    
    cfg->g_threads = 1;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->g_error_resilient = 1;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    // 1. aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_CONFIG
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_CONFIG(aom_codec_ctx_t *ctx, int ctrl, aom_svc_ref_frame_config_t *cfg)
    {
        aom_svc_ref_frame_config_t svc_cfg;
        memset(&svc_cfg, 0, sizeof(svc_cfg));
        
        // Initialize arrays with fuzz data or safe defaults
        if (Size >= 14) {
            for (int i = 0; i < 7; i++) {
                svc_cfg.reference[i] = (Data[i] % 2);
                svc_cfg.ref_idx[i] = (Data[i + 7] % 8);
            }
            for (int i = 0; i < 8; i++) {
                svc_cfg.refresh[i] = (Data[i] % 2);
            }
        } else {
            // Safe defaults
            svc_cfg.reference[0] = 1; svc_cfg.ref_idx[0] = 0;
            svc_cfg.refresh[0] = 1;
        }
        
        ret = aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_CONFIG(&ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_cfg);
        (void)ret;
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE(aom_codec_ctx_t *ctx, int ctrl, int min_partition_size)
    {
        int min_size = 0;
        if (Size > 1) {
            min_size = Data[1] & 0x03; // 0, 1, 2, or 3
        }
        ret = aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_size);
        (void)ret;
    }

    // 3. aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID(aom_codec_ctx_t *ctx, int ctrl, int layer_id)
    {
        int layer_id = 0;
        if (Size > 2) {
            layer_id = Data[2] % 4; // Assume max 4 layers for safety
        }
        ret = aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID(&ctx, AOME_SET_SPATIAL_LAYER_ID, layer_id);
        (void)ret;
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_ENABLE_1TO4_PARTITIONS
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_ENABLE_1TO4_PARTITIONS(aom_codec_ctx_t *ctx, int ctrl, int enable)
    {
        int enable = 0;
        if (Size > 3) {
            enable = (Data[3] % 2);
        }
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_1TO4_PARTITIONS(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, enable);
        (void)ret;
    }

    // 5. aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS(aom_codec_ctx_t *ctx, int ctrl, int num_layers)
    {
        int num_layers = 1;
        if (Size > 4) {
            num_layers = (Data[4] % 4) + 1; // 1 to 4
        }
        ret = aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS(&ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, num_layers);
        (void)ret;
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING
    // Signature: aom_codec_err_t aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING(aom_codec_ctx_t *ctx, int ctrl, int enable)
    {
        int enable = 0;
        if (Size > 5) {
            enable = (Data[5] % 2);
        }
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable);
        (void)ret;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}