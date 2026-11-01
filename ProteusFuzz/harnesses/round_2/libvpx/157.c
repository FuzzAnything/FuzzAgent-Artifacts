// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

// Helper function to safely invoke vpx_codec_control_ with a single int argument
static void invoke_control_int(vpx_codec_ctx_t *ctx, int ctrl_id, int value) {
    (void)vpx_codec_control_(ctx, ctrl_id, value);
}

// Helper function to safely invoke vpx_codec_control_ with no extra arguments
static void invoke_control_none(vpx_codec_ctx_t *ctx, int ctrl_id) {
    (void)vpx_codec_control_(ctx, ctrl_id);
}

int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize minimal context to avoid null pointer dereference in control_
    ctx.name = "VP9 Encoder";
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;

    // Fuzz control ID
    int ctrl_id = (int)Data[0] & 0x7FFF;
    
    // Fuzz value argument
    int value = 0;
    if (Size >= 5) {
        memcpy(&value, Data + 1, sizeof(int));
    }

    // List of known VP9 encoder control IDs to test
    // Note: Some macros might not be defined if headers are incomplete, 
    // but we try to use the ones that are standard.
    int known_ctrls[] = {
        VP9E_SET_DISABLE_LOOPFILTER,
        VP9E_SET_FRAME_PARALLEL_DECODING,
        VP9E_GET_LEVEL,
        VP9E_SET_TARGET_LEVEL,
        VP9E_SET_NOISE_SENSITIVITY
    };
    int num_known = sizeof(known_ctrls) / sizeof(known_ctrls[0]);

    // Test known control IDs with fuzzed value
    for (int i = 0; i < num_known; i++) {
        int id = known_ctrls[i];
        // Try with value
        invoke_control_int(&ctx, id, value);
        // Try with 0
        invoke_control_int(&ctx, id, 0);
        // Try with 1
        invoke_control_int(&ctx, id, 1);
        // Try without extra args (for GET commands)
        invoke_control_none(&ctx, id);
    }

    // Test fuzzed control ID with various values
    invoke_control_int(&ctx, ctrl_id, value);
    invoke_control_int(&ctx, ctrl_id, 0);
    invoke_control_int(&ctx, ctrl_id, 1);
    invoke_control_int(&ctx, ctrl_id, -1);
    invoke_control_none(&ctx, ctrl_id);

    // Test with NULL context to ensure error handling is exercised
    invoke_control_int(NULL, ctrl_id, value);
    invoke_control_none(NULL, ctrl_id);

    return 0;
}