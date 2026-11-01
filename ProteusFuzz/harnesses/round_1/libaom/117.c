// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/* Helper function to safely parse unsigned int from fuzz data */
static uint32_t get_uint32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) > size) {
        return 0;
    }
    uint32_t val;
    memcpy(&val, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return val;
}

/* Helper function to safely parse size_t from fuzz data */
static size_t get_size(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(size_t) > size) {
        return 0;
    }
    size_t val;
    memcpy(&val, data + *offset, sizeof(size_t));
    *offset += sizeof(size_t);
    return val;
}

/* Helper function to get a valid image format */
static aom_img_fmt_t get_fmt(const uint8_t *data, size_t size, size_t *offset) {
    uint32_t val = get_uint32(data, size, offset);
    if (val == 0) return AOM_IMG_FMT_I420;
    if (val >= AOM_IMG_FMT_NONE && val <= AOM_IMG_FMT_I44416) {
        return (aom_img_fmt_t)val;
    }
    return AOM_IMG_FMT_I420;
}

/* Helper function to get a valid alignment */
static unsigned int get_align(const uint8_t *data, size_t size, size_t *offset) {
    uint32_t val = get_uint32(data, size, offset);
    if (val == 0) return 1;
    if (val > 65536) return 65536;
    if (val < 1) return 1;
    return val;
}

