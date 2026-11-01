// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdint.h>
#include <stddef.h>
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
 * The provided signatures for the target functions were empty in the prompt.
 * Based on libaom conventions, these functions are typically control commands
 * passed to aom_codec_control(). They take a codec context and an integer value.
 * We will implement the fuzz driver by initializing an encoder and passing
 * the fuzzed data as the integer argument to these control commands.
 */

static void init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return;

    aom_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 1;

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) return;
}

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int val;

    if (Size < sizeof(int)) {
        return 0;
    }

    /* Initialize context and config */
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder(&ctx, &cfg);

    if (ctx.iface == NULL) {
        return 0;
    }

    /* Extract an integer from the fuzzed data to use as the parameter */
    val = *(const int *)Data;

    /*
     * Since the specific function signatures were not provided in the prompt,
     * we assume they follow the standard aom_codec_control pattern:
     * aom_codec_control(ctx, CONTROL_ID, value)
     *
     * We attempt to invoke the controls corresponding to the requested macros.
     * Note: If the macros are not defined in the included headers, this code
     * will fail to compile. The standard macros are used below.
     */

    // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    (void)res;

    // 2. AV1E_SET_DELTALF_MODE
    res = aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val);
    (void)res;

    // 3. AV1E_SET_QM_MIN
    res = aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);
    (void)res;

    // 4. AV1E_SET_DELTAQ_STRENGTH
    res = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val);
    (void)res;

    // 5. AOME_SET_SHARPNESS
    res = aom_codec_control(&ctx, AOME_SET_SHARPNESS, val);
    (void)res;

    // 6. AV1E_SET_MODE_COST_UPD_FREQ
    res = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
    (void)res;

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}