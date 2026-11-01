// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_QM_MAX at aomcx.h:2042:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID at aomcx.h:1887:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE at aomcx.h:2360:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS at aomcx.h:1923:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE at aomcx.h:2342:1 in aomcx.h
// aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT at aomdx.h:549:1 in aomdx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/* 
 * Helper function to safely read an int from the buffer
 */
static int read_int(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size >= offset + 4) {
        int val = 0;
        memcpy(&val, Data + offset, sizeof(int));
        return val;
    }
    return 0;
}

/* 
 * Helper function to safely read an unsigned int from the buffer
 */
static unsigned int read_uint(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size >= offset + 4) {
        unsigned int val = 0;
        memcpy(&val, Data + offset, sizeof(unsigned int));
        return val;
    }
    return 0;
}

static void fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx) return;

    /* AV1E_SET_QM_MAX expects unsigned int */
    unsigned int qm_max = read_uint(Data, Size, 0);
    aom_codec_control_typechecked_AV1E_SET_QM_MAX(ctx, AV1E_SET_QM_MAX, qm_max);

    /* AOME_SET_SPATIAL_LAYER_ID expects int */
    int spatial_layer_id = read_int(Data, Size, 4);
    aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID(ctx, AOME_SET_SPATIAL_LAYER_ID, spatial_layer_id);

    /* AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE expects unsigned int */
    unsigned int low_complexity = read_uint(Data, Size, 8);
    aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, low_complexity);

    /* AOME_SET_NUMBER_SPATIAL_LAYERS expects int */
    int num_spatial_layers = read_int(Data, Size, 12);
    aom_codec_control_typechecked_AOME_SET_NUMBER_SPATIAL_LAYERS(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, num_spatial_layers);

    /* AV1E_SET_SVC_FRAME_DROP_MODE expects int */
    int svc_frame_drop_mode = read_int(Data, Size, 16);
    aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, svc_frame_drop_mode);
}

static void fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx) return;

    /* AV1D_SET_OPERATING_POINT expects int */
    int operating_point = read_int(Data, Size, 0);
    aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT(ctx, AV1D_SET_OPERATING_POINT, operating_point);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Byte 0: Codec type (0 = Encoder, 1 = Decoder) */
    int is_decoder = (Data[0] % 2) != 0;
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;

    if (is_decoder) {
        /* Initialize Decoder */
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        
        res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_decoder_controls(&ctx, Data, Size);

    } else {
        /* Initialize Encoder */
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        
        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            return 0;
        }

        fuzz_encoder_controls(&ctx, Data, Size);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}