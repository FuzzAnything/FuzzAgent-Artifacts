// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Fuzz driver for libaom decoder control functions.
 * Target APIs:
 * - AOMD_GET_SHOW_FRAME_FLAG
 * - AV1D_GET_FRAME_HEADER_INFO
 * - AV1_COPY_NEW_FRAME_IMAGE
 * - AOMD_GET_FRAME_FLAGS
 * - AOMD_GET_S_FRAME_INFO
 * - AV1_GET_NEW_FRAME_IMAGE
 */

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, arg);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize decoder context */
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    /* Configure decoder with valid fields for the specific libaom version */
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    /* Note: cpu_used and decode_error_resilience are not in this version's struct, omitted */

    /* Get AV1 decoder interface */
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    /* Initialize the decoder */
    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Prepare dummy input data if needed */
    const uint8_t *input_data = Data;
    size_t input_size = Size;

    /* Feed data to decoder to establish state */
    aom_codec_err_t ret = aom_codec_decode(&ctx, input_data, input_size, NULL);
    (void)ret; /* Ignore return value for fuzzing purposes */

    /* Get a frame to ensure we have something to query */
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = aom_codec_get_frame(&ctx, &iter);
    (void)pkt;

    /*
     * Execute Control Calls based on fuzzing input or fixed patterns.
     */

    /* 1. AOMD_GET_SHOW_FRAME_FLAG */
    int show_frame_flag = 0;
    fuzz_decoder_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

    /* 2. AV1D_GET_FRAME_HEADER_INFO */
    unsigned char header_info_dummy[256];
    fuzz_decoder_control(&ctx, AV1D_GET_FRAME_HEADER_INFO, header_info_dummy);

    /* 3. AV1_COPY_NEW_FRAME_IMAGE */
    aom_image_t dummy_img;
    memset(&dummy_img, 0, sizeof(dummy_img));
    dummy_img.w = 16;
    dummy_img.h = 16;
    dummy_img.fmt = AOM_IMG_FMT_I420;
    dummy_img.bit_depth = 8;
    /* Allocate minimal planes to avoid null pointer deref */
    dummy_img.planes[0] = malloc(16 * 16);
    dummy_img.planes[1] = malloc(8 * 8);
    dummy_img.planes[2] = malloc(8 * 8);
    
    if (dummy_img.planes[0] && dummy_img.planes[1] && dummy_img.planes[2]) {
        fuzz_decoder_control(&ctx, AV1_COPY_NEW_FRAME_IMAGE, &dummy_img);
    }
    
    if (dummy_img.planes[0]) free(dummy_img.planes[0]);
    if (dummy_img.planes[1]) free(dummy_img.planes[1]);
    if (dummy_img.planes[2]) free(dummy_img.planes[2]);

    /* 4. AOMD_GET_FRAME_FLAGS */
    int frame_flags = 0;
    fuzz_decoder_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

    /* 5. AOMD_GET_S_FRAME_INFO */
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    fuzz_decoder_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

    /* 6. AV1_GET_NEW_FRAME_IMAGE */
    aom_image_t *new_img_ptr = NULL;
    fuzz_decoder_control(&ctx, AV1_GET_NEW_FRAME_IMAGE, &new_img_ptr);
    if (new_img_ptr) {
        /* Just access to trigger state, do not free */
        (void)new_img_ptr->w;
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}