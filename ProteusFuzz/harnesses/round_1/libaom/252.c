// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void safe_decode_cleanup(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_iter_t iter = NULL;
        while (aom_codec_get_frame(ctx, &iter) != NULL) {
            // Consume frames to free internal buffers
        }
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_252(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Parse basic parameters from input
    unsigned int width = Data[0];
    unsigned int height = Data[1];
    
    // Enforce reasonable bounds to prevent integer overflow and excessive memory allocation
    if (width == 0 || height == 0 || width > 16384 || height > 16384) {
        width = 16;
        height = 16;
    }

    // Limit alignment
    unsigned int align = 16;
    if (Size > 2) {
        align = Data[2] & 0x3F; // Limit to 64
        if (align == 0) align = 16;
    }

    // Select a format based on input
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (Size > 3) {
        int fmt_idx = Data[3] % 10; // Use first 10 formats
        switch (fmt_idx) {
            case 0: fmt = AOM_IMG_FMT_NONE; break;
            case 1: fmt = AOM_IMG_FMT_YV12; break;
            case 2: fmt = AOM_IMG_FMT_I420; break;
            case 3: fmt = AOM_IMG_FMT_AOMYV12; break;
            case 4: fmt = AOM_IMG_FMT_AOMI420; break;
            case 5: fmt = AOM_IMG_FMT_I422; break;
            case 6: fmt = AOM_IMG_FMT_I444; break;
            case 7: fmt = AOM_IMG_FMT_NV12; break;
            case 8: fmt = AOM_IMG_FMT_I42016; break;
            case 9: fmt = AOM_IMG_FMT_YV1216; break;
            default: fmt = AOM_IMG_FMT_I420;
        }
    }

    // Test aom_img_alloc
    aom_image_t img_alloc;
    aom_image_t *img_ptr = aom_img_alloc(&img_alloc, fmt, width, height, align);
    if (img_ptr != NULL) {
        // Test aom_img_plane_height
        for (int p = 0; p < 3; p++) {
            (void)aom_img_plane_height(img_ptr, p);
        }
        aom_img_free(img_ptr);
    }

    // Test aom_img_wrap
    // Calculate approximate size needed for wrapping (simplified)
    // We allocate a buffer large enough for the worst case (I420)
    size_t buf_size = (size_t)width * height * 3 / 2 + 1024;
    unsigned char *img_data = (unsigned char *)malloc(buf_size);
    if (img_data != NULL) {
        aom_image_t img_wrap;
        aom_image_t *wrapped = aom_img_wrap(&img_wrap, fmt, width, height, align, img_data);
        if (wrapped != NULL) {
            for (int p = 0; p < 3; p++) {
                (void)aom_img_plane_height(wrapped, p);
            }
            // Note: We do not call aom_img_free here because img_data is malloc'd
            // The descriptor is stack allocated, so no leak on descriptor side.
        }
        free(img_data);
    }

    // Test Decoder Context and aom_codec_get_frame
    aom_codec_iface_t *decoder = aom_codec_av1_dx();
    if (decoder == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = width;
    cfg.h = height;

    if (aom_codec_dec_init(&ctx, decoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // If we have enough data, try to decode
    if (Size > 10) {
        aom_codec_decode(&ctx, Data, Size, NULL);

        // Iterate over frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *frame;
        while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            // Frame is valid here
            // Test aom_img_plane_height on the decoded frame
            for (int p = 0; p < 3; p++) {
                (void)aom_img_plane_height(frame, p);
            }
            // Do not free the frame, it is managed by the codec
        }
    }

    // Cleanup
    safe_decode_cleanup(&ctx);

    return 0;
}