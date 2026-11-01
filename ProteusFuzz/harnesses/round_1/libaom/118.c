// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA at aomcx.h:2270:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA at aomcx.h:2270:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA at aomcx.h:2270:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX at aomcx.h:2084:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX at aomcx.h:2084:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX at aomcx.h:2084:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY at aomcx.h:2198:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY at aomcx.h:2198:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY at aomcx.h:2198:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA at aomcx.h:2141:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA at aomcx.h:2141:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA at aomcx.h:2141:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int init_encoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to vary configuration slightly
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    }

    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_err_t res;
    int enable_val = 1;
    int disable_val = 0;

    if (Size < 5) return 0;

    if (!init_encoder(&codec, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(ctx, ctrl, data)
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(&codec, AV1E_SET_ENABLE_FLIP_IDTX, enable_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(&codec, AV1E_SET_ENABLE_FLIP_IDTX, disable_val);
    (void)res;
    if (Size > 5) {
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX(&codec, AV1E_SET_ENABLE_FLIP_IDTX, Data[5] & 1);
        (void)res;
    }

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&codec, AV1E_SET_ENABLE_WARPED_MOTION, enable_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&codec, AV1E_SET_ENABLE_WARPED_MOTION, disable_val);
    (void)res;
    if (Size > 6) {
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&codec, AV1E_SET_ENABLE_WARPED_MOTION, Data[6] & 1);
        (void)res;
    }

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    res = aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY(&codec, AV1E_SET_INTRA_DCT_ONLY, enable_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY(&codec, AV1E_SET_INTRA_DCT_ONLY, disable_val);
    (void)res;
    if (Size > 7) {
        res = aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY(&codec, AV1E_SET_INTRA_DCT_ONLY, Data[7] & 1);
        (void)res;
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA(&codec, AV1E_SET_ENABLE_PAETH_INTRA, enable_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA(&codec, AV1E_SET_ENABLE_PAETH_INTRA, disable_val);
    (void)res;
    if (Size > 8) {
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA(&codec, AV1E_SET_ENABLE_PAETH_INTRA, Data[8] & 1);
        (void)res;
    }

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    // Assuming this takes an integer value representing filter strength or mode
    int loopfilter_val = 0;
    if (Size > 9) {
        loopfilter_val = (int)(Data[9] % 64);
    }
    res = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&codec, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&codec, AV1E_SET_LOOPFILTER_CONTROL, 0);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&codec, AV1E_SET_LOOPFILTER_CONTROL, 63);
    (void)res;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_val);
    (void)res;
    res = aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, disable_val);
    (void)res;
    if (Size > 10) {
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, Data[10] & 1);
        (void)res;
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}