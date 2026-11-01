// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t err;

    if (Size < 10) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize a dummy image
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    // Fill image with fuzz data if possible
    if (Size >= cfg.g_w * cfg.g_h * 3 / 2) {
        memcpy(img.planes[0], Data, cfg.g_w * cfg.g_h);
        memcpy(img.planes[1], Data + cfg.g_w * cfg.g_h, (cfg.g_w * cfg.g_h) / 4);
        memcpy(img.planes[2], Data + cfg.g_w * cfg.g_h + (cfg.g_w * cfg.g_h) / 4, (cfg.g_w * cfg.g_h) / 4);
    }

    // Fuzz various control commands
    // AV1E_SET_ENABLE_REF_FRAME_MVS
    int val_mvs = (Data[4] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val_mvs);

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    int val_wedge = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val_wedge);

    // AV1E_SET_RTC_EXTERNAL_RC
    int val_rc = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val_rc);

    // AV1E_SET_ENABLE_ONESIDED_COMP
    int val_comp = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val_comp);

    // AOME_SET_ENABLEAUTOBWDREF
    int val_auto = (Data[8] % 2);
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val_auto);

    // AOME_USE_REFERENCE
    int val_ref = (Data[9] % 2);
    aom_codec_control(&ctx, AOME_USE_REFERENCE, val_ref);

    // Encode a frame to exercise state changes
    err = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Drain the encoder
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // The frame is stored in pkt.data.frame, which is a struct aom_codec_frame_info
            // We do not free the image here as it is managed by the codec internally
            // Accessing pkt.data.frame is sufficient to trigger state checks
            (void)pkt.data.frame; 
        }
    }

    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Run encoder fuzzing
    fuzz_encoder_controls(Data, Size);

    return 0;
}