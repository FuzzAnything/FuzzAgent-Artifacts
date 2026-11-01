// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

/*
 * The target functions listed in the prompt appear to be internal or
 * type-checked wrappers for AV1 encoder controls. In the public libaom API,
 * these correspond to:
 * - AV1E_SET_ENABLE_DIRECTIONAL_INTRA
 * - AV1E_SET_MIN_PARTITION_SIZE
 * - AV1E_SET_ENABLE_PAETH_INTRA
 * - AV1E_SET_ENABLE_MASKED_COMP
 * - AV1E_SET_ENABLE_DIAGONAL_INTRA
 * - AV1E_SET_ENABLE_INTRA_EDGE_FILTER
 *
 * These are invoked via aom_codec_control().
 *
 * Since the prompt states the signatures are empty, we assume the standard
 * aom_codec_control interface which takes an aom_codec_ctx_t* and a control ID.
 */

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    /* Parse input to determine control ID and value */
    int control_id = (int)(Data[0] & 0xFF);
    int value = (int)(Data[1] & 0xFF);

    /*
     * Map the first byte to one of the known AV1 encoder controls.
     * If Data[0] is out of range, we default to a safe control or skip.
     */
    int mapped_control = 0;
    switch (control_id % 6) {
        case 0: mapped_control = AV1E_SET_ENABLE_DIRECTIONAL_INTRA; break;
        case 1: mapped_control = AV1E_SET_MIN_PARTITION_SIZE; break;
        case 2: mapped_control = AV1E_SET_ENABLE_PAETH_INTRA; break;
        case 3: mapped_control = AV1E_SET_ENABLE_MASKED_COMP; break;
        case 4: mapped_control = AV1E_SET_ENABLE_DIAGONAL_INTRA; break;
        case 5: mapped_control = AV1E_SET_ENABLE_INTRA_EDGE_FILTER; break;
        default: mapped_control = AV1E_SET_ENABLE_DIRECTIONAL_INTRA; break;
    }

    /* Initialize encoder context */
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Get the AV1 encoder interface */
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Set up encoder configuration */
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    /* Use input data to vary configuration slightly for diversity */
    cfg.g_w = (Data[2] % 256) + 16;
    cfg.g_h = (Data[3] % 256) + 16;
    cfg.g_timebase.num = (Data[4] % 256) + 1;
    cfg.g_timebase.den = (Data[5] % 256) + 1;
    cfg.rc_target_bitrate = (Data[6] % 256) + 100;

    /* Initialize the encoder */
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /*
     * Invoke the control function with the mapped control ID and value.
     * For AV1E_SET_MIN_PARTITION_SIZE, the value should be a power of 2,
     * but fuzzing allows any int to test boundary/error handling.
     */
    fuzz_encoder_controls(&ctx, mapped_control, value);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}