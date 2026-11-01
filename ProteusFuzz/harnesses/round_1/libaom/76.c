// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int try_encode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 32, 32, 1);
    
    // Fill image with fuzz data safely, ensuring we do not read past 'size'
    size_t max_y = (size_t)img.d_h;
    size_t max_uv = (size_t)(img.d_h / 2);
    
    // Luma plane
    for (size_t i = 0; i < max_y; i++) {
        size_t bytes_to_copy = img.d_w;
        if (i * img.d_w + bytes_to_copy > size) {
            bytes_to_copy = (size > i * img.d_w) ? (size - i * img.d_w) : 0;
            if (bytes_to_copy == 0) break;
        }
        memcpy(img.planes[0] + i * img.stride[0], data + i * img.d_w, bytes_to_copy);
        // Zero out the rest of the row if partial
        if (bytes_to_copy < img.d_w) {
            memset(img.planes[0] + i * img.stride[0] + bytes_to_copy, 0, img.d_w - bytes_to_copy);
        }
    }
    
    // U and V planes
    for (size_t i = 0; i < max_uv; i++) {
        size_t bytes_to_copy = img.d_w / 2;
        size_t offset = (size_t)img.d_h * img.d_w + i * (img.d_w / 2);
        
        if (offset + bytes_to_copy > size) {
            bytes_to_copy = (size > offset) ? (size - offset) : 0;
            if (bytes_to_copy == 0) break;
        }
        
        memcpy(img.planes[1] + i * img.stride[1], data + offset, bytes_to_copy);
        memcpy(img.planes[2] + i * img.stride[2], data + offset, bytes_to_copy);
        
        if (bytes_to_copy < img.d_w / 2) {
            memset(img.planes[1] + i * img.stride[1] + bytes_to_copy, 0, img.d_w / 2 - bytes_to_copy);
            memset(img.planes[2] + i * img.stride[2] + bytes_to_copy, 0, img.d_w / 2 - bytes_to_copy);
        }
    }

    aom_enc_frame_flags_t flags = 0;
    if (size > 0 && data[0] & 1) flags |= AOM_EFLAG_FORCE_KF;

    aom_codec_encode(ctx, &img, 0, 1, flags);
    
    aom_img_free(&img);
    return 1;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_usage = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Extract values from input for control parameters
    int enable_order_hint = (Data[0] % 2);
    int enable_interinter_wedge = (Data[1] % 2);
    int coeff_cost_upd_freq = (Data[2] % 100);
    int enable_smooth_interintra = (Data[3] % 2);
    
    int reduced_ref_set = 0;
    if (Size > 4) {
        reduced_ref_set = (Data[4] % 2);
    }

    int enable_intra_edge_filter = 0;
    if (Size > 5) {
        enable_intra_edge_filter = (Data[5] % 2);
    }

    // Invoke control functions
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, enable_order_hint);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, enable_interinter_wedge);
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_smooth_interintra);
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref_set);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_intra_edge_filter);

    // Encode a frame to exercise the encoder
    try_encode_frame(&ctx, Data, Size);

    // Flush and cleanup
    aom_codec_encode(&ctx, NULL, 0, 1, 0);
    aom_codec_destroy(&ctx);

    return 0;
}