// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_ctx_t enc_ctx = {0};
    
    const aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();

    if (!decoder_iface || !encoder_iface) {
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 64;
    dec_cfg.h = 64;

    aom_codec_enc_cfg_t enc_cfg = {0};
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;

    // Initialize Decoder
    if (aom_codec_dec_init(&ctx, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize Encoder
    if (aom_codec_enc_init(&enc_ctx, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Create a dummy image for encoding to get a valid frame to decode
    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 1) != 0) {
        cleanup_codec(&ctx);
        cleanup_codec(&enc_ctx);
        return 0;
    }
    
    // Initialize image planes
    size_t y_size = (size_t)img.stride[0] * (size_t)img.d_h;
    size_t u_size = (size_t)img.stride[1] * (size_t)(img.d_h / 2);
    size_t v_size = (size_t)img.stride[2] * (size_t)(img.d_h / 2);

    memset(&img.planes[0][0], 0, y_size);
    memset(&img.planes[1][0], 128, u_size);
    memset(&img.planes[2][0], 128, v_size);

    aom_codec_frame_flags_t flags = 0;
    aom_codec_encode(&enc_ctx, &img, 0, 1, flags);

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt = aom_codec_get_frame(&enc_ctx, &iter);
    
    // Check if we successfully got a packet before dereferencing
    if (pkt && pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Ensure buffer is not null and size is valid
        if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
            if (aom_codec_decode(&ctx, pkt->data.frame.buf, pkt->data.frame.sz, NULL) == AOM_CODEC_OK) {
                iter = NULL;
                pkt = aom_codec_get_frame(&ctx, &iter);
                if (pkt && pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // 1. AOMD_GET_STILL_PICTURE
                    aom_still_picture_info still_info;
                    memset(&still_info, 0, sizeof(still_info));
                    aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

                    // 2. AOMD_GET_ORDER_HINT
                    int order_hint;
                    aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

                    // 3. AOMD_GET_LAST_REF_USED
                    int last_ref_used[8];
                    memset(last_ref_used, 0, sizeof(last_ref_used));
                    aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, last_ref_used);

                    // 4. AOMD_GET_FWD_KF_PRESENT
                    int fwd_kf_present;
                    aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

                    // 5. AOMD_GET_S_FRAME_INFO
                    aom_s_frame_info s_frame_info;
                    memset(&s_frame_info, 0, sizeof(s_frame_info));
                    aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

                    // 6. AOMD_GET_ALTREF_PRESENT
                    int altref_present;
                    aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
                }
            }
        }
    }

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);
    cleanup_codec(&enc_ctx);

    return 0;
}