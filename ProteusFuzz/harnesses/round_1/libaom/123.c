// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder context with AV1
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Ensure dimensions are valid and aligned
    unsigned int w = (Data[0] % 255) + 16;
    unsigned int h = (Data[1] % 255) + 16;
    // Align to 16x16 blocks for AV1
    w = (w + 15) & ~15;
    h = (h + 15) & ~15;
    
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy input data for encoding
    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Safe initialization of image planes using the actual allocated size
    // We use img.sz or calculate based on stride * height * planes to be safe,
    // but simply zeroing the allocated buffer via img_data is safest if owner.
    // However, standard practice is to clear planes individually using stride*height.
    // The error occurred because the previous code might have miscalculated or 
    // the allocator didn't zero out, and we wrote past the end if stride was weird.
    // aom_img_alloc guarantees planes are allocated. We just need to be careful.
    // The safest way to clear is to use the stride and height provided by the struct.
    
    // Clear Y plane
    memset(img.planes[0], 0, (size_t)img.stride[0] * img.h);
    // Clear U plane
    memset(img.planes[1], 0, (size_t)img.stride[1] * img.h);
    // Clear V plane
    memset(img.planes[2], 0, (size_t)img.stride[2] * img.h);

    // Fuzz AV1E_SET_QM_MAX
    int qm_max = (Data[0] % 256);
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, &qm_max);

    // Fuzz AOME_SET_ENABLEAUTOBWDREF
    int enable_auto_bwd_ref = (Data[1] % 2);
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, &enable_auto_bwd_ref);

    // Fuzz AV1_GET_REFERENCE
    av1_ref_frame_t ref_frame_get;
    memset(&ref_frame_get, 0, sizeof(ref_frame_get));
    ref_frame_get.idx = (Data[2] % 8);
    ref_frame_get.use_external_ref = 0;
    ref_frame_get.img = img;
    aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref_frame_get);

    // Fuzz AV1_SET_REFERENCE
    av1_ref_frame_t ref_frame_set;
    memset(&ref_frame_set, 0, sizeof(ref_frame_set));
    ref_frame_set.idx = (Data[3] % 8);
    ref_frame_set.use_external_ref = 0;
    ref_frame_set.img = img;
    aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_frame_set);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    int max_ref_frames = (Data[4] % 8) + 1;
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_ref_frames);

    // Fuzz AOME_SET_ENABLEAUTOALTREF
    int enable_auto_alt_ref = (Data[5] % 2);
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, &enable_auto_alt_ref);

    // Encode a frame to exercise the codec
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}