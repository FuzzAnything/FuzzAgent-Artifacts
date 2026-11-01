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
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

/*
 * Fuzz driver for libaom AV1 encoder control commands.
 * Targets:
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 * - AV1E_SET_ENABLE_AB_PARTITIONS
 * - AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
 * - AV1E_SET_MAX_PARTITION_SIZE
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_ENABLE_CHROMA_DELTAQ
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, int width, int height) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width > 0 ? width : 128;
    cfg.g_h = height > 0 ? height : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static int get_control_cmd_from_data(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    uint8_t cmd = data[0] % 6;
    return cmd;
}

static int get_param_value_from_data(const uint8_t *data, size_t size, int cmd) {
    if (size < 2) return 0;
    uint16_t val = (data[1] << 8) | data[2];
    
    // Clamp values based on command type for safety
    if (cmd == 0 || cmd == 1 || cmd == 4 || cmd == 5) {
        // Boolean flags (0 or 1)
        return (val % 2);
    } else if (cmd == 2) {
        // Max consecutive frame drop ms (reasonable range 0-10000)
        return val % 10001;
    } else if (cmd == 3) {
        // Max partition size (reasonable range 4-64)
        return 4 + (val % 61);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Derive dimensions from input data
    int width = (Data[Size - 2] % 100) + 16;
    int height = (Data[Size - 1] % 100) + 16;
    
    // Ensure dimensions are multiples of 8 (AV1 requirement)
    width = (width / 8) * 8;
    height = (height / 8) * 8;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    aom_codec_ctx_t *ctx = init_encoder(iface, width, height);
    if (!ctx) return 0;

    int cmd_idx = get_control_cmd_from_data(Data, Size);
    int param = get_param_value_from_data(Data, Size, cmd_idx);

    int result = 0;
    switch (cmd_idx) {
        case 0: // AV1E_SET_ENABLE_INTERINTER_WEDGE
            result = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, param);
            break;
        case 1: // AV1E_SET_ENABLE_AB_PARTITIONS
            result = aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, param);
            break;
        case 2: // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
            result = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, param);
            break;
        case 3: // AV1E_SET_MAX_PARTITION_SIZE
            result = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, param);
            break;
        case 4: // AV1E_SET_RTC_EXTERNAL_RC
            result = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, param);
            break;
        case 5: // AV1E_SET_ENABLE_CHROMA_DELTAQ
            result = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, param);
            break;
        default:
            break;
    }

    // Try to invoke multiple commands in sequence to explore state
    if (Size > 4) {
        int cmd_idx2 = (Data[3] % 6);
        int param2 = get_param_value_from_data(Data, Size, cmd_idx2);
        
        switch (cmd_idx2) {
            case 0: aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, param2); break;
            case 1: aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, param2); break;
            case 2: aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, param2); break;
            case 3: aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, param2); break;
            case 4: aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, param2); break;
            case 5: aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, param2); break;
            default: break;
        }
    }

    cleanup_codec(ctx);
    return 0;
}