// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret = 0;

    // Initialize configuration based on input data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.g_error_resilient = (Data[5] % 2);
    cfg.rc_target_bitrate = (Data[6] * 256 + Data[7]) + 100;

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Ensure we have enough data for various control commands
    if (Size > 8) {
        // 1. AV1E_GET_NUM_OPERATING_POINTS
        int num_ops;
        ret = aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
        (void)ret;

        // 2. AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
        if (Size > 9) {
            int32_t drop_ms = (int32_t)(Data[8] * 256 + (Size > 10 ? Data[9] : 0));
            ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, drop_ms);
            (void)ret;
        }

        // 3. AV1E_SET_LOOPFILTER_CONTROL
        if (Size > 11) {
            int32_t lf_delta = (int32_t)(Data[10] * 256 + (Size > 12 ? Data[11] : 0));
            ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_delta);
            (void)ret;
        }

        // 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR
        // This typically takes a pointer to a structure or data.
        // We create a small dummy buffer if size permits.
        if (Size > 13) {
            // Construct a minimal film grain param structure or raw data pointer
            // Since the exact struct is complex, we pass a pointer to the input data
            // assuming the API accepts a pointer to a buffer for test vectors.
            // Note: This might trigger an error if the API expects a specific struct layout,
            // which is the goal of fuzzing (to test error handling).
            ret = aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)(const uint8_t*)(Data + 12));
            (void)ret;
        }

        // 5. AV1E_SET_SVC_FRAME_DROP_MODE
        if (Size > 14) {
            int32_t drop_mode = Data[13] % 4; // Assuming 0-3 are valid modes
            ret = aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, drop_mode);
            (void)ret;
        }

        // 6. AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
        if (Size > 15) {
            int32_t drop_frames = (int32_t)(Data[14] * 256 + (Size > 16 ? Data[15] : 0));
            ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, drop_frames);
            (void)ret;
        }
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}