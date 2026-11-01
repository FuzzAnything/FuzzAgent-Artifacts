// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static aom_codec_ctx_t *g_ctx = NULL;
static aom_image_t *g_img = NULL;
static aom_codec_enc_cfg_t g_cfg;
static int g_initialized = 0;

static void cleanup_resources(void) {
    if (g_ctx) {
        aom_codec_destroy(g_ctx);
        g_ctx = NULL;
    }
    if (g_img) {
        aom_img_free(g_img);
        g_img = NULL;
    }
    g_initialized = 0;
}

static int init_encoder(const uint8_t *data, size_t size) {
    if (g_initialized) {
        cleanup_resources();
    }

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &g_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure width/height if possible, else defaults
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && w <= 8192 && h > 0 && h <= 8192) {
            g_cfg.g_w = w;
            g_cfg.g_h = h;
        }
    }

    g_cfg.g_timebase.num = 1;
    g_cfg.g_timebase.den = 30;
    g_cfg.rc_target_bitrate = 1000;
    g_cfg.g_error_resilient = 1;

    g_ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!g_ctx) return 0;
    memset(g_ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(g_ctx, iface, &g_cfg, 0) != AOM_CODEC_OK) {
        free(g_ctx);
        g_ctx = NULL;
        return 0;
    }

    // Fixed: aom_img_alloc requires 5 arguments: img, fmt, d_w, d_h, align
    g_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, g_cfg.g_w, g_cfg.g_h, 1);
    if (!g_img) {
        cleanup_resources();
        return 0;
    }

    g_initialized = 1;
    return 1;
}

int LLVMFuzzerTestOneInput_291(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize or re-initialize encoder context
    if (!init_encoder(Data, Size)) {
        return 0;
    }

    // Parse input data to drive fuzzing logic
    uint8_t cmd = Data[0] % 10;
    uint32_t val = 0;
    if (Size >= 4) {
        val = *(const uint32_t *)(Data + 1);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
    if (cmd == 0) {
        aom_codec_control(g_ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING
    if (cmd == 1) {
        aom_codec_control(g_ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
    if (cmd == 2) {
        aom_codec_control(g_ctx, AV1E_SET_DELTAQ_MODE, val);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE
    if (cmd == 3) {
        aom_codec_control(g_ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val);
    }

    // Fuzz aom_codec_set_option
    if (cmd == 4) {
        const char *key = "cpu-used";
        const char *val_str = "0";
        if (Size > 10) {
            // Construct a safe string from data for value
            char buf[64];
            size_t len = (Size - 10) < 63 ? (Size - 10) : 63;
            memcpy(buf, Data + 10, len);
            buf[len] = '\0';
            val_str = buf;
        }
        aom_codec_set_option(g_ctx, key, val_str);
    }

    // Fuzz aom_codec_encode
    if (cmd >= 5) {
        // Prepare image data if needed
        if (g_img) {
            size_t img_size = g_img->w * g_img->h;
            if (Size > 10) {
                size_t copy_len = (Size - 10) < img_size ? (Size - 10) : img_size;
                memcpy(g_img->planes[AOM_PLANE_Y], Data + 10, copy_len);
            } else {
                memset(g_img->planes[AOM_PLANE_Y], 0, img_size);
            }
            // Fill chroma planes with 0 for safety
            memset(g_img->planes[AOM_PLANE_U], 0, g_img->w * g_img->h / 2);
            memset(g_img->planes[AOM_PLANE_V], 0, g_img->w * g_img->h / 2);
        }

        aom_codec_pts_t pts = (Size > 5) ? *(const int64_t *)(Data + 5) : 0;
        unsigned long duration = (Size > 13) ? *(const unsigned long *)(Data + 13) : 1;
        if (duration == 0) duration = 1; // Ensure non-zero if img is not NULL
        if (duration > UINT32_MAX) duration = UINT32_MAX;

        aom_enc_frame_flags_t flags = 0;
        if (Size > 17) flags = *(const long *)(Data + 17);

        // Call encode
        if (cmd == 5) {
            // Normal encode with image
            aom_codec_encode(g_ctx, g_img, pts, duration, flags);
        } else if (cmd == 6) {
            // Flush encode
            aom_codec_encode(g_ctx, NULL, 0, 0, 0);
        }
    }

    // Cleanup
    cleanup_resources();
    return 0;
}