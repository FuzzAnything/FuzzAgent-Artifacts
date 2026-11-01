// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices used below
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder settings safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 60) + 1;
    cfg.g_timebase.den = (Data[3] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    aom_codec_err_t err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    int enable_dir_intra = (Data[5] % 2);
    aom_codec_control(&codec, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_dir_intra);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_RENDER_SIZE
    // AV1E_SET_RENDER_SIZE expects a pointer to an int array of size 2: [width, height]
    int render_size[2];
    render_size[0] = (Data[6] % 1024) + 16;
    render_size[1] = (Data[7] % 1024) + 16;
    aom_codec_control(&codec, AV1E_SET_RENDER_SIZE, render_size);

    // Fuzz aom_codec_control_typechecked_AV1E_GET_SEQ_LEVEL_IDX
    int seq_level_idx;
    aom_codec_control(&codec, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_DIAGONAL_INTRA
    int enable_diag_intra = (Data[8] % 2);
    aom_codec_control(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diag_intra);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX
    int enable_flip_idtx = (Data[9] % 2);
    aom_codec_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip_idtx);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_PAETH_INTRA
    // Only access if we have more data available to avoid out-of-bounds
    int enable_paeth_intra = (Size > 10) ? (Data[10] % 2) : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth_intra);

    // Cleanup
    cleanup_codec(&codec);

    return 0;
}