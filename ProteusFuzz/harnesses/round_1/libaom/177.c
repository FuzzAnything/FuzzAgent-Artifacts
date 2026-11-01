// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required Header Files */
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/* Helper to initialize the codec context */
static int init_codec(const uint8_t *data, size_t size, aom_codec_ctx_t **ctx) {
    const aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    
    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    /* Allocate the codec context dynamically to avoid stack overflow */
    *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!*ctx) return -1;

    if (aom_codec_dec_init(*ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(*ctx);
        *ctx = NULL;
        return -1;
    }

    /* If input data is provided, try to decode a frame to set up state */
    if (data && size > 0) {
        aom_codec_decode(*ctx, data, size, NULL);
        
        /* Try to get a frame to ensure the decoder is in a valid state */
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt_out = aom_codec_get_frame(*ctx, &iter);
        (void)pkt_out;
    }

    return 0;
}

/* Helper to cleanup */
static void cleanup_codec(aom_codec_ctx_t **ctx) {
    if (*ctx) {
        aom_codec_destroy(*ctx);
        free(*ctx);
        *ctx = NULL;
    }
}

/* Helper to perform a control call safely */
static int do_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, control_id, arg);
}

int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *codec = NULL;

    /* Initialize codec */
    if (init_codec(Data, Size, &codec) != 0) {
        return 0;
    }

    /* Ensure we have a valid codec context */
    if (!codec) {
        return 0;
    }

    /* 
     * Fuzzing Strategy:
     * We will attempt to call the 6 target control functions using the 
     * standard aom_codec_control API with the corresponding constants.
     */

    /* 1. AOMD_GET_SHOW_FRAME_FLAG */
    int show_frame_flag = 0;
    do_control(codec, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

    /* 2. AV1D_GET_FRAME_HEADER_INFO */
    /* We pass a dummy buffer. The API expects a pointer to a struct, 
       but without the struct definition, we pass a buffer to test robustness. */
    uint8_t header_info_buf[256];
    memset(header_info_buf, 0, sizeof(header_info_buf));
    do_control(codec, AV1D_GET_FRAME_HEADER_INFO, header_info_buf);

    /* 3. AV1_GET_NEW_FRAME_IMAGE */
    aom_image_t *img = NULL;
    do_control(codec, AV1_GET_NEW_FRAME_IMAGE, &img);

    /* 4. AOMD_GET_SHOW_EXISTING_FRAME_FLAG */
    int show_existing = 0;
    do_control(codec, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing);

    /* 5. AOMD_GET_BASE_Q_IDX */
    int base_q_idx = 0;
    do_control(codec, AOMD_GET_BASE_Q_IDX, &base_q_idx);

    /* 6. AV1D_SET_SKIP_FILM_GRAIN */
    int skip_film_grain = (Size > 0) ? (Data[0] % 2) : 0;
    do_control(codec, AV1D_SET_SKIP_FILM_GRAIN, &skip_film_grain);

    /* Additional exploration: Try to decode more data if available */
    if (Size > 10) {
        aom_codec_decode(codec, Data, Size, NULL);
        aom_codec_iter_t iter = NULL;
        while (aom_codec_get_frame(codec, &iter)) {
            /* Consume frames to keep decoder state clean */
        }
    }

    /* Cleanup */
    cleanup_codec(&codec);

    return 0;
}