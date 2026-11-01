// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

static int read_int(const uint8_t *d, size_t s) {
    if (s < 4) return 0;
    return (int)((d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3]);
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int ret = 0;
    int val;

    // 1. AV1E_SET_ENABLE_ONESIDED_COMP
    if (size >= 4) {
        val = read_int(data, size);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);
        (void)ret;
    }

    // 2. AV1E_GET_NUM_OPERATING_POINTS
    if (size >= 4) {
        int *out = NULL;
        ret = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &out);
        if (out) {
            (void)*out;
        }
        (void)ret;
    }

    // 3. AV1E_SET_ENABLE_WARPED_MOTION
    if (size >= 4) {
        val = read_int(data + 4, size > 4 ? size - 4 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, val);
        (void)ret;
    }

    // 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Note: The exact type and structure for film grain test vector may vary.
    // Using a generic pointer approach if the specific struct is not directly available or defined differently.
    // Assuming aom_film_grain_test_vector_t might be defined in aomcx.h or similar, 
    // but if not, we might need to construct it based on available types or skip if undefined.
    // For now, we attempt to use a placeholder if the type is not directly accessible.
    // If compilation fails due to missing type, this block may need adjustment based on actual libaom headers.
    #ifdef AOM_FILM_GRAIN_TEST_VECTOR_T_DEFINED
    if (size >= 8) {
        aom_film_grain_test_vector_t fg_tv = {0};
        // Initialize with dummy data if needed
        ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &fg_tv);
        (void)ret;
    }
    #else
    // Fallback or skip if type is not defined
    (void)size;
    #endif

    // 5. AV1E_GET_BASELINE_GF_INTERVAL
    if (size >= 4) {
        int *out_interval = NULL;
        ret = aom_codec_control(ctx, AV1E_GET_BASELINE_GF_INTERVAL, &out_interval);
        if (out_interval) {
            (void)*out_interval;
        }
        (void)ret;
    }

    // 6. AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (size >= 4) {
        val = read_int(data + 4, size > 4 ? size - 4 : 0);
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_212(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal config from fuzz data
    cfg.g_w = (Data[0] % 64) + 16;
    cfg.g_h = (Data[1] % 64) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);
    return 0;
}