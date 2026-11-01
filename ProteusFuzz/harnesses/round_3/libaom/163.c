// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz input to vary configuration slightly
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1] % 1024) + 16;
        cfg.g_timebase.num = (Data[2] % 100) + 1;
        cfg.g_timebase.den = (Data[3] % 100) + 1;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input data
    int enable_smooth_interintra = (Size > 4) ? (Data[4] % 2) : 0;
    int enable_interintra_comp = (Size > 5) ? (Data[5] % 2) : 0;
    int enable_directional_intra = (Size > 6) ? (Data[6] % 2) : 0;
    int enable_interintra_wedge = (Size > 7) ? (Data[7] % 2) : 0;
    int enable_diagonal_intra = (Size > 8) ? (Data[8] % 2) : 0;
    int enable_paeth_intra = (Size > 9) ? (Data[9] % 2) : 0;

    // Apply controls using aom_codec_control
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_smooth_interintra);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_interintra_comp);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_directional_intra);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diagonal_intra);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth_intra);

    // Create a dummy image to encode to ensure state changes are exercised
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image with fuzz data if available
    if (Size > 10) {
        size_t plane_size = img.stride[0] * img.h;
        size_t copy_size = (Size - 10) < plane_size ? (Size - 10) : plane_size;
        memcpy(img.planes[0], Data + 10, copy_size);
    }

    aom_codec_frame_flags_t flags = 0;
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Drain the encoder
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // No need to free image here, the encoder manages the frame buffer
        }
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}