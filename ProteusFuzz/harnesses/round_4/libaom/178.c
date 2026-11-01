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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (ctx == NULL) return -1;

    // Helper to safely extract an integer from fuzz data
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    } else {
        val = 0;
    }

    // 1. AV1E_SET_DELTALF_MODE
    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    (void)ret;

    // 2. AOME_SET_ARNR_STRENGTH
    ret = aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, val);
    (void)ret;

    // 3. AV1E_SET_DENOISE_NOISE_LEVEL
    ret = aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val);
    (void)ret;

    // 4. AOME_SET_SHARPNESS
    ret = aom_codec_control(ctx, AOME_SET_SHARPNESS, val);
    (void)ret;

    // 5. AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val);
    (void)ret;

    // 6. AOME_SET_ARNR_MAXFRAMES
    ret = aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int ret;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Set default configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    if (Size > 0) {
        fuzz_encoder_control(&ctx, Data, Size);
    }

    // Cleanup
    ret = aom_codec_destroy(&ctx);
    (void)ret;

    return 0;
}