// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int is_valid_ctrl_id(int ctrl_id) {
    // List of known valid control IDs for VP8 and VP9 encoders/decoders
    // Based on vp8cx.h and vp8dx.h definitions
    switch (ctrl_id) {
        case VP8E_SET_CPUUSED:
        case VP8E_SET_STATIC_THRESHOLD:
        case VP8E_SET_TOKEN_PARTITIONS:
        case VP8E_SET_NOISE_SENSITIVITY:
        case VP8E_SET_ENABLEAUTOALTREF:
        case VP8E_SET_ARNR_MAXFRAMES:
        case VP8E_SET_ARNR_STRENGTH:
        case VP8E_SET_ARNR_TYPE:
        case VP8E_SET_ROI_MAP:
        case VP8E_SET_FRAME_FLAGS:
        case VP8E_GET_LAST_QUANTIZER:
        case VP8E_GET_LAST_QUANTIZER_64:
        case VP8E_SET_TUNING:
        case VP9E_SET_TUNE_CONTENT:
        case VPXD_GET_LAST_QUANTIZER:
            return 1;
        default:
            // Allow some fuzzed values to test error handling, but limit range
            if (ctrl_id > 0 && ctrl_id < 10000) return 1;
            return 0;
    }
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx = {0};
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int ctrl_id;
    int arg1, arg2;
    int64_t arg64;
    int valid_ctrl = 0;

    // Extract control ID from input
    ctrl_id = *(int32_t *)Data;
    
    // Extract arguments if available
    if (Size >= 8) {
        arg1 = *(int32_t *)(Data + 4);
        if (Size >= 12) {
            arg2 = *(int32_t *)(Data + 8);
        } else {
            arg2 = 0;
        }
    } else {
        arg1 = 0;
        arg2 = 0;
    }
    
    if (Size >= 16) {
        arg64 = *(int64_t *)(Data + 8);
    } else {
        arg64 = 0;
    }

    // Check if control ID is potentially valid to avoid immediate crashes on invalid enums
    valid_ctrl = is_valid_ctrl_id(ctrl_id);

    // Initialize context minimally
    ctx.name = "fuzz_test";
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;

    // Try to get the VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (iface) {
        // Initialize the codec context
        res = vpx_codec_enc_init(&ctx, iface, NULL, 0);
        if (res != VPX_CODEC_OK) {
            // Initialization failed, but we can still test error paths
        }
    }

    // Test vpx_codec_control_ with various arguments
    // Note: vpx_codec_control_ is a varargs function. We simulate different argument types.
    
    if (valid_ctrl) {
        // Case 1: No extra arguments (for queries like GET_LAST_QUANTIZER)
        // We use a temporary buffer to hold the result if needed, but here we just call it
        // Some controls require a pointer to store the result
        int64_t result64 = 0;
        int result32 = 0;
        
        // Attempt to call with different argument patterns based on common usage
        // Pattern A: No arguments (e.g., some flags)
        res = vpx_codec_control_(&ctx, ctrl_id);
        
        // Pattern B: One int argument
        res = vpx_codec_control_(&ctx, ctrl_id, arg1);
        
        // Pattern C: Two int arguments
        res = vpx_codec_control_(&ctx, ctrl_id, arg1, arg2);
        
        // Pattern D: One int64_t argument (for 64-bit quantizers)
        res = vpx_codec_control_(&ctx, ctrl_id, (long)arg64);
        
        // Pattern E: Pointer to int (common for GET operations)
        res = vpx_codec_control_(&ctx, ctrl_id, &result32);
        
        // Pattern F: Pointer to int64_t
        res = vpx_codec_control_(&ctx, ctrl_id, &result64);
    }

    // Cleanup
    if (ctx.iface) {
        vpx_codec_destroy(&ctx);
    }

    return 0;
}