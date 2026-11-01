// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t err;

    // Guard against empty input
    if (Size == 0) {
        return 0;
    }

    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    // Initialize context structure manually
    memset(&ctx, 0, sizeof(ctx));

    // Get decoder interface (VP9 or VP8)
    // We try VP9 first, fallback to VP8 if needed, but for fuzzing VP9 is standard
    iface = vpx_codec_vp9_dx();
    if (!iface) {
        iface = vpx_codec_vp8_dx();
    }

    if (!iface) {
        return 0;
    }

    // Initialize decoder
    // Use vpx_codec_dec_init_ver directly as per target API
    err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // 1. Test vpx_codec_get_stream_info before decode (should likely fail or return default)
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&ctx, &si);

    // 2. Test vpx_codec_decode with fuzzed data
    // Pass the entire fuzz input as coded data
    err = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    // 3. Test vpx_codec_get_stream_info after decode
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&ctx, &si);

    // 4. Iterate over decoded frames
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame if needed, here we just ensure we don't crash
        if (img->planes[0]) {
            // Optional: check validity or access data
        }
        // Frame is valid until next decode or context free
    }

    // 5. Flush the decoder
    err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    // 6. Get any remaining frames after flush
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}