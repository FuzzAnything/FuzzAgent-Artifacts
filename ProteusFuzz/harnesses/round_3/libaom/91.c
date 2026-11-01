// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int id, void *data) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, id, data);
    if (res != AOM_CODEC_OK) return 0;
    return 1;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;

    // Attempt to decode a frame (using input data as bitstream)
    aom_codec_err_t decode_res = aom_codec_decode(&ctx, Data, Size, NULL);
    
    // Iterate over packets if decode was successful or just proceed
    if (decode_res == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Try various control commands on the decoded frame context
                aom_s_frame_info s_frame_info;
                memset(&s_frame_info, 0, sizeof(s_frame_info));
                fuzz_decoder_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

                aom_still_picture_info still_info;
                memset(&still_info, 0, sizeof(still_info));
                fuzz_decoder_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

                int altref_present;
                fuzz_decoder_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

                int fwd_kf_present;
                fuzz_decoder_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

                int order_hint;
                fuzz_decoder_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

                int last_ref_used[8];
                memset(last_ref_used, 0, sizeof(last_ref_used));
                fuzz_decoder_control(&ctx, AOMD_GET_LAST_REF_USED, last_ref_used);
            }
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}