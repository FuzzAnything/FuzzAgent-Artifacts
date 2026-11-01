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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    int32_t val;
    if (size >= sizeof(int32_t)) {
        memcpy(&val, data, sizeof(int32_t));
    } else {
        val = data[0];
    }

    // Try to set various AV1 encoder controls
    // AV1E_SET_FRAME_PERIODIC_BOOST
    aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val);
    
    // AV1E_SET_ENABLE_ONESIDED_COMP
    aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);
    
    // AV1E_SET_ENABLE_REF_FRAME_MVS
    aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
    
    // AV1E_SET_ENABLE_FILTER_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);
    
    // AV1E_SET_ENABLE_SMOOTH_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);

    // AOME_USE_REFERENCE
    aom_codec_control(ctx, AOME_USE_REFERENCE, val);

    return 1;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    
    // Get the interface pointer first
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Pass the interface pointer directly, not its address
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to tweak config slightly to explore states
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] * 256 + Data[4]) + 100;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}