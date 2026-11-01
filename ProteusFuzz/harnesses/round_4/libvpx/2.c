// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder = NULL;

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) != 0) {
        return 0;
    }

    // Attempt to initialize encoder context
    encoder = vpx_codec_vp9_cx();
    if (!encoder) {
        return 0;
    }

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != 0) {
        return 0;
    }

    // Parse input data for configuration values
    int disable_loopfilter = (Data[0] % 2);
    int row_mt = (Data[1] % 2);
    int svc = (Data[2] % 2);
    int screen_content_mode = (Data[3] % 2);

    // Invoke VP9E_SET_DISABLE_LOOPFILTER__value
    if (vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter) != 0) {
        // Ignore errors as part of fuzzing
    }

    // Invoke VP9E_SET_ROW_MT__value (first call)
    if (vpx_codec_control(&ctx, VP9E_SET_ROW_MT, row_mt) != 0) {
        // Ignore errors
    }

    // Invoke VP9E_SET_ROW_MT__value (second call with different value)
    int row_mt_alt = (Data[4] % 2);
    if (vpx_codec_control(&ctx, VP9E_SET_ROW_MT, row_mt_alt) != 0) {
        // Ignore errors
    }

    // Invoke VP9E_SET_SVC__value
    if (vpx_codec_control(&ctx, VP9E_SET_SVC, svc) != 0) {
        // Ignore errors
    }

    // Invoke VP9E_SET_SVC_PARAMETERS__value
    // The specific struct type for SVC parameters is not exposed in the provided headers 
    // or is version-dependent. We allocate a generic buffer to pass as a pointer.
    // The size is chosen to be large enough to avoid immediate out-of-bounds if the struct is small.
    size_t svc_param_size = 256; 
    void *svc_params_ptr = malloc(svc_param_size);
    if (svc_params_ptr) {
        memset(svc_params_ptr, 0, svc_param_size);
        // Copy some fuzz data into the buffer to potentially trigger different code paths
        size_t copy_len = (Size > svc_param_size) ? svc_param_size : Size;
        memcpy(svc_params_ptr, Data, copy_len);
        
        if (vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, svc_params_ptr) != 0) {
            // Ignore errors
        }
        free(svc_params_ptr);
    }

    // Invoke VP8E_SET_SCREEN_CONTENT_MODE__value
    if (vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode) != 0) {
        // Ignore errors
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}