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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_decoder_from_data(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 32;
    cfg.h = 32;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int feed_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    aom_codec_err_t res = aom_codec_decode(ctx, data, size, NULL);
    if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_decoder_from_data(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Feed some data to the decoder to potentially generate frames
    if (feed_decoder(&ctx, Data, Size) != 0) {
        cleanup_decoder(&ctx);
        return 0;
    }

    // Try to get packets to ensure internal state is populated
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Frame available, now try the control functions
            int show_frame_flag;
            aom_codec_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

            int altref_present;
            aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

            int fwd_kf_present;
            aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

            int last_ref_used;
            aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);

            int frame_flags;
            aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

            aom_s_frame_info s_frame_info;
            memset(&s_frame_info, 0, sizeof(s_frame_info));
            aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

            // Break after first frame to avoid infinite loops in fuzzing context
            break;
        }
    }

    cleanup_decoder(&ctx);
    return 0;
}