// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int try_fuzz_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->priv) return 0;

    int ret = 0;

    // 1. AV1D_SET_EXT_REF_PTR
    if (size >= 2) {
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));
        // Create a dummy image if data allows
        if (size > 4) {
            aom_image_t img;
            memset(&img, 0, sizeof(img));
            img.w = data[0];
            img.h = data[1];
            img.bit_depth = data[2];
            img.fmt = (aom_img_fmt_t)data[3];
            // Allocate minimal planes to avoid null pointer issues if codec checks
            if (img.w > 0 && img.h > 0) {
                size_t plane_size = (size_t)img.w * img.h;
                img.planes[0] = malloc(plane_size);
                img.planes[1] = malloc(plane_size / 4);
                img.planes[2] = malloc(plane_size / 4);
                img.stride[0] = img.w;
                img.stride[1] = img.w / 2;
                img.stride[2] = img.w / 2;
                img.sz = plane_size;
                if (img.planes[0] && img.planes[1] && img.planes[2]) {
                    ext_ref.img = &img;
                    ext_ref.num = 1;
                    aom_codec_control(ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
                    ret = 1;
                }
                free(img.planes[0]);
                free(img.planes[1]);
                free(img.planes[2]);
            }
        }
    }

    // 2. AOMD_GET_ALTREF_PRESENT
    int altref = 0;
    aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref);
    ret = 1;

    // 3. AOMD_GET_FWD_KF_PRESENT
    int fwd_kf = 0;
    aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf);
    ret = 1;

    // 4. AOMD_GET_STILL_PICTURE
    aom_still_picture_info still_info;
    memset(&still_info, 0, sizeof(still_info));
    aom_codec_control(ctx, AOMD_GET_STILL_PICTURE, &still_info);
    ret = 1;

    // 5. AOMD_GET_ORDER_HINT
    int order_hint = 0;
    aom_codec_control(ctx, AOMD_GET_ORDER_HINT, &order_hint);
    ret = 1;

    // 6. AOMD_GET_S_FRAME_INFO
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    ret = 1;

    return ret;
}

int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize decoder context with AV1
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Attempt to decode a small chunk to initialize internal state
    // This is necessary for some control queries to be valid
    if (Size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        
        aom_codec_decode(&ctx, Data, Size, NULL);
        
        while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            // Consume packets
            aom_img_free(img);
        }
    }

    // Fuzz the control functions
    try_fuzz_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}