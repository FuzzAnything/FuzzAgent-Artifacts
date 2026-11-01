// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;
    // Fixed: Removed allow_highbitdepth, using default or valid flags only
    cfg.allow_lowbitdepth = 0;

    if (aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy input data if needed
    if (Size > 0) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            aom_codec_ctx_t enc_ctx;
            memset(&enc_ctx, 0, sizeof(enc_ctx));
            aom_codec_enc_cfg_t enc_cfg;
            aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            enc_cfg.g_w = 64;
            enc_cfg.g_h = 64;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;

            if (aom_codec_enc_init(&enc_ctx, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                // Create a dummy frame to encode
                aom_image_t img;
                aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
                
                // Fixed: Use img.h and img.stride[0] instead of d_y
                size_t max_bytes = (size_t)img.h * img.stride[0];
                size_t copy_size = (Size < max_bytes) ? Size : max_bytes;
                
                if (copy_size > 0) {
                    memcpy(img.planes[0], Data, copy_size);
                }

                // Fixed: Use correct type aom_codec_frame_flags_t
                aom_codec_frame_flags_t flags = 0;
                aom_codec_encode(&enc_ctx, &img, 0, 1, flags);

                aom_codec_iter_t iter = NULL;
                const aom_codec_cx_pkt_t *pkt;
                
                // Fixed: Correct function call signature for aom_codec_get_cx_data
                while ((pkt = aom_codec_get_cx_data(&enc_ctx, &iter)) != NULL) {
                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                        // Use encoded data as input for decoder
                        aom_codec_decode(&ctx, pkt->data.frame.buf, pkt->data.frame.sz, NULL);
                        
                        // Test the target control functions
                        int8_t ref_updates[8];
                        aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);

                        int corrupted;
                        aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);

                        int show_frame_flag;
                        aom_codec_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

                        int show_existing_frame_flag;
                        aom_codec_control(&ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing_frame_flag);

                        int8_t ref_used[8];
                        aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, ref_used);

                        int frame_flags;
                        aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
                    }
                }
                aom_codec_destroy(&enc_ctx);
                aom_img_free(&img);
            }
        }
    } else {
        // Try decoding with empty or minimal data to trigger edge cases
        aom_codec_decode(&ctx, NULL, 0, NULL);
        
        // Test control functions even with no data
        int8_t ref_updates[8];
        aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);

        int corrupted;
        aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted);

        int show_frame_flag;
        aom_codec_control(&ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

        int show_existing_frame_flag;
        aom_codec_control(&ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &show_existing_frame_flag);

        int8_t ref_used[8];
        aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, ref_used);

        int frame_flags;
        aom_codec_control(&ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    }

    cleanup_codec(&ctx);
    return 0;
}