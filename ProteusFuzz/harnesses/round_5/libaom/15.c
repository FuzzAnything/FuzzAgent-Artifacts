// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzzed data to set some configuration parameters
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] * 1000) : 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    // Fill with fuzzed data if available
    if (Size > 4) {
        memcpy(img.planes[0], Data + 4, Size - 4);
    }

    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t res;

    // Fuzz various control functions using the correct enum constants
    // 1. AV1E_SET_FRAME_PERIODIC_BOOST
    int32_t boost_val = (Size > 5) ? Data[5] : 0;
    res = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, boost_val);
    (void)res;

    // 2. AOME_SET_CQ_LEVEL
    int32_t cq_level = (Size > 6) ? Data[6] : 0;
    res = aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, cq_level);
    (void)res;

    // 3. AV1E_SET_FORCE_VIDEO_MODE
    int32_t force_video = (Size > 7) ? Data[7] : 0;
    res = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video);
    (void)res;

    // 4. AV1E_SET_COEFF_COST_UPD_FREQ
    int32_t coeff_freq = (Size > 8) ? Data[8] : 0;
    res = aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_freq);
    (void)res;

    // 5. AV1E_SET_DELTAQ_MODE
    int32_t deltaq_mode = (Size > 9) ? Data[9] : 0;
    res = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);
    (void)res;

    // 6. AOME_SET_ENABLEAUTOBWDREF
    int32_t enable_auto = (Size > 10) ? Data[10] : 0;
    res = aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, enable_auto);
    (void)res;

    // Encode the frame to exercise the codec state
    // Fixed: aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags
    res = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        aom_img_free(&img);
        return 0;
    }

    // Drain packets
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Optionally process pkt.data.frame
        }
    }

    cleanup_encoder(&ctx);
    aom_img_free(&img);

    return 0;
}