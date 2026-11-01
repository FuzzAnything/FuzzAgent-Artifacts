// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        iface = aom_codec_av1_cx();
        if (!iface) {
            cleanup_codec(ctx);
            return 0;
        }
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to configure the encoder
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    aom_codec_err_t res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz various control commands based on input data
    uint8_t command_idx = Data[0] % 6;
    int32_t value = 0;

    // Extract a value from the input data if available
    if (Size > 2) {
        value = (Data[2] << 8) | Data[3];
        value = value % 100; // Limit value to a reasonable range
    }

    switch (command_idx) {
        case 0:
            // aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING
            aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value);
            break;
        case 1:
            // aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT
            aom_codec_control(ctx, AV1E_SET_ENABLE_ORDER_HINT, value);
            break;
        case 2:
            // aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
            break;
        case 3:
            // aom_codec_control_typechecked_AV1E_SET_REDUCED_TX_TYPE_SET
            aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value);
            break;
        case 4:
            // aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
            aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, value);
            break;
        case 5:
            // aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTERINTRA
            aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value);
            break;
        default:
            break;
    }

    cleanup_codec(ctx);
    return 0;
}