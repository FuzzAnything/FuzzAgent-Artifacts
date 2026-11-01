// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

// Helper function to initialize the decoder context
static aom_codec_ctx_t* init_decoder(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to cleanup the decoder context
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

// Callback for getting frame buffer (dummy implementation)
// Signature: int cb_get(void *cb_priv, unsigned long frame_id, aom_codec_frame_buffer_t *fb)
static int get_frame_buffer(void *cb_priv, unsigned long frame_id, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)frame_id;
    if (!fb) return -1;

    // Allocate a dummy image
    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return -1;
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 64;
    img->h = 64;
    img->d_w = 64;
    img->d_h = 64;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    
    // Allocate planes
    img->planes[0] = (unsigned char *)malloc(64 * 64);
    img->planes[1] = (unsigned char *)malloc(32 * 32);
    img->planes[2] = (unsigned char *)malloc(32 * 32);
    
    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        free(img);
        return -1;
    }
    
    img->stride[0] = 64;
    img->stride[1] = 32;
    img->stride[2] = 32;
    img->self_allocd = 1;
    
    // Set the frame buffer
    fb->data = img;
    // frame_id is not a member of aom_codec_frame_buffer_t in this version, 
    // so we do not set it here.
    
    return 0;
}

// Callback for releasing frame buffer (dummy implementation)
// Signature: int cb_release(void *cb_priv, aom_codec_frame_buffer_t *fb)
static int release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    if (!fb || !fb->data) return -1;
    
    aom_image_t *img = (aom_image_t *)fb->data;
    if (img) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        free(img);
    }
    fb->data = NULL;
    return 0;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder context
    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    // Prepare stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));

    // Test aom_codec_get_stream_info with uninitialized data (should return UNSUP_BITSTREAM or ERROR)
    aom_codec_err_t err = aom_codec_get_stream_info(ctx, &si);
    (void)err; // Ignore error for fuzzing, just exercise the path

    // Set frame buffer functions before decoding
    err = aom_codec_set_frame_buffer_functions(ctx, get_frame_buffer, release_frame_buffer, NULL);
    if (err == AOM_CODEC_OK) {
        // Try to decode some data to trigger stream info update
        // We don't actually decode here, just exercise the API path
        
        // Attempt to get stream info again after setting callbacks
        err = aom_codec_get_stream_info(ctx, &si);
        (void)err;
    }

    // Test aom_img_plane_width with a dummy image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = 100;
    img.h = 100;
    img.d_w = 100;
    img.d_h = 100;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.planes[0] = (unsigned char *)malloc(100 * 100);
    img.planes[1] = (unsigned char *)malloc(50 * 50);
    img.planes[2] = (unsigned char *)malloc(50 * 50);
    img.stride[0] = 100;
    img.stride[1] = 50;
    img.stride[2] = 50;

    int width0 = aom_img_plane_width(&img, 0);
    int width1 = aom_img_plane_width(&img, 1);
    int width2 = aom_img_plane_width(&img, 2);
    (void)width0;
    (void)width1;
    (void)width2;

    // Cleanup dummy image
    if (img.planes[0]) free(img.planes[0]);
    if (img.planes[1]) free(img.planes[1]);
    if (img.planes[2]) free(img.planes[2]);

    // Cleanup decoder context
    cleanup_decoder(ctx);

    return 0;
}