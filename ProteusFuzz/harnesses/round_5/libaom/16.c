// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    memset(&ctx, 0, sizeof(ctx));
    ctx.iface = iface;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 0;
    
    if (size > 0) cfg.threads = (data[0] % 16) + 1;
    if (size > 1) cfg.allow_lowbitdepth = (data[1] % 2);
    
    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    aom_codec_ctx_t *heap_ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!heap_ctx) {
        aom_codec_destroy(&ctx);
        return NULL;
    }
    memcpy(heap_ctx, &ctx, sizeof(ctx));
    return heap_ctx;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    memset(&ctx, 0, sizeof(ctx));
    ctx.iface = iface;

    ret = aom_codec_enc_config_default(iface, 0, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    if (size > 0) cfg.g_w = (data[0] % 4096) + 16;
    if (size > 1) cfg.g_h = (data[1] % 4096) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 1000) + 1;
    
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    aom_codec_ctx_t *heap_ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!heap_ctx) {
        aom_codec_destroy(&ctx);
        return NULL;
    }
    memcpy(heap_ctx, &ctx, sizeof(ctx));
    return heap_ctx;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int mode = Data[0] % 2;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    if (mode == 0) {
        aom_codec_ctx_t *ctx = init_decoder(payload, payload_size);
        if (!ctx) return 0;

        aom_img_fmt_t fmt;
        aom_codec_control(ctx, AV1D_GET_IMG_FORMAT, &fmt);

        int all_layers = (payload_size > 0) ? payload[0] % 2 : 0;
        aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, all_layers);

        aom_codec_control(ctx, AV1D_GET_FRAME_SIZE, &fmt); 

        aom_codec_control(ctx, AV1D_GET_DISPLAY_SIZE, &fmt);

        int op = (payload_size > 1) ? payload[1] % 128 : 0;
        aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, op);

        aom_codec_decode(ctx, payload, payload_size, NULL);
        
        cleanup_codec(ctx);
    } else {
        aom_codec_ctx_t *ctx = init_encoder(payload, payload_size);
        if (!ctx) return 0;

        int enable = (payload_size > 0) ? payload[0] % 2 : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, enable);

        aom_image_t img;
        int w = (payload_size > 1) ? (payload[1] % 256) + 16 : 16;
        int h = (payload_size > 2) ? (payload[2] % 256) + 16 : 16;
        
        if (aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
            cleanup_codec(ctx);
            return 0;
        }
        
        if (payload_size > 0) {
            // img.planes is an array of pointers to unsigned char
            // stride is a member of the struct pointed to by img.planes[0] is incorrect
            // Actually img.planes[i] is a pointer to unsigned char.
            // The stride is stored in img.stride[i].
            size_t plane_size = img.stride[0] * img.h;
            size_t copy_size = (payload_size > plane_size) ? plane_size : payload_size;
            memcpy(img.planes[0], payload, copy_size);
        }

        // aom_codec_encode signature: (ctx, img, pts, duration, flags)
        aom_codec_encode(ctx, &img, 0, 1, 0);
        aom_img_free(&img);
        cleanup_codec(ctx);
    }

    return 0;
}