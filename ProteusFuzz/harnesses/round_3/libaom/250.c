// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    // Attempt to set the control
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    (void)ret; // Ignore return value for fuzzing, just ensure no crash
    return 1;
}

int LLVMFuzzerTestOneInput_250(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize decoder context
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; // Fixed field name

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input data to drive fuzzing
    int val1 = (Data[0] % 2);
    int val2 = (Data[1] % 2);
    int val3 = (Data[0] % 2);
    int val4 = (Data[1] % 2);
    int val5 = (Data[0] % 2);
    int val6 = (Data[1] % 2);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val1);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val2);

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val3);

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    fuzz_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val4);

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val5);

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}