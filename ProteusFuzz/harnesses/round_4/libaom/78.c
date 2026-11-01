// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int try_control(aom_codec_ctx_t *ctx, int control_id, int32_t val) {
    aom_codec_err_t err = aom_codec_control(ctx, control_id, val);
    if (err != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract parameters from fuzz input
    int32_t width = Data[0] | (Data[1] << 8);
    int32_t height = Data[2] | (Data[3] << 8);
    int32_t fps_num = Data[4] | (Data[5] << 8);
    int32_t fps_den = Data[6] | (Data[7] << 8);
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (fps_den == 0) fps_den = 1;
    if (fps_num == 0) fps_num = 30;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill frame with fuzz data using correct 'planes' member
    // d_height is not a member, use frame.h for Y plane and h/2 for UV
    int y_h = frame.h;
    int uv_h = (frame.h + 1) / 2;
    
    // Plane 0 (Y)
    if (frame.stride[0] > 0 && y_h > 0) {
        size_t bytes = (size_t)frame.stride[0] * (size_t)y_h;
        if (bytes > Size - 8) bytes = Size - 8;
        if (bytes > 0) {
            memcpy(frame.planes[0], Data + 8, bytes);
        }
    }
    
    // Plane 1 (U)
    if (frame.stride[1] > 0 && uv_h > 0) {
        size_t bytes = (size_t)frame.stride[1] * (size_t)uv_h;
        if (bytes > Size - 8) bytes = Size - 8;
        if (bytes > 0) {
            memcpy(frame.planes[1], Data + 8, bytes);
        }
    }

    // Plane 2 (V)
    if (frame.stride[2] > 0 && uv_h > 0) {
        size_t bytes = (size_t)frame.stride[2] * (size_t)uv_h;
        if (bytes > Size - 8) bytes = Size - 8;
        if (bytes > 0) {
            memcpy(frame.planes[2], Data + 8, bytes);
        }
    }

    // Fuzz various control commands using available input data
    int32_t val1 = Data[8] | (Data[9] << 8);
    int32_t val2 = Data[10] | (Data[11] << 8);
    int32_t val3 = Data[12] | (Data[13] << 8);
    int32_t val4 = Data[14] | (Data[15] << 8);
    int32_t val5 = Size > 16 ? Data[16] : 0;

    // AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST (if defined)
    #ifdef AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
    try_control(&ctx, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, val1);
    #endif

    // AV1E_SET_AQ_MODE
    #ifdef AV1E_SET_AQ_MODE
    try_control(&ctx, AV1E_SET_AQ_MODE, val2);
    #endif

    // AV1E_SET_MODE_COST_UPD_FREQ
    #ifdef AV1E_SET_MODE_COST_UPD_FREQ
    try_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val3);
    #endif

    // AOME_SET_MAX_INTRA_BITRATE_PCT
    #ifdef AOME_SET_MAX_INTRA_BITRATE_PCT
    try_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val4);
    #endif

    // AV1E_SET_DELTAQ_STRENGTH
    #ifdef AV1E_SET_DELTAQ_STRENGTH
    try_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val5);
    #endif

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    #ifdef AV1E_SET_ENABLE_GLOBAL_MOTION
    try_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val1 % 2);
    #endif

    // Encode a frame to exercise the codec
    aom_codec_control(&ctx, AOME_SET_CPUUSED, 0);
    aom_enc_frame_flags_t flags = 0;
    if (Size > 17) flags = Data[17];
    
    // Fixed: aom_codec_encode takes 5 arguments
    err = aom_codec_encode(&ctx, &frame, 0, 1, flags);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        aom_img_free(&frame);
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Consume the packet
            // Do nothing, just consume
        }
    }

    aom_codec_destroy(&ctx);
    aom_img_free(&frame);
    return 0;
}