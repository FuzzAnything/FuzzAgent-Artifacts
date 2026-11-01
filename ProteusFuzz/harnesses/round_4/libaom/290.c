// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int32_t arg) {
    if (!ctx) return 0;
    int ret = 0;
    
    // AV1E_SET_ENABLE_WARPED_MOTION
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, arg);
    (void)ret;
    
    // AV1E_SET_ENABLE_RECT_PARTITIONS
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, arg);
    (void)ret;
    
    // AV1E_SET_COLOR_RANGE
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, arg);
    (void)ret;
    
    // AV1E_SET_CHROMA_SAMPLE_POSITION
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, arg);
    (void)ret;
    
    // AV1E_SET_COLOR_PRIMARIES
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, arg);
    (void)ret;
    
    // AV1E_SET_TRANSFER_CHARACTERISTICS
    ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, arg);
    (void)ret;
    
    return 0;
}

int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract parameters from fuzz input
    int32_t arg = *(int32_t *)Data;
    
    // Initialize encoder context
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions with extracted argument
    fuzz_encoder_control(&ctx, arg);

    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}