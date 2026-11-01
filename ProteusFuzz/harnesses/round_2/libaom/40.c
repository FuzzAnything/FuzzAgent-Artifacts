// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx || !ctx->priv) return 0;
    
    aom_codec_err_t res = aom_codec_control(ctx, control_id, arg);
    
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    aom_codec_err_t decode_res = aom_codec_decode(&ctx, Data, Size, &iter);

    if (decode_res == AOM_CODEC_OK || decode_res == AOM_CODEC_OK) {
        while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
            aom_s_frame_info s_frame_info;
            memset(&s_frame_info, 0, sizeof(s_frame_info));
            fuzz_decoder_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

            int frame_flags;
            fuzz_decoder_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

            int altref_present;
            fuzz_decoder_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

            int last_ref_updates;
            fuzz_decoder_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);

            int last_ref_used;
            fuzz_decoder_control(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);

            int frame_corrupted;
            fuzz_decoder_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);
            
            aom_img_free(img);
        }
    }

    aom_codec_destroy(&ctx);

    return 0;
}