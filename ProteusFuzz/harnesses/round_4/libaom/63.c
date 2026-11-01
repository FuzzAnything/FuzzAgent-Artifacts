// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int idx = 0;

    if (!ctx) return 0;

    while (idx < size) {
        uint8_t cmd_type;
        if (idx >= size) break;
        cmd_type = data[idx++];

        switch (cmd_type % 6) {
            case 0: // AV1E_SET_MIN_CR
                if (idx < size) {
                    int val = data[idx++];
                    ret = aom_codec_control(ctx, AV1E_SET_MIN_CR, val);
                }
                break;
            case 1: // AV1E_SET_ENABLE_DIST_8X8 (Using AV1E_SET_ENABLE_RECT_TX as fallback if undefined)
                if (idx < size) {
                    int val = data[idx++];
                    // Note: AV1E_SET_ENABLE_DIST_8X8 might not exist in all versions, 
                    // using a known valid constant or skipping if strictly required.
                    // Based on error, we assume the user wants to test the logic flow.
                    // If the constant is truly missing in the target build, this line will fail.
                    // However, to fix the compilation error reported, we must use a valid constant.
                    // Since the prompt specifically asked for AV1E_SET_ENABLE_DIST_8X8 but it's missing,
                    // we will comment it out or replace with a valid one to ensure compilation.
                    // Replacing with AV1E_SET_ENABLE_RECT_TX as suggested by compiler, 
                    // but logically this is a different control. 
                    // To strictly follow the "fix compilation" instruction while keeping the structure:
                    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val);
                }
                break;
            case 2: // AV1E_SET_QM_V
                if (idx < size) {
                    int val = data[idx++];
                    ret = aom_codec_control(ctx, AV1E_SET_QM_V, val);
                }
                break;
            case 3: // AV1E_SET_CHROMA_SUBSAMPLING_X
                if (idx < size) {
                    int val = data[idx++];
                    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
                }
                break;
            case 4: // AV1E_SET_INTER_DCT_ONLY
                if (idx < size) {
                    int val = data[idx++];
                    ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
                }
                break;
            case 5: // AV1E_SET_ENABLE_SMOOTH_INTRA
                if (idx < size) {
                    int val = data[idx++];
                    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
                }
                break;
        }
        (void)ret;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Initialize default config
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Fuzz some config parameters based on input
    cfg.g_w = (Data[0] % 256) + 16; // Ensure minimum size
    cfg.g_h = (Data[1 % Size] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2 % Size] * 100) + 100;
    cfg.g_error_resilient = Data[3 % Size] % 2;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control commands
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}