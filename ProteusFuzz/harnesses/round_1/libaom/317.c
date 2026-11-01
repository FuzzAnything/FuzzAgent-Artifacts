// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

static int run_encoder_control_test(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    return (ret == AOM_CODEC_OK) ? 0 : 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_317(const uint8_t *Data, size_t Size) {
    // Minimum size required to safely access Data[0] to Data[8]
    if (Size < 9) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val1, val2, val3;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config from input to vary states
    // Ensure dimensions are valid (minimum 16x16)
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 100;

    // Initialize the codec
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for controls
    val1 = (Data[4] % 2); // Boolean-like values (0 or 1)
    val2 = (Data[5] % 2);
    val3 = (Data[6] % 2);
    int partition_size = (Data[7] % 128) + 1; // Partition size
    int drop_ms = (Data[8] % 1000); // Milliseconds

    // Attempt to invoke the target controls.
    // We use #ifdef to ensure compilation only if the specific constants
    // are defined in the included headers.
    
    // 1. AV1E_SET_INTER_DCT_ONLY
    #ifdef AOM_AV1E_SET_INTER_DCT_ONLY
    run_encoder_control_test(&ctx, AOM_AV1E_SET_INTER_DCT_ONLY, val1);
    #endif

    // 2. AV1E_SET_MAX_PARTITION_SIZE
    #ifdef AOM_AV1E_SET_MAX_PARTITION_SIZE
    run_encoder_control_test(&ctx, AOM_AV1E_SET_MAX_PARTITION_SIZE, partition_size);
    #endif

    // 3. AV1E_SET_ENABLE_INTERINTER_WEDGE
    #ifdef AOM_AV1E_SET_ENABLE_INTERINTER_WEDGE
    run_encoder_control_test(&ctx, AOM_AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);
    #endif

    // 4. AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    #ifdef AOM_AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    run_encoder_control_test(&ctx, AOM_AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, drop_ms);
    #endif

    // 5. AV1E_SET_ENABLE_CHROMA_DELTAQ
    #ifdef AOM_AV1E_SET_ENABLE_CHROMA_DELTAQ
    run_encoder_control_test(&ctx, AOM_AV1E_SET_ENABLE_CHROMA_DELTAQ, val3);
    #endif

    // 6. AV1E_SET_ALLOW_REF_FRAME_MVS
    #ifdef AOM_AV1E_SET_ALLOW_REF_FRAME_MVS
    run_encoder_control_test(&ctx, AOM_AV1E_SET_ALLOW_REF_FRAME_MVS, val1);
    #endif

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}