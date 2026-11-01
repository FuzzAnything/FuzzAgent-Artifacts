// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

// Helper to read 16-bit unsigned integer
static int read_uint16(const uint8_t **data, size_t *size) {
    if (*size < 2) return 0;
    uint16_t val = (uint16_t)data[0] | ((uint16_t)data[1] << 8);
    *data += 2;
    *size -= 2;
    return val;
}

// Helper to read 32-bit unsigned integer
static int read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    uint32_t val = (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
                   ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

// Helper to read 32-bit signed integer
static int read_int32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    int32_t val = (int32_t)data[0] | ((int32_t)data[1] << 8) |
                  ((int32_t)data[2] << 16) | ((int32_t)data[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

// Helper to read boolean (1 byte)
static int read_bool(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    int val = (int)data[0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t err;
    const uint8_t *ptr = Data;
    size_t rem = Size;

    // Initialize encoder context
    // Use aom_codec_av1_cx() which returns the interface pointer for both enc/dec
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Parse configuration from input
    if (rem >= 4) {
        cfg.g_w = read_uint32(&ptr, &rem);
        cfg.g_h = read_uint32(&ptr, &rem);
        cfg.g_timebase.num = read_uint32(&ptr, &rem);
        cfg.g_timebase.den = read_uint32(&ptr, &rem);
        cfg.g_threads = read_uint32(&ptr, &rem) % 32 + 1;
        cfg.g_error_resilient = read_bool(&ptr, &rem);
        cfg.rc_target_bitrate = read_uint32(&ptr, &rem);
        cfg.rc_min_quantizer = read_uint32(&ptr, &rem) % 64;
        cfg.rc_max_quantizer = read_uint32(&ptr, &rem) % 64;
    }

    // Ensure dimensions are valid
    if (cfg.g_w == 0 || cfg.g_h == 0 || cfg.g_w > 4096 || cfg.g_h > 4096) {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }
    if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 30;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Allocate memory for the frame using aom_img_alloc
    aom_image_t *img_ptr = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img_ptr) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    img = *img_ptr;

    // Fill with some data
    if (img.planes[0]) {
        for (int i = 0; i < img.w * img.h; i++) {
            img.planes[0][i] = (uint8_t)(ptr[i % rem]);
        }
    }
    if (img.planes[1]) {
        for (int i = 0; i < (img.w / 2) * (img.h / 2); i++) {
            img.planes[1][i] = (uint8_t)(ptr[(i + 100) % rem]);
        }
    }
    if (img.planes[2]) {
        for (int i = 0; i < (img.w / 2) * (img.h / 2); i++) {
            img.planes[2][i] = (uint8_t)(ptr[(i + 200) % rem]);
        }
    }

    // Fuzzing logic for specific control IDs
    // 1. AV1E_SET_RENDER_SIZE
    // The type is typically a struct containing width and height. 
    // Since aom_render_size_t might not be directly exposed or named differently in some versions,
    // we define a local struct if needed, or rely on the macro definition if it exists.
    // In standard libaom, AV1E_SET_RENDER_SIZE expects a pointer to a struct with w and h.
    // We will define a local struct to ensure compatibility.
    if (rem >= 8) {
        int32_t width = read_int32(&ptr, &rem);
        int32_t height = read_int32(&ptr, &rem);
        if (width > 0 && height > 0 && width < 4096 && height < 4096) {
            // Define a local struct matching the expected layout for AV1E_SET_RENDER_SIZE
            struct { int32_t w; int32_t h; } rs = { .w = width, .h = height };
            aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, &rs);
        }
    }

    // 2. AV1E_GET_SEQ_LEVEL_IDX
    if (rem >= 1) {
        int32_t level_idx = 0;
        aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
        (void)level_idx;
    }

    // 3. AV1E_SET_ENABLE_PALETTE
    if (rem >= 1) {
        int32_t enable = read_bool(&ptr, &rem);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, &enable);
    }

    // 4. AV1E_SET_ENABLE_DNL_DENOISING
    if (rem >= 1) {
        int32_t enable = read_bool(&ptr, &rem);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, &enable);
    }

    // 5. AV1E_SET_RTC_EXTERNAL_RC
    if (rem >= 1) {
        int32_t enable = read_bool(&ptr, &rem);
        aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, &enable);
    }

    // 6. AV1E_SET_ENABLE_CHROMA_DELTAQ
    if (rem >= 1) {
        int32_t enable = read_bool(&ptr, &rem);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, &enable);
    }

    // Try to encode a frame to exercise the state changes
    // aom_codec_encode signature: (ctx, img, pts, duration, flags)
    if (img.planes[0]) {
        aom_codec_encode(&ctx, &img, 0, 1, 0);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}