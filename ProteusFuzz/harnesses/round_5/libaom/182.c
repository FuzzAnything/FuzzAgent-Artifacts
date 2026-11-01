// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;
    int num_op = 0;

    if (size > 0) {
        val = data[0];
    }
    if (size > 1) {
        num_op = data[1];
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);
    (void)ret;

    // AV1E_GET_NUM_OPERATING_POINTS
    ret = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_op);
    (void)ret;

    // AV1E_SET_ENABLE_REF_FRAME_MVS
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
    (void)ret;

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
    (void)ret;

    // AV1E_SET_ALLOW_REF_FRAME_MVS
    ret = aom_codec_control(ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, val);
    (void)ret;

    // AV1E_SET_ENABLE_SMOOTH_INTRA
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (Size == 0) {
        return 0;
    }

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to set some config parameters
    cfg.g_w = (Size > 0) ? (Data[0] % 2048 + 16) : 16;
    cfg.g_h = (Size > 1) ? (Data[1] % 2048 + 16) : 16;
    cfg.g_timebase.num = (Size > 2) ? (Data[2] % 100 + 1) : 1;
    cfg.g_timebase.den = (Size > 3) ? (Data[3] % 100 + 1) : 1;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] % 10000 + 100) : 100;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize the codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}