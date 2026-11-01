// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"

static int setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    // Set minimal config to allow initialization
    cfg->g_w = 16;
    cfg->g_h = 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->rc_end_usage = AOM_CBR;

    err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int ret;

    if (Size == 0) return 0;

    // Initialize codec context
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (setup_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    // Extract data for control arguments
    int val_int = 0;
    if (Size >= sizeof(int)) {
        memcpy(&val_int, Data, sizeof(int));
    } else {
        val_int = (int)Data[0];
    }

    // Fuzz AV1E_SET_QM_U (Quantization Matrix)
    // Passing raw fuzz data as the argument to trigger internal parsing
    const uint8_t *qm_data = (Size >= 256) ? Data : (const uint8_t *)"\0";
    ret = aom_codec_control(&codec, AV1E_SET_QM_U, (void *)qm_data);
    (void)ret;

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    int skip_filter = (val_int != 0) ? 1 : 0;
    ret = aom_codec_control(&codec, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_filter);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    int enable_rest = (val_int & 2) ? 1 : 0;
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_RESTORATION, &enable_rest);
    (void)ret;

    // Fuzz AV1E_SET_TILE_ROWS
    int tile_rows = (val_int % 8) + 1; 
    ret = aom_codec_control(&codec, AV1E_SET_TILE_ROWS, &tile_rows);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    int enable_intrabc = (val_int & 4) ? 1 : 0;
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    int chroma_pos = val_int % 4;
    ret = aom_codec_control(&codec, AV1E_SET_CHROMA_SAMPLE_POSITION, &chroma_pos);
    (void)ret;

    // Cleanup
    cleanup_encoder(&codec);

    return 0;
}