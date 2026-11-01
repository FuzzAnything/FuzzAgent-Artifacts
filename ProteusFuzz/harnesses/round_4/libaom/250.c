// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_250(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t decoder_ctx;
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&decoder_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_image_t *img;

    /* Feed the fuzzing data as a bitstream packet */
    aom_codec_err_t res = aom_codec_decode(&decoder_ctx, (const unsigned char *)Data, Size, &iter);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(&decoder_ctx);
        return 0;
    }

    /* Iterate through decoded frames to get an image for control calls */
    while ((pkt = aom_codec_get_frame(&decoder_ctx, &iter)) != NULL) {
        if (pkt->kind != AOM_CODEC_CX_FRAME_PKT) {
            continue;
        }
        
        img = &pkt->data.frame;
        if (!img) {
            continue;
        }

        int ret;
        int val;
        aom_image_t *new_img = NULL;
        int frame_size;

        /* 1. AOMD_GET_SHOW_FRAME_FLAG */
        ret = aom_codec_control(&decoder_ctx, AOMD_GET_SHOW_FRAME_FLAG, &val);
        (void)ret; (void)val;

        /* 2. AOMD_GET_SHOW_EXISTING_FRAME_FLAG */
        ret = aom_codec_control(&decoder_ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &val);
        (void)ret; (void)val;

        /* 3. AV1D_SET_SKIP_FILM_GRAIN */
        int skip_film_grain = (Size % 2); 
        ret = aom_codec_control(&decoder_ctx, AV1D_SET_SKIP_FILM_GRAIN, &skip_film_grain);
        (void)ret;

        /* 4. AV1D_GET_FRAME_HEADER_INFO */
        unsigned char dummy_buf[256];
        ret = aom_codec_control(&decoder_ctx, AV1D_GET_FRAME_HEADER_INFO, dummy_buf);
        (void)ret;

        /* 5. AV1_GET_NEW_FRAME_IMAGE */
        ret = aom_codec_control(&decoder_ctx, AV1_GET_NEW_FRAME_IMAGE, &new_img);
        (void)ret; (void)new_img;

        /* 6. AV1D_GET_FRAME_SIZE */
        ret = aom_codec_control(&decoder_ctx, AV1D_GET_FRAME_SIZE, &frame_size);
        (void)ret; (void)frame_size;
    }

    cleanup_codec(&decoder_ctx);
    return 0;
}