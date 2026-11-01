// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    if (size < 4) return NULL;

    iface = vpx_codec_vp8_cx();
    if (!iface) return NULL;

    ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static vpx_codec_ctx_t *create_decoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;

    if (size < 4) return NULL;

    iface = vpx_codec_vp8_dx();
    if (!iface) return NULL;

    ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    res = vpx_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test vpx_codec_error with NULL context
    const char *err_msg = vpx_codec_error(NULL);
    (void)err_msg;

    // Test Encoder Path
    vpx_codec_ctx_t *enc_ctx = create_encoder_context(Data, Size);
    if (enc_ctx) {
        vpx_codec_iter_t iter = NULL;
        vpx_image_t img;
        memset(&img, 0, sizeof(img));
        img.w = (Data[0] % 16) + 16;
        img.h = (Data[1] % 16) + 16;
        img.fmt = VPX_IMG_FMT_I420;
        img.bit_depth = 8;
        img.stride[0] = img.w;
        img.planes[0] = (unsigned char *)Data; // Fuzz data as plane

        // Encode a frame
        vpx_codec_encode(enc_ctx, &img, 0, 1, 0, 0);
        
        // Get preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(enc_ctx);
        (void)preview;

        // Iterate encoded data
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(enc_ctx, &iter)) != NULL) {
            // Consume packet data
        }

        // Flush encoder
        vpx_codec_encode(enc_ctx, NULL, 0, 0, 0, 0);
        iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(enc_ctx, &iter)) != NULL) {
            // Consume packet data
        }

        // Check error after potential failure
        err_msg = vpx_codec_error(enc_ctx);
        (void)err_msg;

        cleanup_context(enc_ctx);
    }

    // Test Decoder Path
    vpx_codec_ctx_t *dec_ctx = create_decoder_context(Data, Size);
    if (dec_ctx) {
        vpx_codec_iter_t iter = NULL;
        
        // Decode data
        vpx_codec_decode(dec_ctx, Data, Size, NULL, 0);

        // Iterate decoded frames
        vpx_image_t *frame;
        while ((frame = vpx_codec_get_frame(dec_ctx, &iter)) != NULL) {
            // Frame processing
        }

        // Flush decoder
        vpx_codec_decode(dec_ctx, NULL, 0, NULL, 0);
        iter = NULL;
        while ((frame = vpx_codec_get_frame(dec_ctx, &iter)) != NULL) {
            // Frame processing
        }

        // Check error
        err_msg = vpx_codec_error(dec_ctx);
        (void)err_msg;

        cleanup_context(dec_ctx);
    }

    return 0;
}