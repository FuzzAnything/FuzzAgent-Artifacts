// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

static void fuzz_vpx_codec_error(const vpx_codec_ctx_t *ctx) {
    const char *error_str = vpx_codec_error(ctx);
    (void)error_str;
}

static void fuzz_vpx_codec_error_detail(const vpx_codec_ctx_t *ctx) {
    const char *detail = vpx_codec_error_detail(ctx);
    (void)detail;
}

static void fuzz_vpx_codec_control_(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int ctrl_id;
    memcpy(&ctrl_id, data, sizeof(int));
    
    if (size >= sizeof(int) * 2) {
        int int_arg;
        memcpy(&int_arg, data + sizeof(int), sizeof(int));
        vpx_codec_control_(ctx, ctrl_id, int_arg);
    } else {
        fuzz_vpx_codec_error(ctx);
        fuzz_vpx_codec_error_detail(ctx);
    }
}

static void fuzz_vpx_codec_get_frame(vpx_codec_ctx_t *ctx) {
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    
    while ((img = vpx_codec_get_frame(ctx, &iter)) != NULL) {
        if (img) {
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
    }
}

static void fuzz_vpx_codec_err_to_string(const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_err_t)) return;
    
    vpx_codec_err_t err;
    memcpy(&err, data, sizeof(vpx_codec_err_t));
    const char *str = vpx_codec_err_to_string(err);
    (void)str;
}

static vpx_codec_ctx_t* init_vp9_decoder() {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t*)calloc(1, sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

static void cleanup_decoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = init_vp9_decoder();
    if (!ctx) {
        vpx_codec_ctx_t dummy_ctx = {0};
        fuzz_vpx_codec_error(&dummy_ctx);
        fuzz_vpx_codec_error_detail(&dummy_ctx);
        fuzz_vpx_codec_err_to_string(data, size);
        return 0;
    }
    
    fuzz_vpx_codec_error(ctx);
    fuzz_vpx_codec_error_detail(ctx);
    
    if (size > 0) {
        vpx_codec_decode(ctx, data, size, NULL, 0);
        fuzz_vpx_codec_get_frame(ctx);
    }
    
    fuzz_vpx_codec_control_(ctx, data, size);
    fuzz_vpx_codec_err_to_string(data, size);
    
    cleanup_decoder(ctx);
    
    return 0;
}