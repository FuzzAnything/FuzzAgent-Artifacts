// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_TILE_ROWS at aomcx.h:1945:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ROW_MT at aomcx.h:1939:1 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG at aomcx.h:2252:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION at aomcx.h:2020:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING at aomcx.h:2321:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

/* Helper to initialize the encoder context with default settings */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;

    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/* Helper to clean up the encoder context */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

/* Helper to read an integer from fuzz input safely */
static int read_int_from_input(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0;
    }
    int val = *(const int32_t *)(data + *offset);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int success = init_encoder_context(&ctx, &cfg);

    if (!success) {
        return 0;
    }

    size_t offset = 0;
    int val = 0;
    unsigned int uval = 0;

    /*
     * The target functions follow the pattern:
     * aom_codec_err_t aom_codec_control_typechecked_##id(aom_codec_ctx_t *ctx, int ctrl, typ data)
     * We must pass the control ID (e.g., AV1E_SET_ENABLE_INTRABC) as the second argument.
     */

    /* 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC (type: int) */
    if (offset + 4 <= Size) {
        val = read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(&ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    /* 2. aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG (type: unsigned int) */
    if (offset + 4 <= Size) {
        uval = (unsigned int)read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_ENABLE_EXT_TILE_DEBUG(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, uval);
    }

    /* 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION (type: unsigned int) */
    if (offset + 4 <= Size) {
        uval = (unsigned int)read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION(&ctx, AV1E_SET_ENABLE_RESTORATION, uval);
    }

    /* 4. aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING (type: unsigned int) */
    if (offset + 4 <= Size) {
        uval = (unsigned int)read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, uval);
    }

    /* 5. aom_codec_control_typechecked_AV1E_SET_TILE_ROWS (type: unsigned int) */
    if (offset + 4 <= Size) {
        uval = (unsigned int)read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_TILE_ROWS(&ctx, AV1E_SET_TILE_ROWS, uval);
    }

    /* 6. aom_codec_control_typechecked_AV1E_SET_ROW_MT (type: unsigned int) */
    if (offset + 4 <= Size) {
        uval = (unsigned int)read_int_from_input(Data, Size, &offset);
        aom_codec_control_typechecked_AV1E_SET_ROW_MT(&ctx, AV1E_SET_ROW_MT, uval);
    }

    cleanup_encoder_context(&ctx);

    return 0;
}