/* Helper function to get safe width/height */
static unsigned int get_dim(const uint8_t *data, size_t size, size_t *offset) {
    uint32_t val = get_uint32(data, size, offset);
    if (val == 0) return 16;
    if (val > 16384) return 16384;
    if (val < 1) return 1;
    return val;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    size_t offset = 0;
    uint32_t op = get_uint32(Data, Size, &offset);
    op %= 7; /* 0-6 for 6 functions + 1 extra for codec init path */

    /* Allocate context on heap to prevent stack overflow */
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;
    unsigned char *img_data = NULL;

    /* Initialize decoder context for codec-related functions */
    if (op == 0 || op == 3) {
        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        const aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) {
            free(ctx);
            return 0;
        }

        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;

        if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            free(ctx);
            ctx = NULL;
        }
    }

    switch (op) {
        case 0: {
            /* Fuzz aom_codec_get_frame */
            if (!ctx) break;

            /* Prepare some dummy data to decode if needed */
            if (offset < Size) {
                /* Try to decode with remaining data */
                aom_codec_err_t res = aom_codec_decode(ctx, Data + offset, Size - offset, 0);
                if (res == AOM_CODEC_OK) {
                    iter = NULL;
                    while ((img = aom_codec_get_frame(ctx, &iter)) != NULL) {
                        /* Process frame: check metadata */
                        size_t num_meta = aom_img_num_metadata(img);
                        for (size_t i = 0; i < num_meta; i++) {
                            const aom_metadata_t *meta = aom_img_get_metadata(img, i);
                            if (meta) {
                                /* Safe access to metadata fields */
                                (void)meta->type;
                                (void)meta->sz;
                            }
                        }
                    }
                }
            }
            break;
        }
        case 1: {
            /* Fuzz aom_img_num_metadata */
            aom_img_fmt_t fmt = get_fmt(Data, Size, &offset);
            unsigned int w = get_dim(Data, Size, &offset);
            unsigned int h = get_dim(Data, Size, &offset);
            unsigned int align = get_align(Data, Size, &offset);

            img = aom_img_alloc(NULL, fmt, w, h, align);
            if (img) {
                size_t count = aom_img_num_metadata(img);
                (void)count;
                aom_img_free(img);
            }
            /* Test with NULL */
            (void)aom_img_num_metadata(NULL);
            break;
        }
        case 2: {
            /* Fuzz aom_img_alloc */
            aom_img_fmt_t fmt = get_fmt(Data, Size, &offset);
            unsigned int w = get_dim(Data, Size, &offset);
            unsigned int h = get_dim(Data, Size, &offset);
            unsigned int align = get_align(Data, Size, &offset);

            /* Test allocation with NULL img pointer */
            img = aom_img_alloc(NULL, fmt, w, h, align);
            if (img) {
                aom_img_free(img);
            }

            /* Test allocation with pre-allocated struct */
            aom_image_t local_img;
            memset(&local_img, 0, sizeof(local_img));
            img = aom_img_alloc(&local_img, fmt, w, h, align);
            if (img) {
                aom_img_free(img);
            }
            break;
        }
        case 3: {
            /* Fuzz aom_codec_get_preview_frame */
            if (!ctx) break;
            
            /* This function is typically for encoders, but we can call it on a decoder context
               to test the NULL return path or internal error handling safely. */
            const aom_image_t *preview = aom_codec_get_preview_frame(ctx);
            if (preview) {
                /* Should not happen for decoder, but handle if it does */
                size_t num_meta = aom_img_num_metadata(preview);
                (void)num_meta;
            }
            break;
        }
        case 4: {
            /* Fuzz aom_img_get_metadata */
            aom_img_fmt_t fmt = get_fmt(Data, Size, &offset);
            unsigned int w = get_dim(Data, Size, &offset);
            unsigned int h = get_dim(Data, Size, &offset);
            unsigned int align = get_align(Data, Size, &offset);

            img = aom_img_alloc(NULL, fmt, w, h, align);
            if (img) {
                size_t idx = get_size(Data, Size, &offset);
                const aom_metadata_t *meta = aom_img_get_metadata(img, idx);
                if (meta) {
                    (void)meta->type;
                }
                aom_img_free(img);
            }
            /* Test with NULL image */
            (void)aom_img_get_metadata(NULL, 0);
            break;
        }
        case 5: {
            /* Fuzz aom_img_wrap */
            aom_img_fmt_t fmt = get_fmt(Data, Size, &offset);
            unsigned int w = get_dim(Data, Size, &offset);
            unsigned int h = get_dim(Data, Size, &offset);
            unsigned int stride_align = get_align(Data, Size, &offset);

            /* Calculate required size roughly to avoid allocation failure */
            uint64_t approx_size = (uint64_t)w * h * 3 + 1024;
            if (approx_size > 1024 * 1024 * 10) approx_size = 1024 * 1024 * 10;
            
            img_data = malloc(approx_size);
            if (!img_data) break;
            
            memset(img_data, 0, approx_size);
            
            /* Wrap with NULL img pointer */
            img = aom_img_wrap(NULL, fmt, w, h, stride_align, img_data);
            if (img) {
                size_t count = aom_img_num_metadata(img);
                (void)count;
                aom_img_free(img);
            }

            /* Wrap with pre-allocated struct */
            aom_image_t local_img;
            memset(&local_img, 0, sizeof(local_img));
            img = aom_img_wrap(&local_img, fmt, w, h, stride_align, img_data);
            if (img) {
                aom_img_free(img);
            }
            
            free(img_data);
            break;
        }
        case 6: {
            /* Combined path: Alloc, Wrap, Metadata, Codec Frame iteration */
            aom_img_fmt_t fmt = get_fmt(Data, Size, &offset);
            unsigned int w = get_dim(Data, Size, &offset);
            unsigned int h = get_dim(Data, Size, &offset);
            unsigned int align = get_align(Data, Size, &offset);

            /* Alloc path */
            img = aom_img_alloc(NULL, fmt, w, h, align);
            if (img) {
                size_t idx = get_size(Data, Size, &offset);
                const aom_metadata_t *meta = aom_img_get_metadata(img, idx);
                if (meta) {
                    (void)meta->sz;
                }
                aom_img_free(img);
            }

            /* Wrap path */
            uint64_t approx_size = (uint64_t)w * h * 3 + 1024;
            if (approx_size > 1024 * 1024 * 10) approx_size = 1024 * 1024 * 10;
            img_data = malloc(approx_size);
            if (img_data) {
                memset(img_data, 0, approx_size);
                img = aom_img_wrap(NULL, fmt, w, h, align, img_data);
                if (img) {
                    size_t count = aom_img_num_metadata(img);
                    for (size_t i = 0; i < count; i++) {
                        const aom_metadata_t *m = aom_img_get_metadata(img, i);
                        if (m) (void)m->type;
                    }
                    aom_img_free(img);
                }
                free(img_data);
            }
            break;
        }
    }

    /* Cleanup */
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}