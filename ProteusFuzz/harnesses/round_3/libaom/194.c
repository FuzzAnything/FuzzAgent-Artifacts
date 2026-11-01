// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Uses standard aom_codec_control API with AV1E_* constants.
 */

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, arg);
}

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* Initialize codec context */
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Initialize encoder with default config */
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    /* Parse input data to drive fuzzing logic */
    uint32_t seed = *(const uint32_t *)Data;
    int val = (seed & 0xFF);
    int val2 = ((seed >> 8) & 0xFF);
    int val3 = ((seed >> 16) & 0xFF);
    int val4 = ((seed >> 24) & 0xFF);

    /*
     * 1. AV1E_SET_ENABLE_ONESIDED_COMP
     * Sets a boolean flag.
     */
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, (void *)(intptr_t)(val & 1));

    /*
     * 2. AV1E_SET_ENABLE_SMOOTH_INTRA
     * Sets a boolean flag.
     */
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, (void *)(intptr_t)(val2 & 1));

    /*
     * 3. AV1E_GET_NUM_OPERATING_POINTS
     * Gets an integer value.
     */
    int num_ops = 0;
    fuzz_encoder_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
    (void)num_ops; /* Suppress unused warning */

    /*
     * 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR
     * Sets a film grain test vector.
     */
    if (Size >= 100) {
        fuzz_encoder_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void *)(Data + 4));
    }

    /*
     * 5. AV1E_GET_BASELINE_GF_INTERVAL
     * Gets an integer value.
     */
    int gf_interval = 0;
    fuzz_encoder_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval);
    (void)gf_interval;

    /*
     * 6. AV1E_SET_INTRA_DEFAULT_TX_ONLY
     * Sets a boolean flag.
     */
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, (void *)(intptr_t)(val4 & 1));

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}