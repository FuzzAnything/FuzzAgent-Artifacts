// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret;
    int val = 0;

    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // AV1E_SET_INTER_DCT_ONLY
    ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
    (void)ret;

    // AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
    ret = aom_codec_control(ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, val);
    (void)ret;

    // AV1E_SET_QM_V
    ret = aom_codec_control(ctx, AV1E_SET_QM_V, val);
    (void)ret;

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
    (void)ret;

    // AV1E_SET_REDUCED_TX_TYPE_SET
    ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t err;
    uint8_t *plane0 = NULL;
    uint8_t *plane1 = NULL;
    uint8_t *plane2 = NULL;
    uint8_t fill_val0 = 0;
    uint8_t fill_val1 = 0;
    uint8_t fill_val2 = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Set default configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzz data if available
    if (Size > 0) {
        cfg.g_w = (Size % 1024) + 16;
        cfg.g_h = (Size % 1024) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_profile = 0;
        cfg.rc_target_bitrate = 1000;
    }

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image frame
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.bit_depth = 8;
    
    // Allocate planes
    size_t plane0_size = img.w * img.h;
    size_t plane1_size = ((img.w + 1) / 2) * ((img.h + 1) / 2);
    size_t plane2_size = ((img.w + 1) / 2) * ((img.h + 1) / 2);

    plane0 = malloc(plane0_size);
    plane1 = malloc(plane1_size);
    plane2 = malloc(plane2_size);

    if (!plane0 || !plane1 || !plane2) {
        if (plane0) free(plane0);
        if (plane1) free(plane1);
        if (plane2) free(plane2);
        aom_codec_destroy(&ctx);
        return 0;
    }

    img.planes[0] = plane0;
    img.planes[1] = plane1;
    img.planes[2] = plane2;

    // Set strides manually as required by aom_image_t
    img.stride[0] = img.w;
    img.stride[1] = (img.w + 1) / 2;
    img.stride[2] = (img.w + 1) / 2;

    // Safely determine fill values from input data
    if (Size > 0) fill_val0 = Data[0];
    if (Size > 1) fill_val1 = Data[1];
    if (Size > 2) fill_val2 = Data[2];

    // Fill image with fuzz data
    memset(img.planes[0], fill_val0, plane0_size);
    memset(img.planes[1], fill_val1, plane1_size);
    memset(img.planes[2], fill_val2, plane2_size);

    // Encode a frame
    err = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        free(plane0);
        free(plane1);
        free(plane2);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Get encoded packet
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
            (void)pkt.data.frame;
        }
    }

    // Cleanup
    free(plane0);
    free(plane1);
    free(plane2);
    aom_codec_destroy(&ctx);

    return 0;
}