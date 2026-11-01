// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE at aomcx.h:2165:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP at aomcx.h:2090:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE at aomcx.h:1999:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION at aomcx.h:1987:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES at aomcx.h:1978:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS at aomcx.h:1981:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_301(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    unsigned int val_ui;
    int val_i;

    if (Size < 2) return 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg) != 0) return 0;

    val_ui = (unsigned int)Data[0];
    val_i = (int)Data[1];

    // Fuzz AV1E_SET_DELTALF_MODE (unsigned int)
    ret = aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE(&codec, AV1E_SET_DELTALF_MODE, val_ui);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP (int)
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP(&codec, AV1E_SET_ENABLE_DIST_WTD_COMP, val_i);
    (void)ret;

    // Fuzz AV1E_SET_COLOR_RANGE (int)
    ret = aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE(&codec, AV1E_SET_COLOR_RANGE, val_i);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION (int)
    ret = aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION(&codec, AV1E_SET_CHROMA_SAMPLE_POSITION, val_i);
    (void)ret;

    // Fuzz AV1E_SET_COLOR_PRIMARIES (int)
    ret = aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES(&codec, AV1E_SET_COLOR_PRIMARIES, val_i);
    (void)ret;

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS (int)
    ret = aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS(&codec, AV1E_SET_TRANSFER_CHARACTERISTICS, val_i);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}