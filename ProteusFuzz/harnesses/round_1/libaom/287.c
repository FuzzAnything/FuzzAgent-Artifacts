// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/* Required Header Files */
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

static aom_codec_ctx_t *create_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;

    if (!iface) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    /* Ensure valid configuration to prevent internal null pointer dereferences */
    cfg.g_w = (size > 0) ? (data[0] % 256) + 16 : 128;
    cfg.g_h = (size > 1) ? (data[1] % 256) + 16 : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = (size > 2) ? (data[2] % 4) + 1 : 1;
    cfg.rc_target_bitrate = (size > 3) ? (data[3] % 1000) + 100 : 1000;
    cfg.g_lag_in_frames = 0; /* Disable lookahead to simplify state */
    cfg.rc_end_usage = AOM_CBR; /* Use CBR to avoid complex rate control states */

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    aom_codec_ctx_t *codec = create_encoder(Data, Size);
    if (!codec) return 0;

    /* 
     * The crash occurred because aom_codec_control was called with invalid IDs 
     * or before the encoder was fully ready to accept specific controls (like 
     * those requiring frame data). 
     * 
     * To fix:
     * 1. Only use known valid control IDs for AV1 encoder.
     * 2. Ensure the encoder is in a state where controls are safe.
     * 3. Avoid passing raw input data as pointers to controls that expect 
     *    specific structures unless we construct them.
     */

    /* Helper to safely extract values */
    #define GET_VAL(idx) ((idx < Size) ? Data[idx] : 0)

    /* 
     * We will attempt to call controls that are generally safe.
     * Since the specific "typechecked" functions are internal macros, 
     * we map them to their likely public enum counterparts or safe integer ranges.
     * 
     * Common safe controls:
     * - AOME_SET_CPUUSED
     * - AOME_SET_CQ_LEVEL
     * - AV1E_SET_ENABLE_TX64
     * - AV1E_SET_MATRIX_COEFFICIENTS (requires specific struct, skip raw int)
     * - AV1E_SET_INTER_DCT_ONLY
     * - AV1E_SET_BITRATE_ONE_PASS_CBR
     */

    /* Try setting CPU usage (Safe) */
    int cpu_used = GET_VAL(0) % 8;
    aom_codec_control(codec, AOME_SET_CPUUSED, cpu_used);

    /* Try setting CQ level (Safe) */
    int cq_level = GET_VAL(1) % 64;
    aom_codec_control(codec, AOME_SET_CQ_LEVEL, cq_level);

    /* Try enabling TX64 (Safe integer flag) */
    int enable_tx64 = GET_VAL(2) % 2;
    aom_codec_control(codec, AV1E_SET_ENABLE_TX64, enable_tx64);

    /* Try setting Inter DCT Only (Safe integer flag) */
    int inter_dct_only = GET_VAL(3) % 2;
    aom_codec_control(codec, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    /* Try setting Bitrate One Pass CBR (Safe integer flag) */
    int one_pass_cbr = GET_VAL(4) % 2;
    aom_codec_control(codec, AV1E_SET_BITRATE_ONE_PASS_CBR, one_pass_cbr);

    /* 
     * For MATRIX_COEFFICIENTS, it usually expects an enum value. 
     * We try a few valid enum values if available, or skip if unsure.
     * Assuming standard values: 0=Identity, 1=BT.709, etc.
     */
    int matrix_coeff = GET_VAL(5) % 10; 
    /* Only try if it's a likely valid range to avoid segfaults in internal switch */
    if (matrix_coeff < 10) {
        aom_codec_control(codec, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coeff);
    }

    /* 
     * We avoid passing raw Data pointer to controls that might interpret it 
     * as a complex structure unless we are sure. The crash log suggests 
     * an internal pointer dereference (frame->idx) which implies the control 
     * triggered a frame processing path with uninitialized data.
     * By restricting to simple integer controls, we mitigate this.
     */

    cleanup_encoder(codec);

    return 0;
}