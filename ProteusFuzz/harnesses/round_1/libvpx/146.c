// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>

/* Dummy callback functions for external frame buffer matching libvpx signatures */
/* Note: vpx_codec_frame_buffer is an opaque struct, must use 'struct' tag in C */
static int dummy_get_frame_buffer(void *cb_priv, unsigned long size, struct vpx_codec_frame_buffer *buf) {
    (void)cb_priv;
    (void)size;
    (void)buf;
    return -1; /* Return failure to simulate inability to provide buffer */
}

static int dummy_release_frame_buffer(void *cb_priv, struct vpx_codec_frame_buffer *buf) {
    (void)cb_priv;
    (void)buf;
    return 0;
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_dec_cfg_t cfg;
    vpx_fixed_buf_t buf;
    uint8_t *temp_buf = NULL;
    size_t temp_size = 0;
    vpx_image_t *frame = NULL;
    const char *err_str = NULL;
    vpx_codec_err_t res;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Initialize configuration */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;

    /* Initialize decoder context */
    res = vpx_codec_dec_init_ver(&ctx, vpx_codec_vp8_dx(), &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, try to get error string with NULL ctx to test that path */
        err_str = vpx_codec_error(NULL);
        (void)err_str;
        return 0;
    }

    /* Test vpx_codec_set_frame_buffer_functions with correct signatures */
    res = vpx_codec_set_frame_buffer_functions(&ctx, dummy_get_frame_buffer, dummy_release_frame_buffer, NULL);
    (void)res;

    /* Prepare data for decoding if available */
    const uint8_t *decode_data = Data;
    unsigned int decode_sz = (unsigned int)Size;

    /* Fuzz decode operation */
    /* Pass data */
    res = vpx_codec_decode(&ctx, decode_data, decode_sz, NULL, 0);
    (void)res;

    /* Flush decoder */
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    (void)res;

    /* Iterate over frames */
    while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Frame is valid here, do nothing or inspect */
        (void)frame;
    }

    /* Test vpx_codec_error after potential decode errors */
    err_str = vpx_codec_error(&ctx);
    (void)err_str;

    /* Test vpx_codec_set_cx_data_buf (Encoder function, but we test it on decoder context for fuzzing) */
    /* Allocate a small buffer for testing */
    temp_size = Size > 1024 ? 1024 : Size;
    temp_buf = (uint8_t *)malloc(temp_size);
    if (temp_buf) {
        memset(temp_buf, 0, temp_size);
        buf.buf = temp_buf;
        buf.sz = temp_size;
        
        /* Try setting custom buffer with various padding */
        unsigned int pad_before = (Size % 100);
        unsigned int pad_after = (Size % 100);
        
        res = vpx_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
        (void)res;

        /* Restore default by passing NULL */
        res = vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
        (void)res;

        free(temp_buf);
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}