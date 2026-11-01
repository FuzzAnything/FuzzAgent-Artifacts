// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *Data, size_t Size) {
    if (!ctx) return 0;

    int ret = 0;
    int val_int;
    int32_t val_int32;

    // Helper to get a value from fuzz data
    #define GET_INT(idx) ((idx < Size) ? Data[idx] : 0)
    #define GET_INT32(idx) ((idx + 3 < Size) ? ((int32_t)Data[idx] | (Data[idx+1]<<8) | (Data[idx+2]<<16) | (Data[idx+3]<<24)) : 0)

    // AV1E_SET_MTU
    if (Size >= 1) {
        val_int = GET_INT(0);
        // Ensure value is within reasonable bounds for MTU (0-64 usually)
        if (val_int > 64) val_int = 64;
        ret = aom_codec_control(ctx, AV1E_SET_MTU, val_int);
        (void)ret; // Ignore return for fuzzing exploration
    }

    // AV1E_SET_QM_U
    if (Size >= 2) {
        val_int = GET_INT(1);
        ret = aom_codec_control(ctx, AV1E_SET_QM_U, val_int);
        (void)ret;
    }

    // AV1E_SET_ENABLE_QM
    if (Size >= 3) {
        val_int = GET_INT(2) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val_int);
        (void)ret;
    }

    // AV1E_SET_MV_COST_UPD_FREQ
    if (Size >= 4) {
        val_int32 = GET_INT32(3);
        // Clamp to reasonable range if needed, but let codec handle it
        ret = aom_codec_control(ctx, AV1E_SET_MV_COST_UPD_FREQ, val_int32);
        (void)ret;
    }

    // AV1E_SET_FP_MT
    if (Size >= 5) {
        val_int = GET_INT(4) ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_FP_MT, val_int);
        (void)ret;
    }

    // AV1E_SET_TIER_MASK
    if (Size >= 6) {
        val_int32 = GET_INT32(5);
        ret = aom_codec_control(ctx, AV1E_SET_TIER_MASK, val_int32);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_292(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Setup dummy file if needed
    setup_dummy_file(Data, Size);

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz config parameters slightly based on input
    cfg.g_w = (Size % 4096) + 16;
    cfg.g_h = (Size % 4096) + 16;
    cfg.g_timebase.num = (Size % 100) + 1;
    cfg.g_timebase.den = (Size % 1000) + 1;
    cfg.g_threads = (Size % 8) + 1;
    cfg.rc_target_bitrate = (Size % 10000) + 100;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzz logic on controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}