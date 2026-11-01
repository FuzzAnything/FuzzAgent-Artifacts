// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int val;

    // Extract parameters from input if possible, otherwise use defaults
    unsigned int width = 128;
    unsigned int height = 128;
    unsigned int fps_num = 30;
    unsigned int fps_den = 1;
    unsigned int threads = 1;
    unsigned int gop_size = 0;

    if (Size >= 8) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        if (width == 0) width = 128;
        if (height == 0) height = 128;
        fps_num = Data[4];
        fps_den = Data[5];
        if (fps_den == 0) fps_den = 1;
    }
    if (Size >= 12) {
        threads = Data[6];
        gop_size = (Data[8] << 8) | Data[9];
    }

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Allocate context
    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = threads;
    // Removed g_cpu_used as it is not a member of aom_codec_enc_cfg
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_lag_in_frames = gop_size;

    // Initialize the codec context
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz the target control functions
    // Note: aom_codec_control takes the control ID directly. 
    // The macros like AV1E_SET_ENABLE_RESTORATION are already the control IDs.
    // We do not use AOM_CODEC_CTRL_ID macro as it is not standard in the public API 
    // for these specific calls in this context, or the macro name might differ.
    // We pass the enum value directly.

    // 1. AV1E_SET_ENABLE_RESTORATION
    if (Size > 10) {
        int enable_restoration = Data[10] & 0x01;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
    }

    // 2. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (Size > 11) {
        int enable_tx_size_search = Data[11] & 0x01;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);
    }

    // 3. AV1E_SET_ENABLE_OVERLAY
    if (Size > 12) {
        int enable_overlay = Data[12] & 0x01;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    }

    // 4. AV1E_SET_TRANSFER_CHARACTERISTICS
    if (Size > 13) {
        int transfer_chars = Data[13] % 10; 
        ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_chars);
    }

    // 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 14) {
        int intra_default_tx_only = Data[14] & 0x01;
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);
    }

    // 6. AV1E_SET_ENABLE_TX64
    if (Size > 15) {
        int enable_tx64 = Data[15] & 0x01;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, enable_tx64);
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}