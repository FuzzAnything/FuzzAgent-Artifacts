// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* Required Header Files */
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

/* Helper function to safely extract integers from fuzzing input */
static int extract_int_from_input(const uint8_t *data, size_t size, size_t *offset, int min_val, int max_val) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    
    if (val < min_val) val = min_val;
    if (val > max_val) val = max_val;
    return val;
}

/* Helper function to safely extract unsigned integers from fuzzing input */
static unsigned int extract_uint_from_input(const uint8_t *data, size_t size, size_t *offset, unsigned int min_val, unsigned int max_val) {
    if (*offset + sizeof(unsigned int) > size) {
        return 0;
    }
    unsigned int val;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    
    if (val < min_val) val = min_val;
    if (val > max_val) val = max_val;
    return val;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *encoder_iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int ret;

    /* Initialize context structure manually to avoid uninitialized memory issues */
    memset(&ctx, 0, sizeof(ctx));

    /* Get AV1 encoder interface */
    encoder_iface = aom_codec_av1_cx();
    if (encoder_iface == NULL) {
        return 0;
    }

    /* Initialize encoder configuration */
    err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /* Set some basic configuration from input if available */
    size_t offset = 0;
    if (Size > 0) {
        cfg.g_w = extract_uint_from_input(Data, Size, &offset, 16, 4096);
        cfg.g_h = extract_uint_from_input(Data, Size, &offset, 16, 4096);
        /* Fixed: g_timebase is a struct aom_rational, not g_time_base */
        cfg.g_timebase.num = extract_uint_from_input(Data, Size, &offset, 1, 1000000);
        cfg.g_timebase.den = extract_uint_from_input(Data, Size, &offset, 1, 1000000);
        cfg.g_threads = extract_uint_from_input(Data, Size, &offset, 0, 16);
        cfg.rc_target_bitrate = extract_int_from_input(Data, Size, &offset, 1, 10000000);
    }

    /* Initialize the codec context */
    err = aom_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC */
    /* Expected: int value (0 or 1) */
    if (offset + sizeof(int) <= Size) {
        int enable_intrabc;
        memcpy(&enable_intrabc, Data + offset, sizeof(int));
        offset += sizeof(int);
        /* Fixed: Use the enum value directly as the control ID, no AOM_CODEC_CTRL_ID macro needed */
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
        (void)ret; /* Suppress unused variable warning */
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_REFERENCE_FRAMES */
    /* Expected: int value (0 to 7 typically) */
    if (offset + sizeof(int) <= Size) {
        int max_ref_frames = extract_int_from_input(Data, Size, &offset, 0, 7);
        ret = aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);
        (void)ret;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR */
    /* Expected: int value */
    if (offset + sizeof(int) <= Size) {
        int max_consec_drop = extract_int_from_input(Data, Size, &offset, 0, 100);
        ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_consec_drop);
        (void)ret;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_RATE_DISTRIBUTION_INFO */
    /* This typically expects a pointer to a struct, but we'll try with a dummy int for fuzzing */
    if (offset + sizeof(int) <= Size) {
        int dummy_rate_info = extract_int_from_input(Data, Size, &offset, 0, 1000);
        ret = aom_codec_control(&ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, dummy_rate_info);
        (void)ret;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_TARGET_SEQ_LEVEL_IDX */
    /* Expected: int value */
    if (offset + sizeof(int) <= Size) {
        int seq_level_idx = extract_int_from_input(Data, Size, &offset, 0, 100);
        ret = aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, seq_level_idx);
        (void)ret;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY */
    /* Expected: int value */
    if (offset + sizeof(int) <= Size) {
        int noise_sensitivity = extract_int_from_input(Data, Size, &offset, 0, 100);
        ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
        (void)ret;
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}