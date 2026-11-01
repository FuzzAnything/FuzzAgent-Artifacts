// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed cpu_used and low_delay as they are not members of aom_codec_dec_cfg in newer libaom versions
    // cfg.cpu_used = 0; 
    // cfg.low_delay = 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    // Attempt to decode the input data
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Iterate through decoded frames to potentially find a valid frame to query
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // 1. aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT
        int altref_present = 0;
        aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

        // 2. aom_codec_control_typechecked_AOMD_GET_ORDER_HINT
        int order_hint = 0;
        aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

        // 3. aom_codec_control_typechecked_AOMD_GET_STILL_PICTURE
        aom_still_picture_info still_info = {0};
        aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

        // 4. aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS
        uint32_t frame_flags = 0;
        aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

        // 5. aom_codec_control_typechecked_AOMD_GET_FWD_KF_PRESENT
        int fwd_kf_present = 0;
        aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

        // 6. aom_codec_control_typechecked_AOMD_GET_S_FRAME_INFO
        aom_s_frame_info s_frame_info = {0};
        aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

        aom_img_free(img);
    }

    cleanup_codec(&ctx);
    return 0;
}