// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static int safe_read_int(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + 1 <= Size) {
        return (int)Data[offset];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for basic configuration
    if (Size < 8) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic parameters with bounds checking
    cfg.g_w = safe_read_int(Data, Size, 0) % 1024 + 16;
    cfg.g_h = safe_read_int(Data, Size, 1) % 1024 + 16;
    cfg.g_timebase.num = safe_read_int(Data, Size, 2) % 100 + 1;
    cfg.g_timebase.den = safe_read_int(Data, Size, 3) % 100 + 1;
    cfg.rc_target_bitrate = safe_read_int(Data, Size, 4) % 1000 + 1;
    cfg.g_threads = safe_read_int(Data, Size, 5) % 16 + 1;
    cfg.g_error_resilient = safe_read_int(Data, Size, 6) % 2;
    cfg.g_lag_in_frames = safe_read_int(Data, Size, 7) % 25;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = safe_read_int(Data, Size, 8) % 2;
    int val2 = safe_read_int(Data, Size, 9) % 2;
    int val3 = safe_read_int(Data, Size, 10) % 2;
    int val4 = safe_read_int(Data, Size, 11) % 2;
    int val5 = safe_read_int(Data, Size, 12) % 3; // Tune content has more options
    int val6 = safe_read_int(Data, Size, 13) % 2;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val1);

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val2);

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val3);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val4);

    // Fuzz AV1E_SET_TUNE_CONTENT
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val5);

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}