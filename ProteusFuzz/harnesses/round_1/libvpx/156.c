// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Required headers for libvpx */
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

/* 
 * Note: The target functions provided in the prompt (VP9E_GET_ACTIVEMAP__value,
 * VP8E_SET_ACTIVEMAP__value, etc.) appear to be internal or obfuscated names
 * that do not exist in the public libvpx API. The public API uses macros like
 * VP8E_SET_ACTIVEMAP which expand to vpx_codec_control calls.
 *
 * Since the prompt states the source code for these specific names is empty/missing,
 * this fuzzer targets the actual public API equivalents that perform the described
 * logic (setting active maps, temporal layers, noise sensitivity, frame flags,
 * and CPU usage) to ensure the underlying logic is tested.
 */

static vpx_codec_ctx_t *create_encoder(void) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_err_t err = vpx_codec_enc_init(ctx, iface, NULL, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Set a default resolution to ensure the encoder is valid */
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void destroy_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t *ctx = create_encoder();
    if (!ctx) return 0;

    /* 
     * We attempt to exercise the logic described in the prompt by calling
     * the corresponding public vpx_codec_control commands.
     * 
     * 1. VP8E_SET_ACTIVEMAP
     * 2. VP8E_SET_TEMPORAL_LAYER_ID
     * 3. VP8E_SET_NOISE_SENSITIVITY
     * 4. VP8E_SET_FRAME_FLAGS
     * 5. VP8E_SET_CPUUSED
     */

    /* Helper to safely extract values from fuzz input */
    uint32_t val = 0;
    if (Size >= 4) {
        memcpy(&val, Data, 4);
    } else if (Size >= 1) {
        val = Data[0];
    }

    /* 1. Test VP8E_SET_CPUUSED (Range 0-8) */
    int cpu_used = val % 9;
    vpx_codec_control(ctx, VP8E_SET_CPUUSED, cpu_used);

    /* 2. Test VP8E_SET_NOISE_SENSITIVITY */
    int noise_sensitivity = val % 100; /* Arbitrary range */
    vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    /* 3. Test VP8E_SET_TEMPORAL_LAYER_ID */
    int layer_id = val % 8; /* Arbitrary range */
    vpx_codec_control(ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);

    /* 4. Test VP8E_SET_FRAME_FLAGS */
    int frame_flags = val; 
    vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, frame_flags);

    /* 5. Test VP8E_SET_ACTIVEMAP */
    /* We need to construct a valid active map structure */
    if (Size >= 8) {
        unsigned int rows = (Data[0] % 16) + 1;
        unsigned int cols = (Data[1] % 16) + 1;
        
        /* Ensure rows/cols match the frame size (128x128 -> 8x8 blocks of 16x16) */
        /* 128/16 = 8. So max rows/cols should be 8. */
        if (rows > 8) rows = 8;
        if (cols > 8) cols = 8;

        size_t map_size = rows * cols;
        unsigned char *active_map = (unsigned char *)malloc(map_size);
        if (active_map) {
            /* Fill with fuzz data */
            size_t fill_len = (Size > map_size) ? map_size : Size;
            memcpy(active_map, Data + 2, fill_len);
            
            vpx_active_map_t map;
            map.active_map = active_map;
            map.rows = rows;
            map.cols = cols;

            vpx_codec_control(ctx, VP8E_SET_ACTIVEMAP, &map);
            
            free(active_map);
        }
    }

    /* Note: VP9E_GET_ACTIVEMAP__value was mentioned but is VP9 specific and 
     * the encoder is VP8 here. Also, it's a GET operation which usually 
     * requires a frame context. Since we are in an encoder context without 
     * a frame being encoded, we skip the GET operation to avoid crashes. */

    destroy_encoder(ctx);
    return 0;
}