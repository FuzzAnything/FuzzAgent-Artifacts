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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 */

static int get_int_val(const uint8_t *d, size_t s, int def) {
    if (s >= 1) return (int)d[0];
    return def;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    const aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    /* Use the standard interface getter for AV1 encoder */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return NULL;
    }

    /* Use fuzz input to configure width/height if possible, ensuring validity */
    if (size >= 2) {
        uint16_t w = data[0];
        uint16_t h = data[1];
        if (w < 16) w = 16;
        if (h < 16) h = 16;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_usage = AOM_USAGE_REALTIME;

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) {
        return NULL;
    }
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t *codec = init_encoder(Data, Size);
    if (!codec) {
        return 0;
    }

    /*
     * Map fuzz data to control values.
     * We will attempt to call the control functions with various values
     * derived from the input buffer.
     */

    int palette_val = get_int_val(Data, Size, 0);
    int wedge_val = get_int_val(Size > 1 ? Data + 1 : Data, Size > 1 ? Size - 1 : 0, 0);
    int edge_filter_val = get_int_val(Size > 2 ? Data + 2 : Data, Size > 2 ? Size - 2 : 0, 0);
    int filter_intra_val = get_int_val(Size > 3 ? Data + 3 : Data, Size > 3 ? Size - 3 : 0, 0);
    int tune_content_val = get_int_val(Size > 4 ? Data + 4 : Data, Size > 4 ? Size - 4 : 0, 0);
    int rect_tx_val = get_int_val(Size > 5 ? Data + 5 : Data, Size > 5 ? Size - 5 : 0, 0);

    /*
     * Invoke control functions.
     * Note: Since the exact macro names for the control IDs are not provided
     * in the prompt (signatures were empty), we use the standard AV1 control IDs
     * that correspond to these features. If the typechecked macros resolve to
     * these IDs, this will work.
     *
     * AV1E_SET_ENABLE_PALETTE
     * AV1E_SET_ENABLE_INTERINTER_WEDGE
     * AV1E_SET_ENABLE_INTRA_EDGE_FILTER
     * AV1E_SET_ENABLE_FILTER_INTRA
     * AV1E_SET_TUNE_CONTENT
     * AV1E_SET_ENABLE_RECT_TX
     */

    /* Try AV1E_SET_ENABLE_PALETTE */
    aom_codec_control(codec, AV1E_SET_ENABLE_PALETTE, palette_val);

    /* Try AV1E_SET_ENABLE_INTERINTER_WEDGE */
    aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, wedge_val);

    /* Try AV1E_SET_ENABLE_INTRA_EDGE_FILTER */
    aom_codec_control(codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, edge_filter_val);

    /* Try AV1E_SET_ENABLE_FILTER_INTRA */
    aom_codec_control(codec, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra_val);

    /* Try AV1E_SET_TUNE_CONTENT */
    aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, tune_content_val);

    /* Try AV1E_SET_ENABLE_RECT_TX */
    aom_codec_control(codec, AV1E_SET_ENABLE_RECT_TX, rect_tx_val);

    /*
     * Also try with boundary values to explore more states
     */
    aom_codec_control(codec, AV1E_SET_ENABLE_PALETTE, 1);
    aom_codec_control(codec, AV1E_SET_ENABLE_PALETTE, 0);
    aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, 1);
    aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, 0);
    aom_codec_control(codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, 1);
    aom_codec_control(codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, 0);
    aom_codec_control(codec, AV1E_SET_ENABLE_FILTER_INTRA, 1);
    aom_codec_control(codec, AV1E_SET_ENABLE_FILTER_INTRA, 0);
    aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, 0);
    aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, 1);
    aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, 2);
    aom_codec_control(codec, AV1E_SET_ENABLE_RECT_TX, 1);
    aom_codec_control(codec, AV1E_SET_ENABLE_RECT_TX, 0);

    cleanup_encoder(codec);

    return 0;
}