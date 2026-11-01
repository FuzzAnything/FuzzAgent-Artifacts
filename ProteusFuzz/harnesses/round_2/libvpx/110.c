// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

static void static_put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
}

static void static_put_slice_cb(void *user_priv, const vpx_image_t *frame,
                                const vpx_image_rect_t *slice_rect,
                                const vpx_image_rect_t *frame_rect) {
    (void)user_priv;
    (void)frame;
    (void)slice_rect;
    (void)frame_rect;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    const char *err_str;

    if (Size < 2) {
        return 0;
    }

    // Select interface based on first byte
    uint8_t iface_selector = Data[0];
    if (iface_selector == 0) {
        iface = vpx_codec_vp8_dx();
    } else if (iface_selector == 1) {
        iface = vpx_codec_vp9_dx();
    } else {
        // Try to get a valid interface or skip if none available
        iface = vpx_codec_vp8_dx();
        if (!iface) iface = vpx_codec_vp9_dx();
    }

    if (!iface) {
        return 0;
    }

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (Data[1] % 4) + 1; // 1 to 4 threads
    cfg.w = 0; // Let decoder determine
    cfg.h = 0; // Let decoder determine

    // Initialize decoder context
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // If init fails, try to test error handling functions anyway
        err_str = vpx_codec_err_to_string(res);
        (void)err_str;
        return 0;
    }

    // Register callbacks
    // Use user_priv from input data if available, otherwise NULL
    void *user_priv = (Size > 2) ? (void *)(uintptr_t)(Data[2]) : NULL;
    
    res = vpx_codec_register_put_frame_cb(&ctx, static_put_frame_cb, user_priv);
    (void)res;

    res = vpx_codec_register_put_slice_cb(&ctx, static_put_slice_cb, user_priv);
    (void)res;

    // Decode data
    // Use remaining data as input
    const uint8_t *decode_data = (Size > 2) ? Data + 2 : NULL;
    unsigned int decode_size = (Size > 2) ? (unsigned int)(Size - 2) : 0;
    
    // Ensure we don't pass NULL with non-zero size
    if (decode_data == NULL) {
        decode_size = 0;
    }

    // Perform decode
    res = vpx_codec_decode(&ctx, decode_data, decode_size, user_priv, 0);
    (void)res;

    // Flush decoder
    res = vpx_codec_decode(&ctx, NULL, 0, user_priv, 0);
    (void)res;

    // Test error string conversion with various potential error codes
    err_str = vpx_codec_err_to_string(VPX_CODEC_OK);
    (void)err_str;
    err_str = vpx_codec_err_to_string(VPX_CODEC_ERROR);
    (void)err_str;
    err_str = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)err_str;
    err_str = vpx_codec_err_to_string(VPX_CODEC_INCAPABLE);
    (void)err_str;
    err_str = vpx_codec_err_to_string(VPX_CODEC_INVALID_PARAM);
    (void)err_str;

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}