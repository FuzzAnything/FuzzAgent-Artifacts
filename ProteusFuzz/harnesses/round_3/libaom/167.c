// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static int fuzz_encoder_controls(const uint8_t *data, size_t size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_image_t *frame;
    aom_codec_cx_pkt_t *pkt;
    int value;
    int result = 0;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder settings safely
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.g_threads = (data[4] % 4) + 1;
    if (size > 5) cfg.rc_target_bitrate = (data[5] % 10000) + 1000;
    if (size > 6) cfg.g_error_resilient = (data[6] % 3);
    if (size > 7) cfg.g_lag_in_frames = (data[7] % 20);

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Create a dummy frame
    frame = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!frame) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fill frame with fuzzing data
    memset(frame->planes[0], 0, frame->d_w * frame->d_h);
    memset(frame->planes[1], 128, (frame->d_w * frame->d_h) / 4);
    memset(frame->planes[2], 128, (frame->d_w * frame->d_h) / 4);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    if (size > 8) {
        value = data[8] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
        if (ret != AOM_CODEC_OK) result = 1;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (size > 9) {
        value = data[9] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
        if (ret != AOM_CODEC_OK) result = 1;
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (size > 10) {
        value = data[10] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, value);
        if (ret != AOM_CODEC_OK) result = 1;
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (size > 11) {
        value = data[11] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, value);
        if (ret != AOM_CODEC_OK) result = 1;
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    if (size > 12) {
        value = data[12] % 2;
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_PAETH_INTRA, value);
        if (ret != AOM_CODEC_OK) result = 1;
    }

    // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH
    if (size > 13) {
        int *cdef_strength = malloc(sizeof(int));
        if (cdef_strength) {
            ret = aom_codec_control(&codec, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strength);
            if (ret == AOM_CODEC_OK) {
                // Use the value to ensure it's read
                (void)*cdef_strength;
            }
            free(cdef_strength);
        }
    }

    // Encode a frame to exercise the codec further
    ret = aom_codec_encode(&codec, frame, 0, 1, 0);
    if (ret == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(&codec, NULL))) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process encoded frame
            }
            aom_codec_destroy(&codec); 
        }
    }

    // Cleanup
    aom_img_free(frame);
    aom_codec_destroy(&codec);

    return result;
}

int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    return fuzz_encoder_controls(Data, Size);
}