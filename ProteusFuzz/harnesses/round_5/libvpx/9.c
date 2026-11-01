// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8.h"
#include "vpx/vpx_decoder.h"

static void set_postproc(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < sizeof(vp8_postproc_cfg_t)) {
        return;
    }
    
    vp8_postproc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    if (size >= sizeof(int)) {
        memcpy(&cfg.post_proc_flag, data, sizeof(int));
    }
    if (size >= 2 * sizeof(int)) {
        memcpy(&cfg.deblocking_level, data + sizeof(int), sizeof(int));
        cfg.deblocking_level = cfg.deblocking_level & 0xFF;
        if (cfg.deblocking_level > 16) cfg.deblocking_level = 16;
    }
    if (size >= 3 * sizeof(int)) {
        memcpy(&cfg.noise_level, data + 2 * sizeof(int), sizeof(int));
        cfg.noise_level = cfg.noise_level & 0xFF;
        if (cfg.noise_level > 16) cfg.noise_level = 16;
    }

    // Use vpx_codec_control with VP8_SET_POSTPROC if available
    #ifdef VP8_SET_POSTPROC
        vpx_codec_control(ctx, VP8_SET_POSTPROC, &cfg);
    #endif
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get the VP8 decoder interface using the function pointer returned by vpx_codec_vp8_dx()
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }
    
    // 1. vpx_codec_iface_name
    const char *name = vpx_codec_iface_name(iface);
    (void)name;

    // 2. vpx_codec_dec_init_ver
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    cfg.w = (Size > 2) ? (Data[0] << 8 | Data[1]) : 16;
    cfg.h = (Size > 4) ? (Data[2] << 8 | Data[3]) : 16;
    cfg.threads = (Size > 5) ? (Data[4] % 8 + 1) : 1;
    
    if (cfg.w == 0) cfg.w = 16;
    if (cfg.h == 0) cfg.h = 16;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // 3. VP8_SET_POSTPROC__value (First call)
    set_postproc(&ctx, Data, Size);

    // 4. VP8_SET_POSTPROC__value (Second call)
    set_postproc(&ctx, Data + (Size / 2), Size / 2);

    // 5. vpx_codec_decode
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    if (res != VPX_CODEC_OK) {
        // Continue to cleanup even on decode error
    }

    // 6. vpx_codec_get_frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    // 7. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    return 0;
}