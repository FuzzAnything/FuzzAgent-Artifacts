// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_QM_U at aomcx.h:2048:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING at aomcx.h:2321:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION at aomcx.h:2020:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TILE_ROWS at aomcx.h:1945:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION at aomcx.h:1987:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = Data[0] > 0 ? Data[0] : 64;
    cfg.g_h = Data[1] > 0 ? Data[1] : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    int value;
    if (Size > 2) {
        value = Data[2];
    } else {
        value = 0;
    }

    // Fuzz AV1E_SET_QM_U
    aom_codec_control_typechecked_AV1E_SET_QM_U(&ctx, AV1E_SET_QM_U, (unsigned int)value);

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, (unsigned int)value);

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION(&ctx, AV1E_SET_ENABLE_RESTORATION, (unsigned int)value);

    // Fuzz AV1E_SET_TILE_ROWS
    aom_codec_control_typechecked_AV1E_SET_TILE_ROWS(&ctx, AV1E_SET_TILE_ROWS, (unsigned int)value);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(&ctx, AV1E_SET_ENABLE_INTRABC, value);

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, (int)value);

    aom_codec_destroy(&ctx);

    return 0;
}