// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"

static void fuzz_decode_and_encode(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    aom_codec_err_t ret;

    // Initialize decoder
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Changed allow_highbitdepth to allow_lowbitdepth as per struct definition
    cfg.allow_lowbitdepth = 1;

    iface = aom_codec_av1_dx();
    if (!iface) return;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return;

    // Decode input data
    if (size > 0) {
        ret = aom_codec_decode(&ctx, data, size, NULL);
        if (ret != AOM_CODEC_OK) {
            // Try to get frames even if decode failed partially
        }
    }

    // Iterate over decoded frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Test aom_img_num_metadata
        size_t num_meta = aom_img_num_metadata(img);
        (void)num_meta;

        // Test aom_img_get_metadata with various indices
        for (size_t i = 0; i <= num_meta + 1; i++) {
            const aom_metadata_t *meta = aom_img_get_metadata(img, i);
            if (meta) {
                // Safe access
                (void)meta->type;
                (void)meta->sz;
            }
        }

        // Test aom_img_plane_height for valid and invalid planes
        int h0 = aom_img_plane_height(img, 0);
        int h1 = aom_img_plane_height(img, 1);
        int h2 = aom_img_plane_height(img, 2);
        int h3 = aom_img_plane_height(img, 3); // Invalid plane
        int hn = aom_img_plane_height(img, -1); // Invalid plane
        (void)h0; (void)h1; (void)h2; (void)h3; (void)hn;

        // Test aom_codec_get_preview_frame (might fail if not supported, but safe to call)
        const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
        if (preview) {
            size_t p_num = aom_img_num_metadata(preview);
            (void)p_num;
        }
    }

    aom_codec_destroy(&ctx);
}

static void fuzz_image_wrapping(const uint8_t *data, size_t size) {
    aom_image_t img;
    aom_img_fmt_t fmt;
    unsigned int d_w, d_h;
    unsigned int stride_align;
    unsigned char *img_data = NULL;
    size_t alloc_size;

    // Extract parameters from fuzz data if available
    if (size >= 4) {
        d_w = (data[0] << 8) | data[1];
        d_h = (data[2] << 8) | data[3];
        // Clamp dimensions to reasonable limits to avoid allocation failures
        if (d_w == 0) d_w = 1;
        if (d_h == 0) d_h = 1;
        if (d_w > 4096) d_w = 4096;
        if (d_h > 4096) d_h = 4096;
    } else {
        d_w = 16;
        d_h = 16;
    }

    stride_align = (size > 4) ? (data[4] % 256) : 16;
    if (stride_align == 0) stride_align = 16;

    // Select format
    fmt = (aom_img_fmt_t)(size % 10);
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;

    // Calculate allocation size manually or use aom_img_wrap with dummy pointer
    // To be safe, we allocate a buffer large enough for typical cases
    // Using a conservative estimate for I420: Y + U + V
    // Y: d_w * d_h
    // U: (d_w/2) * (d_h/2)
    // V: (d_w/2) * (d_h/2)
    // Plus alignment padding
    size_t base_size = (size_t)d_w * d_h + (size_t)d_w * d_h / 2;
    if (base_size == 0) base_size = 256;
    
    // Allocate buffer
    img_data = (unsigned char *)malloc(base_size * 2); // Extra safety
    if (!img_data) return;

    // Initialize image structure
    memset(&img, 0, sizeof(img));

    // Wrap the image
    aom_image_t *wrapped = aom_img_wrap(&img, fmt, d_w, d_h, stride_align, img_data);
    if (wrapped) {
        // Test aom_img_num_metadata on wrapped image (should be 0 initially)
        size_t num = aom_img_num_metadata(wrapped);
        (void)num;

        // Test aom_img_get_metadata
        const aom_metadata_t *meta = aom_img_get_metadata(wrapped, 0);
        (void)meta;

        // Test aom_img_plane_height
        int h0 = aom_img_plane_height(wrapped, 0);
        int h1 = aom_img_plane_height(wrapped, 1);
        int h2 = aom_img_plane_height(wrapped, 2);
        (void)h0; (void)h1; (void)h2;
    }

    free(img_data);
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run decoder fuzzing
    fuzz_decode_and_encode(Data, Size);

    // Run image wrapping fuzzing
    fuzz_image_wrapping(Data, Size);

    return 0;
}