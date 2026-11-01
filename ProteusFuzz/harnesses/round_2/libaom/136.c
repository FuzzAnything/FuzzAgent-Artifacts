// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Correct function name to get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic parameters
    // Ensure width and height are valid (multiples of 16 for AV1 usually, min 16)
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame for encoding context
    aom_image_t frame;
    if (aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fill frame with dummy data if Size allows
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = (cfg.g_w * cfg.g_h) / 4;
    
    if (Size >= y_size + 2 * uv_size) {
        memcpy(frame.planes[0], Data, y_size);
        memcpy(frame.planes[1], Data + y_size, uv_size);
        memcpy(frame.planes[2], Data + y_size + uv_size, uv_size);
    } else {
        // Fill with zeros if data is insufficient
        memset(frame.planes[0], 0, y_size);
        memset(frame.planes[1], 0, uv_size);
        memset(frame.planes[2], 0, uv_size);
    }

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    int val_diag = (Size > 2) ? (Data[2] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val_diag);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    int val_interintra = (Size > 3) ? (Data[3] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val_interintra);

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    // This control typically expects a pointer to a struct, but we pass an int to trigger error handling or valid behavior if cast works.
    int val_lf = (Size > 4) ? (Data[4] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, val_lf);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    int val_drop = (Size > 5) ? (Data[5] % 100) : 0;
    aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val_drop);

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    int val_dct = (Size > 6) ? (Data[6] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val_dct);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    int val_paeth = (Size > 7) ? (Data[7] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val_paeth);

    // Attempt to encode a frame to exercise the state
    aom_codec_cx_pkt_t pkt;
    aom_codec_encode(&ctx, &frame, 0, 1, 0);
    aom_codec_get_cx_data(&ctx, &pkt);
    // Free any output packets
    if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
        aom_img_free(&pkt.data.frame);
    }

    // Cleanup
    aom_img_free(&frame);
    cleanup_codec(&ctx);

    return 0;
}