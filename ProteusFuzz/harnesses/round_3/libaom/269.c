// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int create_dummy_image(aom_image_t *img, unsigned int w, unsigned int h, int bit_depth) {
    if (img == NULL) return 0;
    
    img->fmt = AOM_IMG_FMT_I420;
    img->w = w;
    img->h = h;
    img->bit_depth = bit_depth;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_COLOCATED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->monochrome = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->metadata = NULL;
    img->fb_priv = NULL;

    // Allocate planes
    size_t y_size = w * h;
    size_t uv_size = (w / 2) * (h / 2);
    
    img->planes[0] = malloc(y_size);
    img->planes[1] = malloc(uv_size);
    img->planes[2] = malloc(uv_size);
    img->stride[0] = w;
    img->stride[1] = w / 2;
    img->stride[2] = w / 2;
    
    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        return 0;
    }

    // Initialize with simple pattern
    for (int i = 0; i < y_size; i++) img->planes[0][i] = i % 256;
    for (int i = 0; i < uv_size; i++) img->planes[1][i] = 128;
    for (int i = 0; i < uv_size; i++) img->planes[2][i] = 128;

    img->sz = y_size + 2 * uv_size;
    img->bps = 8;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 1;

    return 1;
}

static void free_image(aom_image_t *img) {
    if (img == NULL) return;
    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
}

static aom_codec_iface_t *get_av1_encoder_iface() {
    return aom_codec_av1_cx();
}

int LLVMFuzzerTestOneInput_269(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Parse basic parameters from input
    unsigned int width = (Data[0] % 64) + 16;
    unsigned int height = (Data[1] % 64) + 16;
    int bit_depth = (Data[2] % 5) + 8; // 8, 10, 12, 14, 16
    int use_null_img = (Data[3] % 2);

    // Ensure width and height are multiples of 2 for I420
    width = (width / 2) * 2;
    height = (height / 2) * 2;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_codec_iface_t *iface = get_av1_encoder_iface();

    if (!iface) return 0;

    // Initialize config
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    if (!use_null_img) {
        if (!create_dummy_image(&img, width, height, bit_depth)) {
            aom_codec_destroy(&ctx);
            return 0;
        }
    }

    // Fuzz aom_codec_encode with various parameters
    aom_codec_pts_t pts = (aom_codec_pts_t)(Data[4] * 256 + Data[5]);
    unsigned long duration = (Data[6] % 100) + 1;
    aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)(Data[7] & 0xFF);

    // Call encode
    aom_image_t *img_ptr = use_null_img ? NULL : &img;
    res = aom_codec_encode(&ctx, img_ptr, pts, duration, flags);
    // Ignore return value for fuzzing purposes, just ensure no crash

    // Try to flush
    while (res == AOM_CODEC_OK) {
        aom_codec_cx_pkt_t *pkt;
        res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;
        
        // Drain packets
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // aom_codec_get_cx_data returns a pointer to internal data, 
            // do not free it manually. The context manages it.
            aom_codec_iter_t iter = NULL;
            // Just consume to avoid accumulation if needed, but usually 
            // getting the next one is enough for fuzzing loop logic.
            // If specific packet processing is needed, it would be done here.
        }
    }

    // Cleanup image
    if (!use_null_img) {
        free_image(&img);
    }

    // Cleanup context
    aom_codec_destroy(&ctx);

    return 0;
}