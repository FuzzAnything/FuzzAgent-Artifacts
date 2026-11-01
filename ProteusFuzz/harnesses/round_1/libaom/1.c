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
#include <stdarg.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    // Extract control ID and argument from fuzz data
    int ctrl_id = (int)(data[0] & 0xFF);
    int arg_val = (int)(data[1] & 0xFF);

    aom_codec_err_t res;
    
    // Try AOME_SET_CPUUSED (common control)
    if (ctrl_id == 0) {
        res = aom_codec_control(ctx, AOME_SET_CPUUSED, arg_val);
    }
    // Try AV1E_SET_INTER_DCT_ONLY if available
    else if (ctrl_id == 1) {
        res = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, arg_val);
    }
    // Try AV1E_SET_ERROR_RESILIENT_MODE
    else if (ctrl_id == 2) {
        res = aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, arg_val);
    }
    // Try AV1E_SET_QUANT_B_ADAPT
    else if (ctrl_id == 3) {
        res = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, arg_val);
    }
    // Try AV1E_SET_BITRATE_ONE_PASS_CBR
    else if (ctrl_id == 4) {
        res = aom_codec_control(ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, arg_val);
    }
    // Try with random control ID (might trigger error handling)
    else {
        res = aom_codec_control(ctx, ctrl_id, arg_val);
    }
    
    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Need at least 5 bytes for configuration
    if (Size < 5) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize encoder context
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzz data to configure encoder safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder controls with remaining data
    if (Size > 5) {
        fuzz_encoder_controls(&ctx, Data + 5, Size - 5);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}