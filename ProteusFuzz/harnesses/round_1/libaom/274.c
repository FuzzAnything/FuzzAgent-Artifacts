// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aom_external_partition.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

static aom_image_t* create_test_image(const uint8_t *Data, size_t Size) {
    if (Size < 16) return NULL;

    aom_image_t *img = malloc(sizeof(aom_image_t));
    if (!img) return NULL;

    memset(img, 0, sizeof(aom_image_t));

    // Parse dimensions from input data safely
    unsigned int w = 0, h = 0;
    if (Size >= 8) {
        w = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        h = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    }
    
    // Clamp dimensions to reasonable bounds to prevent allocation failures
    if (w == 0 || w > 4096) w = 16;
    if (h == 0 || h > 4096) h = 16;

    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->bit_depth = 8;
    img->fmt = AOM_IMG_FMT_I420;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->monochrome = 0;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_UNKNOWN;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->bps = 8;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;

    // Allocate planes
    size_t plane_size = (size_t)w * h;
    size_t plane_size_uv = (size_t)(w >> 1) * (h >> 1);

    img->planes[0] = malloc(plane_size);
    img->stride[0] = w;
    
    img->planes[1] = malloc(plane_size_uv);
    img->stride[1] = w >> 1;

    img->planes[2] = malloc(plane_size_uv);
    img->stride[2] = w >> 1;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Fill with data
    size_t data_len = Size > 16 ? Size - 16 : 0;
    if (data_len > 0) {
        memcpy(img->planes[0], Data + 16, data_len > plane_size ? plane_size : data_len);
        if (data_len > plane_size) {
            size_t uv_len = data_len - plane_size;
            memcpy(img->planes[1], Data + 16 + plane_size, uv_len > plane_size_uv ? plane_size_uv : uv_len);
            if (uv_len > plane_size_uv) {
                size_t v_len = uv_len - plane_size_uv;
                memcpy(img->planes[2], Data + 16 + plane_size + plane_size_uv, v_len > plane_size_uv ? plane_size_uv : v_len);
            }
        }
    }

    return img;
}

int LLVMFuzzerTestOneInput_274(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_image_t *img = create_test_image(Data, Size);
    if (!img) return 0;

    // Fuzz aom_img_plane_width
    // Try valid planes (0, 1, 2) and some invalid ones derived from input
    int plane_idx = Data[15] % 5; 
    aom_img_plane_width(img, plane_idx);
    aom_img_plane_width(img, 0);
    aom_img_plane_width(img, 1);
    aom_img_plane_width(img, 2);
    aom_img_plane_width(img, -1);
    aom_img_plane_width(img, 10);

    // Fuzz aom_img_plane_height
    aom_img_plane_height(img, plane_idx);
    aom_img_plane_height(img, 0);
    aom_img_plane_height(img, 1);
    aom_img_plane_height(img, 2);
    aom_img_plane_height(img, -1);
    aom_img_plane_height(img, 10);

    // Fuzz aom_img_set_rect
    // Extract parameters from input
    unsigned int x = (Size > 16) ? Data[16] : 0;
    unsigned int y = (Size > 17) ? Data[17] : 0;
    unsigned int w = (Size > 18) ? Data[18] : 0;
    unsigned int h = (Size > 19) ? Data[19] : 0;
    unsigned int border = (Size > 20) ? Data[20] : 0;

    // Modulate to potentially valid ranges
    if (img->w > 0 && x > img->w) x = 0;
    if (img->h > 0 && y > img->h) y = 0;
    if (w > img->w) w = img->w;
    if (h > img->h) h = img->h;
    if (border > 10) border = 10;

    aom_img_set_rect(img, x, y, w, h, border);
    
    // Try invalid coordinates
    aom_img_set_rect(img, img->w + 10, img->h + 10, 10, 10, 0);
    aom_img_set_rect(img, 0, 0, 0, 0, 0);
    aom_img_set_rect(img, 0, 0, img->w + 1, img->h + 1, 0);

    // Fuzz aom_img_remove_metadata
    // Since aom_metadata_array_t is incomplete in the public header, 
    // we cannot manually construct it. We rely on the function's NULL safety.
    // We set metadata to NULL explicitly to test the NULL path.
    img->metadata = NULL;
    aom_img_remove_metadata(img);
    
    // Call again on NULL metadata
    aom_img_remove_metadata(img);

    // Fuzz aom_img_free
    // Ensure we free the image properly
    aom_img_free(img);

    // Call on NULL (should be safe)
    aom_img_free(NULL);

    return 0;
}