// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_QM_U at aomcx.h:2048:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING at aomcx.h:1951:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_Y at aomcx.h:2192:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF at aomcx.h:2017:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_X at aomcx.h:2189:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

/*
 * Fuzz driver for libaom encoder control functions.
 * Target APIs:
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
 * - aom_codec_control_typechecked_AV1E_SET_QM_U
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING
 * - aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_Y
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF
 * - aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_X
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (codec == NULL || cfg == NULL) return 0;

    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    /* Use fuzzing data to configure width/height if possible */
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        /* Sanity checks */
        if (w > 0 && w <= 4096 && h > 0 && h <= 4096) {
            cfg->g_w = w;
            cfg->g_h = h;
        }
    }

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(codec);
        return 0;
    }

    return 1;
}

static int setup_test_frame(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_image_t img;
    aom_codec_err_t ret;
    int width = 64;
    int height = 64;

    if (codec == NULL) return 0;

    /* Initialize image with fuzz data if size is sufficient */
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    if (img.planes[0] == NULL) {
        aom_img_free(&img);
        return 0;
    }

    /* Calculate total bytes for the image */
    size_t img_size = (size_t)img.w * img.h;
    size_t total_size = img_size * 3 / 2; /* Approximate for I420 */

    if (size >= total_size) {
        memcpy(img.planes[0], data, img_size);
        memcpy(img.planes[1], data + img_size, img_size / 4);
        memcpy(img.planes[2], data + img_size + img_size / 4, img_size / 4);
    } else if (size > 0) {
        /* Fill with pattern if data is too small */
        memset(img.planes[0], 0xAA, img_size);
        memset(img.planes[1], 0xAA, img_size / 4);
        memset(img.planes[2], 0xAA, img_size / 4);
    }

    /* Use 0 for flags instead of AOM_EFLAG_NONE. Fixed argument count to 5. */
    ret = aom_codec_encode(codec, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }

    aom_img_free(&img);
    return 1;
}

int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int initialized = 0;
    int frame_sent = 0;
    aom_codec_err_t ret;

    if (Size == 0) return 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    /* Initialize encoder */
    if (!init_encoder(&codec, &cfg, Data, Size)) {
        return 0;
    }
    initialized = 1;

    /* Send a frame to ensure encoder is in a valid state for control calls */
    if (setup_test_frame(&codec, Data, Size)) {
        frame_sent = 1;
    }

    /*
     * Invoke target control functions with diverse values derived from fuzz input.
     * The typechecked functions expect: (ctx, ctrl_id, data)
     */

    /* 1. AV1E_SET_ENABLE_INTRABC (expect 0 or 1) */
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(&codec, AV1E_SET_ENABLE_INTRABC, val1);
    (void)ret;

    /* 2. AV1E_SET_QM_U (expect int, range 0-255 typically) */
    unsigned int val2 = (Size > 1) ? (Data[1] % 256) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_QM_U(&codec, AV1E_SET_QM_U, val2);
    (void)ret;

    /* 3. AV1E_SET_ENABLE_KEYFRAME_FILTERING (expect 0 or 1) */
    unsigned int val3 = (Size > 2) ? (Data[2] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING(&codec, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val3);
    (void)ret;

    /* 4. AV1E_SET_CHROMA_SUBSAMPLING_Y (expect int, typically 0-1 or similar) */
    unsigned int val4 = (Size > 3) ? (Data[3] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_Y(&codec, AV1E_SET_CHROMA_SUBSAMPLING_Y, val4);
    (void)ret;

    /* 5. AV1E_SET_ENABLE_CDEF (expect 0 or 1) */
    int val5 = (Size > 4) ? (Data[4] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF(&codec, AV1E_SET_ENABLE_CDEF, val5);
    (void)ret;

    /* 6. AV1E_SET_CHROMA_SUBSAMPLING_X (expect int, typically 0-1 or similar) */
    unsigned int val6 = (Size > 5) ? (Data[5] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_CHROMA_SUBSAMPLING_X(&codec, AV1E_SET_CHROMA_SUBSAMPLING_X, val6);
    (void)ret;

    /* Cleanup */
    if (initialized) {
        aom_codec_destroy(&codec);
    }

    return 0;
}