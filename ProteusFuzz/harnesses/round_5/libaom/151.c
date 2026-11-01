// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    // The macro is typically AV1E_SET_ENABLE_WARPED_MOTION, passed directly to aom_codec_control
    if (size >= 1) {
        int val = data[0] % 2; // 0 or 1
        aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, val);
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Requires aom_film_grain_params_t. We skip complex struct construction to avoid crashes.
    // Attempting to pass raw data is unsafe without proper struct alignment.
    
    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    // Requires aom_loopfilter_control_t. Skipped for safety.

    // Fuzz AV1E_SET_FILM_GRAIN_TABLE
    // Requires aom_film_grain_table_t. Skipped for safety.

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    // Requires aom_matrix_coefficients_t. Skipped for safety.

    return 0;
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Set some minimal config from fuzz data to vary states
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_error_resilient = (Data[5] % 3);
    cfg.g_lag_in_frames = (Data[6] % 20);
    cfg.g_threads = (Data[7] % 4) + 1;
    cfg.g_profile = (Data[8] % 4);
    cfg.g_usage = (Data[9] % 3);
    cfg.g_pass = AOM_RC_ONE_PASS; // Fixed: AOM_ONE_PASS -> AOM_RC_ONE_PASS
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Call control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Call aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Do nothing with headers, just ensure no crash
        free(headers->buf);
        free(headers);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}