// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/* Helper function to extract int from data safely */
static int get_int(const uint8_t *d, size_t sz, int def) {
    if (sz >= 4) {
        int v = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
        return v;
    }
    return def;
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width = 16, height = 16;
    int fps_num = 30, fps_den = 1;

    if (size >= 2) {
        width = data[0] + 1;
        height = data[1] + 1;
        if (width > 4096 || height > 4096) { width = 16; height = 16; }
    }
    if (size >= 4) {
        fps_num = (data[2] + 1) * (data[3] + 1);
        if (fps_num == 0) fps_num = 30;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fps_num;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t res;
    int val_int;
    int val_int2;
    int val_int3;
    int val_int4;
    int val_int5;
    int val_int6;
    int val_int7;

    if (Size == 0) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    val_int = get_int(Data, Size, 0);
    val_int2 = get_int(Data + 1, Size > 1 ? Size - 1 : 0, 0);
    val_int3 = get_int(Data + 2, Size > 2 ? Size - 2 : 0, 0);
    val_int4 = get_int(Data + 3, Size > 3 ? Size - 3 : 0, 0);
    val_int5 = get_int(Data + 4, Size > 4 ? Size - 4 : 0, 0);
    val_int6 = get_int(Data + 5, Size > 5 ? Size - 5 : 0, 0);
    val_int7 = get_int(Data + 6, Size > 6 ? Size - 6 : 0, 0);

    /* 1. AV1E_SET_FRAME_PERIODIC_BOOST */
    res = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val_int);
    (void)res;

    /* 2. AV1E_SET_ENABLE_DIST_8X8 - Using AV1E_SET_ENABLE_DIST_8X8 if available, 
       otherwise fallback to a known valid constant or skip. 
       Based on error, AV1E_SET_ENABLE_DIST_8X8 might not exist in this version.
       Checking aomcx.h, it seems AV1E_SET_ENABLE_DIST_8X8 is not defined.
       We will use AV1E_SET_ENABLE_DIST_8X8 only if it compiles, but since we can't
       conditionally compile here easily without macros, we skip it if it's not defined.
       However, the user asked to fuzz it. Let's assume the constant name might be 
       different or it's a typo in the task. 
       Looking at the error: 'AV1E_SET_ENABLE_DIST_8X8' is undeclared.
       We will remove the call to this specific constant to fix compilation.
       If the user insists on fuzzing this specific logic, they might need to check 
       the actual constant name in their version of libaom.
       For now, we skip this line to ensure compilation.
       Wait, the task says "fuzz the library libaom using the information provided".
       The information provided lists the function names but the constants might be 
       different. Let's check the aomcx.h for similar names.
       Common AV1 encoder controls:
       AV1E_SET_FRAME_PERIODIC_BOOST
       AV1E_SET_ENABLE_TPL_MODEL
       AV1E_SET_ENABLE_DUAL_FILTER
       AV1E_SET_ENABLE_ANGLE_DELTA
       AV1E_SET_QM_V
       
       AV1E_SET_ENABLE_DIST_8X8 is not in the standard list of AV1 controls in recent libaom.
       It might be a typo for AV1E_SET_ENABLE_DIST_8X8 (which doesn't exist) or 
       perhaps AV1E_SET_ENABLE_DIST_8X8 is a custom or very new/old constant.
       Given the compiler error, we must remove it or replace it with a valid one.
       Since we cannot guess the correct constant, we will skip this specific control
       to ensure the fuzz driver compiles.
    */
    /* Skipped: AV1E_SET_ENABLE_DIST_8X8 due to undeclared identifier */

    /* 3. AV1E_SET_ENABLE_TPL_MODEL */
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val_int3);
    (void)res;

    /* 4. AV1E_SET_ENABLE_DUAL_FILTER */
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val_int4);
    (void)res;

    /* 5. AV1E_SET_ENABLE_ANGLE_DELTA */
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val_int5);
    (void)res;

    /* 6. AV1E_SET_QM_V */
    {
        int qm_val = val_int6;
        res = aom_codec_control(&ctx, AV1E_SET_QM_V, qm_val);
        (void)res;

        if (qm_val > 0 && qm_val <= 100) {
            int buf[100];
            memset(buf, 0, sizeof(buf));
            for (int i = 0; i < qm_val && i < 100; i++) {
                buf[i] = (int)Data[i % Size];
            }
            res = aom_codec_control(&ctx, AV1E_SET_QM_V, buf);
            (void)res;
        }
    }

    /* Additional diversity: try with different values and combinations */
    for (int i = 0; i < 3 && Size > i; i++) {
        int v = (int)Data[i];
        res = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, v);
        (void)res;
        /* Skipped: AV1E_SET_ENABLE_DIST_8X8 */
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, v);
        (void)res;
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, v);
        (void)res;
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, v);
        (void)res;
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}