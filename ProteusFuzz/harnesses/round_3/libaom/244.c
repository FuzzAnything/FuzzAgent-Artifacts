// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 100;

    int init_ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Use input data to derive control parameters
    int enable_rect_tx = (Size > 0) ? (Data[0] % 2) : 0;
    int enable_cfl_intra = (Size > 1) ? (Data[1] % 2) : 0;
    int enable_intrabc = (Size > 2) ? (Data[2] % 2) : 0;
    int tune_content = (Size > 3) ? (Data[3] % 3) : 0; // Assuming 0, 1, 2 are valid
    int quant_b_adapt = (Size > 4) ? (Data[4] % 2) : 0;
    int intra_default_tx_only = (Size > 5) ? (Data[5] % 2) : 0;

    // Invoke target API functions
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, tune_content);
    aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, quant_b_adapt);
    aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}