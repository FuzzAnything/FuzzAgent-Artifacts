// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"

static int fuzz_image_functions(const uint8_t *data, size_t size) {
    if (size < 20) return 0;

    uint32_t width = *(uint32_t *)data;
    uint32_t height = *(uint32_t *)(data + 4);
    uint32_t align = *(uint32_t *)(data + 8) & 0xFFFF;
    uint32_t fmt_idx = *(uint32_t *)(data + 12) % 10;
    uint32_t plane_idx = *(uint32_t *)(data + 16) % 3;

    // Clamp dimensions to reasonable limits to avoid allocation failures or crashes
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align == 0) align = 1;
    if (align > 65536) align = 65536;

    aom_img_fmt_t fmts[] = {
        AOM_IMG_FMT_I420, AOM_IMG_FMT_YV12, AOM_IMG_FMT_I422, AOM_IMG_FMT_I444,
        AOM_IMG_FMT_NV12, AOM_IMG_FMT_I42016, AOM_IMG_FMT_YV1216, AOM_IMG_FMT_I42216,
        AOM_IMG_FMT_I44416, AOM_IMG_FMT_NONE
    };
    
    aom_img_fmt_t fmt = (fmt_idx < 10) ? fmts[fmt_idx] : AOM_IMG_FMT_I420;
    if (fmt == AOM_IMG_FMT_NONE) return 0;

    // Test aom_img_alloc
    aom_image_t img_alloc = {0};
    aom_image_t *img_allocated = aom_img_alloc(&img_alloc, fmt, width, height, align);
    
    if (img_allocated) {
        // Test aom_img_plane_height on allocated image
        int h = aom_img_plane_height(img_allocated, plane_idx);
        (void)h; // Use the result to avoid unused variable warning

        // Cleanup allocated image
        aom_img_free(img_allocated);
    }

    // Test aom_img_wrap
    // Calculate required buffer size roughly
    // For simplicity, we allocate a large enough buffer based on max possible stride
    // This is a heuristic for fuzzing, not production code
    size_t max_stride = (width + align - 1) & ~(align - 1);
    size_t max_height = (height + 1) & ~1;
    size_t buf_size = max_stride * max_height * 3; // Assume worst case 3 planes
    
    unsigned char *buf = malloc(buf_size);
    if (!buf) return 0;
    
    memset(buf, 0, buf_size);
    
    aom_image_t img_wrap = {0};
    aom_image_t *img_wrapped = aom_img_wrap(&img_wrap, fmt, width, height, align, buf);
    
    if (img_wrapped) {
        // Test aom_img_plane_height on wrapped image
        int h_wrap = aom_img_plane_height(img_wrapped, plane_idx);
        (void)h_wrap;
        
        // Cleanup wrapped image (does not free buf)
        aom_img_free(img_wrapped);
    }
    
    free(buf);
    return 0;
}

static int fuzz_codec_frame_iteration(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    // Initialize decoder context
    const aom_codec_iface_t *decoder_if = aom_codec_av1_dx();
    if (!decoder_if) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: use allow_lowbitdepth instead of allow_highbitdepth
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(&ctx, decoder_if, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Decode some data (even if invalid, it should be handled gracefully)
    // We pass the fuzzing data as a frame
    aom_codec_err_t res = aom_codec_decode(&ctx, data, size, NULL);
    if (res != AOM_CODEC_OK) {
        // Even if decode fails, we might still have frames or just exit
        // Try to get frames anyway to test the iterator logic
    }

    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    
    // Iterate frames
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process image if needed
        (void)img;
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz image related functions
    fuzz_image_functions(Data, Size);

    // Fuzz codec frame iteration
    fuzz_codec_frame_iteration(Data, Size);

    return 0;
}