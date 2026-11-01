// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"

/* 
 * Note: The functions listed in the task description (e.g., 
 * aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_TX) are internal 
 * or generated wrappers not directly exposed in the public API. 
 * The public API uses aom_codec_control() with specific control IDs 
 * (e.g., AV1E_SET_ENABLE_RECT_TX). 
 * 
 * This fuzzer targets the standard aom_codec_control() function 
 * passing the specific control IDs associated with the requested features 
 * to exercise the underlying logic.
 */

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec) return 0;

    /* 
     * We will attempt to set various encoder controls.
     * The input data is used to generate values for these controls.
     */
    
    /* Control IDs to fuzz based on the task description */
    /* aom_codec_control_id_t is not a standard type, use int or the enum directly */
    const int controls[] = {
        AV1E_SET_ENABLE_RECT_TX,
        AV1E_SET_ENABLE_DUAL_FILTER,
        AV1E_SET_ENABLE_INTERINTRA_WEDGE,
        AV1E_SET_TUNE_CONTENT,
        AV1E_SET_ENABLE_FILTER_INTRA,
        AV1E_SET_ENABLE_DIST_WTD_COMP
    };
    const size_t num_controls = sizeof(controls) / sizeof(controls[0]);

    for (size_t i = 0; i < num_controls; i++) {
        /* Generate a value based on input data if available */
        int32_t value = 0;
        if (size >= 4) {
            value = *(int32_t *)(data + (i * 4 % (size - 3)));
        } else {
            value = (int32_t)(data[0] % 2);
        }

        /* 
         * AV1E_SET_TUNE_CONTENT expects a specific enum, but we fuzz with raw int 
         * to test error handling. 
         * Other controls expect 0 or 1, but we fuzz with arbitrary values.
         */
        aom_codec_control(codec, controls[i], value);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_image_t *img = NULL;
    aom_codec_cx_pkt_t pkt;

    /* Initialize codec context */
    memset(&codec, 0, sizeof(codec));

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Initialize configuration */
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    /* Set some basic config from input data to vary states */
    if (Size > 0) cfg.g_w = (Size % 1024) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    /* Initialize the encoder */
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    /* 
     * Create a dummy frame to ensure the encoder is in a state where 
     * controls might be accepted (though many controls are init-time only).
     * We use a small fixed size image to avoid allocation failures.
     */
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!img) {
        aom_codec_destroy(&codec);
        return 0;
    }

    /* Fill image with input data if possible */
    if (Size > 0) {
        size_t plane_size = img->d_w * img->d_h;
        size_t copy_size = (Size < plane_size) ? Size : plane_size;
        memcpy(img->planes[0], Data, copy_size);
        memcpy(img->planes[1], Data, copy_size);
        memcpy(img->planes[2], Data, copy_size);
    }

    /* Submit the frame */
    /* Fixed type: aom_enc_frame_flags_t */
    aom_enc_frame_flags_t flags = 0;
    /* Fixed arguments: aom_codec_encode takes 5 arguments */
    ret = aom_codec_encode(&codec, img, 0, 1, flags);
    if (ret != AOM_CODEC_OK) {
        /* Even if encoding fails, we might still be able to test controls */
    }

    /* 
     * Fuzz the controls. 
     * Note: Many AV1E_* controls must be called before encoding starts.
     * Calling them after encoding might trigger error paths we want to test.
     */
    fuzz_encoder_controls(&codec, Data, Size);

    /* Try to encode again to see if state changed */
    ret = aom_codec_encode(&codec, img, 1, 1, flags);
    
    /* Drain the encoder */
    while (aom_codec_get_cx_data(&codec, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            /* Do nothing with output, just consume it */
        }
    }

    /* Cleanup */
    aom_img_free(img);
    aom_codec_destroy(&codec);

    return 0;
}