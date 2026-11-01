// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

/* Helper function to initialize a basic image for testing */
static aom_image_t* create_test_image(const uint8_t *data, size_t size) {
    aom_image_t *img = malloc(sizeof(aom_image_t));
    if (!img) return NULL;

    memset(img, 0, sizeof(aom_image_t));

    /* Use fuzzing data to determine image properties, ensuring safety */
    if (size > 0) {
        img->w = (data[0] % 64) + 16;
        img->h = (size > 1 ? (data[1] % 64) + 16 : 16);
    } else {
        img->w = 16;
        img->h = 16;
    }

    img->fmt = AOM_IMG_FMT_I420;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_COLOCATED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->d_w = img->w;
    img->d_h = img->h;
    img->r_w = img->w;
    img->r_h = img->h;
    img->temporal_id = 0;
    img->spatial_id = 0;

    /* Allocate planes */
    size_t stride = img->w;
    size_t plane_size = stride * img->h;
    size_t uv_size = (stride / 2) * (img->h / 2);

    img->planes[0] = malloc(plane_size);
    img->planes[1] = malloc(uv_size);
    img->planes[2] = malloc(uv_size);
    img->stride[0] = stride;
    img->stride[1] = stride / 2;
    img->stride[2] = stride / 2;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    /* Fill with dummy data */
    memset(img->planes[0], 128, plane_size);
    memset(img->planes[1], 128, uv_size);
    memset(img->planes[2], 128, uv_size);

    img->metadata = NULL;
    img->user_priv = NULL;
    img->fb_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->sz = 0;
    img->bps = 0;

    return img;
}

/* Helper function to cleanup image */
static void destroy_test_image(aom_image_t *img) {
    if (!img) return;

    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);

    /* Remove metadata if present */
    aom_img_remove_metadata(img);

    free(img);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Create a valid image structure */
    aom_image_t *img = create_test_image(Data, Size);
    if (!img) return 0;

    /* Test 1: aom_img_num_metadata on empty image */
    size_t count = aom_img_num_metadata(img);
    (void)count; /* Suppress unused warning, expected 0 */

    /* Test 2: aom_img_get_metadata on empty image (should return NULL) */
    const aom_metadata_t *meta = aom_img_get_metadata(img, 0);
    if (meta != NULL) {
        /* Should not happen */
    }

    /* Prepare metadata data from fuzz input */
    size_t meta_data_size = (Size > 10) ? (Size - 10) : 0;
    const uint8_t *meta_data = (Size > 10) ? &Data[10] : Data;
    uint32_t meta_type = (Size > 1) ? Data[1] : 0;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;

    if (Size > 2) {
        uint8_t flag_byte = Data[2];
        if (flag_byte <= 18) {
            insert_flag = (aom_metadata_insert_flags_t)flag_byte;
        }
    }

    /* Test 3: aom_img_add_metadata */
    if (meta_data_size > 0) {
        int ret = aom_img_add_metadata(img, meta_type, meta_data, meta_data_size, insert_flag);
        if (ret == 0) {
            /* Verify addition */
            size_t new_count = aom_img_num_metadata(img);
            if (new_count > 0) {
                /* Test 4: aom_img_get_metadata with valid index */
                const aom_metadata_t *retrieved_meta = aom_img_get_metadata(img, 0);
                if (retrieved_meta) {
                    /* Check fields */
                    (void)retrieved_meta->type;
                    (void)retrieved_meta->sz;
                    (void)retrieved_meta->insert_flag;
                    if (retrieved_meta->payload && retrieved_meta->sz > 0) {
                        /* Safe access to payload */
                        volatile uint8_t x = retrieved_meta->payload[0];
                        (void)x;
                    }
                }

                /* Test 5: aom_img_get_metadata with invalid index */
                const aom_metadata_t *invalid_meta = aom_img_get_metadata(img, new_count + 100);
                if (invalid_meta != NULL) {
                    /* Should not happen */
                }

                /* Test 6: aom_img_metadata_alloc and manual free */
                aom_metadata_t *manual_meta = aom_img_metadata_alloc(
                    meta_type, meta_data, meta_data_size, insert_flag
                );
                if (manual_meta) {
                    /* Verify allocation */
                    if (manual_meta->payload) {
                        volatile uint8_t y = manual_meta->payload[0];
                        (void)y;
                    }
                    /* Test 7: aom_img_metadata_free */
                    aom_img_metadata_free(manual_meta);
                }

                /* Test 8: aom_img_remove_metadata */
                aom_img_remove_metadata(img);
                size_t final_count = aom_img_num_metadata(img);
                if (final_count != 0) {
                    /* Should be 0 */
                }
            }
        }
    }

    /* Test 9: aom_img_add_metadata with NULL data (should fail) */
    int ret_null = aom_img_add_metadata(img, meta_type, NULL, 0, insert_flag);
    if (ret_null == 0) {
        /* Should fail */
    }

    /* Test 10: aom_img_add_metadata with zero size (should fail) */
    int ret_zero = aom_img_add_metadata(img, meta_type, Data, 0, insert_flag);
    if (ret_zero == 0) {
        /* Should fail */
    }

    /* Cleanup */
    destroy_test_image(img);

    return 0;
}