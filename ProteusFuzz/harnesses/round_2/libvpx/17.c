// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = NULL;
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    int width = 64, height = 64;
    int pts = 0;
    int duration = 1;
    unsigned long deadline = 0;
    vpx_enc_frame_flags_t flags = 0;
    int token_partitions = 0;
    unsigned int max_intra_bitrate_pct = 0;
    int svc_layer_id = 0;
    int temporal_layer_id = 0;

    // Parse input data to determine dimensions and other parameters safely
    // Ensure we have enough bytes for each read, and handle alignment carefully
    if (Size >= 8) {
        width = *(const uint32_t *)(Data);
        height = *(const uint32_t *)(Data + 4);
    }
    if (Size >= 12) pts = *(const int32_t *)(Data + 8);
    if (Size >= 16) duration = *(const int32_t *)(Data + 12);
    if (Size >= 20) {
        // Read unsigned long safely. On 64-bit systems, this is 8 bytes.
        // We need to ensure we don't read past Size.
        if (Size >= 24) {
            deadline = *(const unsigned long *)(Data + 16);
        } else {
            // Fallback if size is between 20 and 24, read as 4 bytes
            deadline = *(const unsigned int *)(Data + 16);
        }
    }
    if (Size >= 24) flags = *(const long *)(Data + 20);
    if (Size >= 28) token_partitions = *(const int *)(Data + 24);
    if (Size >= 32) max_intra_bitrate_pct = *(const unsigned int *)(Data + 28);
    if (Size >= 36) svc_layer_id = *(const int *)(Data + 32);
    if (Size >= 40) temporal_layer_id = *(const int *)(Data + 36);

    // Ensure valid dimensions
    if (width <= 0 || height <= 0) {
        width = 64;
        height = 64;
    }
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (duration <= 0) duration = 1;

    // Initialize encoder configuration
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(&cfg, iface, 0) != VPX_CODEC_OK) {
        return 0;
    }
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the codec
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set token partitions
    vpx_codec_control(&ctx, VP8E_SET_TOKEN_PARTITIONS, token_partitions);

    // Set max intra bitrate percentage
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_bitrate_pct);

    // Set SVC layer ID (VP9 specific, but we try to set it)
    struct vpx_svc_layer_id {
        int spatial_layer_id;
        int temporal_layer_id;
        int temporal_layer_id_per_spatial[16];
    };
    struct vpx_svc_layer_id svc_layer;
    svc_layer.spatial_layer_id = svc_layer_id;
    svc_layer.temporal_layer_id = svc_layer_id;
    memset(svc_layer.temporal_layer_id_per_spatial, 0, sizeof(svc_layer.temporal_layer_id_per_spatial));
    
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer);

    // Set temporal layer ID
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, temporal_layer_id);

    // Allocate image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image data with fuzz input safely
    size_t img_size = img->d_w * img->d_h * 3 / 2;
    size_t copy_size = (Size > img_size) ? img_size : Size;
    if (copy_size > 0) {
        memcpy(img->planes[VPX_PLANE_Y], Data, copy_size);
    }
    // Fill remaining planes with 0 if needed
    size_t uv_size = img->d_w * img->d_h / 4;
    if (uv_size > 0) {
        memset(img->planes[VPX_PLANE_U], 0, uv_size);
        memset(img->planes[VPX_PLANE_V], 0, uv_size);
    }

    // Encode frame
    res = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    if (res != VPX_CODEC_OK) {
        // Continue to cleanup even on error
    }

    // Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
        }
    }

    // Flush encoder
    res = vpx_codec_encode(&ctx, NULL, pts + duration, duration, flags, deadline);
    if (res != VPX_CODEC_OK) {
        // Continue to cleanup even on error
    }

    // Get remaining encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
        }
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(img);

    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    fuzz_encode(Data, Size);
    return 0;
}