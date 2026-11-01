// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>

// Helper to safely read integers from fuzz input
static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(uint32_t)) return 0;
    uint32_t val;
    memcpy(&val, *data, sizeof(uint32_t));
    *data += sizeof(uint32_t);
    *size -= sizeof(uint32_t);
    return val;
}

static int64_t read_int64(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int64_t)) return 0;
    int64_t val;
    memcpy(&val, *data, sizeof(int64_t));
    *data += sizeof(int64_t);
    *size -= sizeof(int64_t);
    return val;
}

static unsigned long read_ulong(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned long)) return 0;
    unsigned long val;
    memcpy(&val, *data, sizeof(unsigned long));
    *data += sizeof(unsigned long);
    *size -= sizeof(unsigned long);
    return val;
}

int LLVMFuzzerTestOneInput_294(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // 1. Prepare environment
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    // Initialize config
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Fuzz config parameters
    cfg.g_w = read_uint32(&ptr, &remaining) % 4096;
    cfg.g_h = read_uint32(&ptr, &remaining) % 4096;
    cfg.g_timebase.num = read_uint32(&ptr, &remaining) % 1000 + 1;
    cfg.g_timebase.den = read_uint32(&ptr, &remaining) % 1000 + 1;
    cfg.g_threads = read_uint32(&ptr, &remaining) % 16 + 1;
    cfg.g_lag_in_frames = read_uint32(&ptr, &remaining) % 10;
    cfg.rc_target_bitrate = read_uint32(&ptr, &remaining) % 10000000;
    cfg.rc_min_quantizer = read_uint32(&ptr, &remaining) % 63;
    cfg.rc_max_quantizer = read_uint32(&ptr, &remaining) % 63;
    cfg.g_pass = AOM_RC_ONE_PASS;

    // Initialize encoder context
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Create a dummy image
    aom_image_t img;
    aom_img_fmt_t fmt = (aom_img_fmt_t)(read_int(&ptr, &remaining) % 8);
    if (fmt < 0) fmt = AOM_IMG_FMT_I420;
    
    int w = cfg.g_w;
    int h = cfg.g_h;
    if (w == 0 || h == 0) { w = 16; h = 16; }

    // aom_img_alloc returns void, check for NULL planes
    aom_img_alloc(&img, fmt, w, h, 1);
    if (!img.planes[0]) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    if (img.planes[0]) {
        size_t plane_size = img.stride[0] * img.h;
        size_t copy_size = (remaining < plane_size) ? remaining : plane_size;
        if (copy_size > 0) {
            memcpy(img.planes[0], ptr, copy_size);
        }
    }

    // 2. Invoke target functions diversely
    
    // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_NOISE_LEVEL
    // Assuming it takes an int value
    int denoise_val = read_int(&ptr, &remaining);
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, denoise_val);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE
    // Assuming it takes an int value
    int sframe_val = read_int(&ptr, &remaining);
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, sframe_val);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    // Assuming it takes an int value
    int intra_edge_val = read_int(&ptr, &remaining);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, intra_edge_val);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_DNL_DENOISING
    // Assuming it takes an int value
    int dnl_val = read_int(&ptr, &remaining);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, dnl_val);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
    // Assuming it takes an int value
    int rtc_val = read_int(&ptr, &remaining);
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_val);

    // Fuzz aom_codec_encode
    aom_codec_pts_t pts = read_int64(&ptr, &remaining);
    unsigned long duration = read_ulong(&ptr, &remaining);
    aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)read_int(&ptr, &remaining);

    // Ensure duration is within valid range as per docs (<= UINT32_MAX)
    if (duration > UINT32_MAX) duration = UINT32_MAX;
    
    // Ensure strictly increasing PTS logic is tested by varying input
    // But for fuzzing, we just pass the value. The library handles validation.
    
    // Encode a frame
    if (remaining > 0 || (remaining == 0 && Size > 0)) {
        // If we have data, encode the image
        aom_codec_encode(&ctx, &img, pts, duration, flags);
    } else {
        // Flush encoder
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
    }

    // 3. Cleanup
    // Check for errors and clean up
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume output packets to prevent memory leaks
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Do nothing, just consume
        }
    }

    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}