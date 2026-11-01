// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_ENABLE_OVERLAY
    if (size >= 1) {
        int val = data[0] & 0x1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, val);
    }

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    if (size >= 2) {
        int val = data[1] & 0x1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    }

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    if (size >= 3) {
        int val = data[2] & 0x7; // Limit to valid range roughly
        aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    }

    // AV1E_SET_ENABLE_PALETTE
    if (size >= 4) {
        int val = data[3] & 0x1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
    }

    // AV1E_GET_NUM_OPERATING_POINTS
    if (size >= 5) {
        int val = 0;
        aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &val);
        (void)val; // Use the value to avoid unused warnings if needed
    }

    // AV1E_SET_CHROMA_SAMPLE_POSITION
    if (size >= 6) {
        int val = data[5] & 0x3; // Limit to valid range roughly
        aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize config
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Use input data to configure some basic settings
    cfg.g_w = (Data[0] % 255) + 16; // Ensure minimum width
    cfg.g_h = (Data[1] % 255) + 16; // Ensure minimum height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic for control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}