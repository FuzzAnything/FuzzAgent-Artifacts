// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    // Need at least 8 bytes: 4 for control_id, 4 for value
    if (Size < 8) return 0;

    // Safely extract control ID and value from input
    int32_t control_id = 0;
    int32_t value = 0;
    
    // Ensure we don't read beyond buffer bounds
    if (Size >= 4) {
        memcpy(&control_id, Data, sizeof(int32_t));
    }
    if (Size >= 8) {
        memcpy(&value, Data + 4, sizeof(int32_t));
    }

    // Limit value range to reasonable bounds to avoid immediate crashes
    if (value < -1000000 || value > 1000000) {
        value = 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    
    if (enc_iface == NULL) return 0;

    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    
    if (dec_iface == NULL) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    if (aom_codec_dec_init(&dec_ctx, dec_iface, NULL, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Try encoder controls
    fuzz_encoder_control(&enc_ctx, control_id, value);
    
    // Try decoder controls
    fuzz_decoder_control(&dec_ctx, control_id, value);

    // Cleanup
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}