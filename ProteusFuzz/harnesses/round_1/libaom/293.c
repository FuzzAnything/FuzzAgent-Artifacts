// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

static int try_control(aom_codec_ctx_t *ctx, int ctrl_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_293(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = NULL;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize default encoder configuration with correct arguments
    if (aom_codec_enc_config_default(iface, &cfg, AOM_USAGE_REALTIME) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from the fuzz input to diversify control parameters
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    // Attempt to invoke the target controls using standard AV1 control IDs.
    // Corrected identifiers based on compiler error messages (AV1E_... instead of AOM_CODEC_AV1E_...)

    // 1. AV1E_SET_INTER_DCT_ONLY
    try_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val1);

    // 2. AV1E_SET_QM_MAX
    try_control(&ctx, AV1E_SET_QM_MAX, val2);

    // 3. AV1E_SET_QM_Y
    try_control(&ctx, AV1E_SET_QM_Y, val3);

    // 4. AV1E_SET_BITRATE_ONE_PASS_CBR
    try_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, val4);

    // 5. AOME_SET_CQ_LEVEL
    try_control(&ctx, AOME_SET_CQ_LEVEL, val5);

    // 6. AOME_SET_CPUUSED
    try_control(&ctx, AOME_SET_CPUUSED, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}