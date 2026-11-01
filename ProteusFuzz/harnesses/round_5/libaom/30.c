// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    int32_t value = 0;
    if (size >= 4) {
        memcpy(&value, data, 4);
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, value);

    // AV1E_SET_ENABLE_CFL_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, value);

    // AV1E_SET_ENABLE_DUAL_FILTER
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);

    // AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value);

    // AV1E_SET_ENABLE_FILTER_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);

    // AV1E_SET_ENABLE_SMOOTH_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, value);

    return 1;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some config parameters
    cfg.g_w = (Size > 0) ? (Data[0] % 2048 + 16) : 16;
    cfg.g_h = (Size > 1) ? (Data[1] % 2048 + 16) : 16;
    cfg.g_timebase.num = (Size > 2) ? (Data[2] % 100 + 1) : 1;
    cfg.g_timebase.den = (Size > 3) ? (Data[3] % 100 + 1) : 1;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] % 1000 + 100) : 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy frame
    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzing data if available
    if (Size > 0) {
        // Calculate total size to copy safely
        size_t y_size = (size_t)img.d_w * (size_t)img.d_h;
        size_t uv_size = y_size / 4;
        size_t total_size = y_size + 2 * uv_size;
        size_t copy_size = (Size < total_size) ? Size : total_size;
        
        // Copy data to Y plane
        size_t y_copy = (copy_size < y_size) ? copy_size : y_size;
        memcpy(img.planes[0], Data, y_copy);
        
        // Copy data to U plane if enough data
        if (copy_size > y_size) {
            size_t remaining = copy_size - y_size;
            size_t uv_copy = (remaining < uv_size) ? remaining : uv_size;
            memcpy(img.planes[1], Data + y_size, uv_copy);
            
            // Copy data to V plane if enough data
            if (remaining > uv_size) {
                size_t v_copy = (remaining - uv_size < uv_size) ? remaining - uv_size : uv_size;
                memcpy(img.planes[2], Data + y_size + uv_size, v_copy);
            }
        }
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Send frame
    if (aom_codec_encode(&ctx, &img, 0, 1, 0) != AOM_CODEC_OK) {
        // Ignore error
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}