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

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    return (res == AOM_CODEC_OK);
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size) {
    // Need at least 5 bytes to read 1 byte for control ID and 4 bytes for value
    if (Size < 5) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and data from fuzz input safely
    int control_id = Data[0];
    int value = (Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4];
    
    // Map control IDs to known constants if possible
    int mapped_control = control_id;
    switch (control_id) {
        case 0: mapped_control = AV1E_SET_RTC_EXTERNAL_RC; break;
        case 1: mapped_control = AV1E_SET_LOSSLESS; break;
        case 2: mapped_control = AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST; break;
        case 3: mapped_control = AV1E_SET_TIER_MASK; break;
        case 4: mapped_control = AV1E_SET_QM_MIN; break;
        case 5: mapped_control = AV1E_SET_MV_COST_UPD_FREQ; break;
        default: mapped_control = control_id; break;
    }

    // Invoke control
    invoke_control(&ctx, mapped_control, value);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}