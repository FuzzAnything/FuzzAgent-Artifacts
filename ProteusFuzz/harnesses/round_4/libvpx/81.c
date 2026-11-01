// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8_SET_REFERENCE__value at vp8.h:120:1 in vp8.h
// VP8_COPY_REFERENCE__value at vp8.h:122:1 in vp8.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9_GET_REFERENCE__value at vp8.h:126:1 in vp8.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static void setup_dummy_image(vpx_image_t *img) {
    if (!img) return;
    memset(img, 0, sizeof(vpx_image_t));
    img->fmt = VPX_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    
    // Allocate dummy planes to avoid null pointer dereferences
    size_t plane_size = 16 * 16;
    img->planes[VPX_PLANE_Y] = malloc(plane_size);
    img->planes[VPX_PLANE_U] = malloc(plane_size / 4);
    img->planes[VPX_PLANE_V] = malloc(plane_size / 4);
    img->stride[VPX_PLANE_Y] = 16;
    img->stride[VPX_PLANE_U] = 8;
    img->stride[VPX_PLANE_V] = 8;
    
    if (img->planes[VPX_PLANE_Y]) memset(img->planes[VPX_PLANE_Y], 0, plane_size);
    if (img->planes[VPX_PLANE_U]) memset(img->planes[VPX_PLANE_U], 0, plane_size / 4);
    if (img->planes[VPX_PLANE_V]) memset(img->planes[VPX_PLANE_V], 0, plane_size / 4);
}

static void cleanup_image(vpx_image_t *img) {
    if (!img) return;
    if (img->planes[VPX_PLANE_Y]) free(img->planes[VPX_PLANE_Y]);
    if (img->planes[VPX_PLANE_U]) free(img->planes[VPX_PLANE_U]);
    if (img->planes[VPX_PLANE_V]) free(img->planes[VPX_PLANE_V]);
    img->planes[VPX_PLANE_Y] = NULL;
    img->planes[VPX_PLANE_U] = NULL;
    img->planes[VPX_PLANE_V] = NULL;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize a dummy codec context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    // 1. Test VP9E_SET_SVC
    int svc_val = 0;
    if (Size > 0) svc_val = Data[0];
    vpx_codec_control(&codec, VP9E_SET_SVC, svc_val);

    // 2. Test VP9E_SET_SVC_LAYER_ID
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    
    // Safe extraction of integers from buffer
    int32_t val1 = 0, val2 = 0;
    if (Size >= 4) {
        memcpy(&val1, Data, sizeof(int32_t));
        layer_id.spatial_layer_id = val1;
    }
    if (Size >= 8) {
        memcpy(&val2, Data + 4, sizeof(int32_t));
        layer_id.temporal_layer_id = val2;
    }
    
    // Fill temporal_layer_id_per_spatial safely
    if (Size >= 12) {
        int32_t temp_val;
        for (int i = 0; i < 4 && (Size - 8) >= (i+1)*4; i++) {
            memcpy(&temp_val, Data + 8 + i*4, sizeof(int32_t));
            layer_id.temporal_layer_id_per_spatial[i] = temp_val;
        }
    }

    vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);

    // 3. Test VP9_GET_REFERENCE
    vp9_ref_frame_t vp9_ref;
    memset(&vp9_ref, 0, sizeof(vp9_ref));
    if (Size > 0) vp9_ref.idx = Data[0] % 10;
    setup_dummy_image(&vp9_ref.img);

    vpx_codec_control(&codec, VP9_GET_REFERENCE, &vp9_ref);
    cleanup_image(&vp9_ref.img);

    // 4. Test VP8_SET_REFERENCE
    vpx_ref_frame_t vp8_ref;
    memset(&vp8_ref, 0, sizeof(vp8_ref));
    
    if (Size > 0) {
        int type_idx = Data[0] % 3;
        if (type_idx == 0) vp8_ref.frame_type = VP8_LAST_FRAME;
        else if (type_idx == 1) vp8_ref.frame_type = VP8_GOLD_FRAME;
        else vp8_ref.frame_type = VP8_ALTR_FRAME;
    }
    
    setup_dummy_image(&vp8_ref.img);

    vpx_codec_control(&codec, VP8_SET_REFERENCE, &vp8_ref);
    cleanup_image(&vp8_ref.img);

    // 5. Test VP8_COPY_REFERENCE
    vpx_ref_frame_t vp8_copy_ref;
    memset(&vp8_copy_ref, 0, sizeof(vp8_copy_ref));
    
    if (Size > 0) {
        int type_idx = Data[0] % 3;
        if (type_idx == 0) vp8_copy_ref.frame_type = VP8_LAST_FRAME;
        else if (type_idx == 1) vp8_copy_ref.frame_type = VP8_GOLD_FRAME;
        else vp8_copy_ref.frame_type = VP8_ALTR_FRAME;
    }
    
    setup_dummy_image(&vp8_copy_ref.img);

    vpx_codec_control(&codec, VP8_COPY_REFERENCE, &vp8_copy_ref);
    cleanup_image(&vp8_copy_ref.img);

    return 0;
}