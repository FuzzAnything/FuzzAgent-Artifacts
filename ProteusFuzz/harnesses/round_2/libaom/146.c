// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>

static void safe_cleanup(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static void safe_img_free(aom_image_t *img) {
    if (img != NULL) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Parse basic parameters from fuzz input
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    uint32_t stride_align = Data[4];
    uint32_t fmt_idx = Data[5];
    
    // Clamp dimensions to safe limits to avoid OOM or invalid states
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (stride_align == 0) stride_align = 16;
    if (stride_align > 65536) stride_align = 65536;

    // Map format index to valid enum
    aom_img_fmt_t fmt;
    switch (fmt_idx % 8) {
        case 0: fmt = AOM_IMG_FMT_I420; break;
        case 1: fmt = AOM_IMG_FMT_YV12; break;
        case 2: fmt = AOM_IMG_FMT_NV12; break;
        case 3: fmt = AOM_IMG_FMT_I422; break;
        case 4: fmt = AOM_IMG_FMT_I444; break;
        case 5: fmt = AOM_IMG_FMT_I42016; break;
        case 6: fmt = AOM_IMG_FMT_YV1216; break;
        default: fmt = AOM_IMG_FMT_I420; break;
    }

    // --- Test aom_img_alloc ---
    aom_image_t *img_alloc = aom_img_alloc(NULL, fmt, width, height, stride_align);
    if (img_alloc != NULL) {
        // Test aom_img_num_metadata and aom_img_get_metadata on allocated image
        size_t meta_count = aom_img_num_metadata(img_alloc);
        for (size_t i = 0; i < meta_count; i++) {
            const aom_metadata_t *meta = aom_img_get_metadata(img_alloc, i);
            if (meta != NULL) {
                // Just access to ensure no crash
                volatile uint32_t t = meta->type;
                volatile size_t s = meta->sz;
                (void)t; (void)s;
            }
        }
        safe_img_free(img_alloc);
    }

    // --- Test aom_img_wrap ---
    // Calculate required size roughly (simplified for fuzzing)
    // We use a fixed large buffer to ensure we cover the requirements for small images
    size_t max_buffer_size = (size_t)width * height * 4 * 2; 
    if (max_buffer_size < 1024) max_buffer_size = 1024;
    
    unsigned char *img_data = (unsigned char *)malloc(max_buffer_size);
    if (img_data != NULL) {
        // Try wrapping with NULL img (heap alloc for descriptor)
        aom_image_t *img_wrap_heap = aom_img_wrap(NULL, fmt, width, height, stride_align, img_data);
        if (img_wrap_heap != NULL) {
            size_t meta_count = aom_img_num_metadata(img_wrap_heap);
            for (size_t i = 0; i < meta_count; i++) {
                const aom_metadata_t *meta = aom_img_get_metadata(img_wrap_heap, i);
                if (meta != NULL) {
                    volatile uint32_t t = meta->type;
                    (void)t;
                }
            }
            aom_img_free(img_wrap_heap);
        }

        // Try wrapping with pre-allocated descriptor
        aom_image_t img_wrap_stack;
        memset(&img_wrap_stack, 0, sizeof(img_wrap_stack));
        aom_image_t *img_wrap_stack_ptr = aom_img_wrap(&img_wrap_stack, fmt, width, height, stride_align, img_data);
        if (img_wrap_stack_ptr != NULL) {
            size_t meta_count = aom_img_num_metadata(img_wrap_stack_ptr);
            for (size_t i = 0; i < meta_count; i++) {
                const aom_metadata_t *meta = aom_img_get_metadata(img_wrap_stack_ptr, i);
                if (meta != NULL) {
                    volatile uint32_t t = meta->type;
                    (void)t;
                }
            }
        }
        free(img_data);
    }

    // --- Test Codec APIs (Decoder) ---
    // We need a valid codec context to test aom_codec_get_frame and aom_codec_get_preview_frame
    // Use aom_codec_av1_dx() to get the AV1 decoder interface
    // Explicitly declare the function to avoid implicit declaration errors
    extern const aom_codec_iface_t *aom_codec_av1_dx(void);

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Decode the fuzz data
    aom_codec_err_t res = aom_codec_decode(&ctx, Data, Size, NULL);
    (void)res;

    // Test aom_codec_get_frame
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame
        size_t meta_count = aom_img_num_metadata(frame);
        for (size_t i = 0; i < meta_count; i++) {
            const aom_metadata_t *meta = aom_img_get_metadata(frame, i);
            if (meta != NULL) {
                volatile uint32_t t = meta->type;
                (void)t;
            }
        }
        // Do not free frame, it's owned by the codec until next decode or destroy
    }

    // Test aom_codec_get_preview_frame
    // This is typically for encoders, but let's call it to test error handling
    const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
    if (preview != NULL) {
        size_t meta_count = aom_img_num_metadata(preview);
        for (size_t i = 0; i < meta_count; i++) {
            const aom_metadata_t *meta = aom_img_get_metadata(preview, i);
            if (meta != NULL) {
                volatile uint32_t t = meta->type;
                (void)t;
            }
        }
    }

    safe_cleanup(&ctx);

    return 0;
}