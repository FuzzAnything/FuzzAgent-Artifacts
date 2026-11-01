// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_256(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize configuration
    aom_codec_enc_cfg_t cfg;
    
    // Get the AV1 encoder interface explicitly to ensure it's available
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Extract control values from input data
    int val1 = (Size > 4) ? (Data[4] & 0x1) : 0;
    int val2 = (Size > 5) ? (Data[5] & 0x1) : 0;
    int val3 = (Size > 6) ? (Data[6] & 0x1) : 0;
    int val4 = (Size > 7) ? (Data[7] & 0x1) : 0;
    int val5 = (Size > 8) ? (Data[8] & 0x1) : 0;
    int val6 = (Size > 9) ? (Data[9] & 0x1) : 0;

    // Fuzz the target control functions
    // Note: Using the actual enum names defined in aomcx.h for AV1 encoder controls.
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val3);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val4);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_OBMC, val5);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}