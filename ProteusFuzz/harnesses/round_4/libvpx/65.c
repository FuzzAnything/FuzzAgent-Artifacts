// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx_codec.h"
#include "vp8cx.h"
#include "vp8dx.h"
#include "vpx_decoder.h"

/* 
 * Note: The functions VP9_SET_BYTE_ALIGNMENT__value, VP9E_SET_DISABLE_LOOPFILTER__value,
 * VP9E_SET_FRAME_PARALLEL_DECODING__value, VP9E_SET_NOISE_SENSITIVITY__value, and
 * VP8E_SET_SCREEN_CONTENT_MODE__value are typically macros or internal constants in libvpx,
 * not direct callable functions. They are usually used as arguments to vpx_codec_control.
 * Since their direct signatures are empty/undefined, this fuzzer focuses on exercising
 * vpx_codec_control_ with various control IDs that correspond to these features,
 * as well as general codec initialization and control flow.
 */

static int try_init_encoder(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface, 
                            vpx_codec_enc_cfg_t *cfg, vpx_codec_flags_t flags) {
    if (ctx == NULL || iface == NULL || cfg == NULL) {
        return -1;
    }

    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, cfg, flags);
    if (res != VPX_CODEC_OK) {
        return -1;
    }

    // Set some basic config to ensure valid state
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 100;

    res = vpx_codec_enc_init(ctx, iface, cfg, flags);
    return (res == VPX_CODEC_OK) ? 0 : -1;
}

static int try_init_decoder(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface,
                            vpx_codec_dec_cfg_t *cfg, vpx_codec_flags_t flags) {
    if (ctx == NULL || iface == NULL || cfg == NULL) {
        return -1;
    }

    // Initialize decoder config manually. 
    // Removed 'lookahead' member as it may not exist in all libvpx versions.
    memset(cfg, 0, sizeof(*cfg));
    cfg->threads = 1;
    // cfg->lookahead = 0; // Removed to fix compilation error

    vpx_codec_err_t res = vpx_codec_dec_init(ctx, iface, cfg, flags);
    return (res == VPX_CODEC_OK) ? 0 : -1;
}

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use the first 4 bytes to determine which codec and mode to try
    uint32_t mode = *(const uint32_t *)Data;
    
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    int init_success = 0;

    // Select interface based on mode bits
    if (mode & 0x1) {
        iface = vpx_codec_vp8_cx();
    } else if (mode & 0x2) {
        iface = vpx_codec_vp8_dx();
    } else if (mode & 0x4) {
        iface = vpx_codec_vp9_cx();
    } else if (mode & 0x8) {
        iface = vpx_codec_vp9_dx();
    } else {
        // Default to VP8 encoder if no specific bit set
        iface = vpx_codec_vp8_cx();
    }

    if (iface == NULL) {
        return 0;
    }

    // Determine if we are testing encoder or decoder based on another bit
    if (mode & 0x10) {
        // Decoder path
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        if (try_init_decoder(&ctx, iface, &dec_cfg, 0) == 0) {
            init_success = 1;
        }
    } else {
        // Encoder path
        memset(&enc_cfg, 0, sizeof(enc_cfg));
        if (try_init_encoder(&ctx, iface, &enc_cfg, 0) == 0) {
            init_success = 1;
        }
    }

    if (!init_success) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Extract control ID and parameters from the remaining data
    // We try to fuzz vpx_codec_control_ with various IDs
    // Common control IDs (these are usually defined in vpx_codec.h or specific headers)
    // Since specific macros like VP9E_SET_DISABLE_LOOPFILTER__value are not callable directly,
    // we attempt to pass them as 'ctrl_id' if they are defined as integers.
    
    // We will try a range of control IDs derived from the input data
    for (size_t i = 4; i + 4 <= Size; i += 4) {
        int ctrl_id = *(const int32_t *)(Data + i);
        
        // Try calling vpx_codec_control_ with the extracted ID
        // We pass a dummy argument to satisfy the variadic nature if needed.
        int dummy_arg = 0;
        if (i + 8 <= Size) {
            dummy_arg = *(const int32_t *)(Data + i + 4);
        }

        // Attempt control call
        // Note: vpx_codec_control_ is variadic. We attempt to call it with a dummy int.
        vpx_codec_err_t res = vpx_codec_control_(&ctx, ctrl_id, dummy_arg);
        
        // We ignore the result for the sake of fuzzing, as the goal is to trigger internal logic
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}