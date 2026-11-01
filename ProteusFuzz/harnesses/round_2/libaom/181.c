// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (size < 1) return 0;
    val = data[0];

    // AV1E_SET_ENABLE_INTERINTRA_COMP
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    if (ret != AOM_CODEC_OK) {
        // Fuzzing can tolerate errors, just continue
    }

    if (size > 1) {
        val = data[1];
        // AV1E_SET_ENABLE_RECT_PARTITIONS
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
    }

    if (size > 2) {
        val = data[2];
        // AV1E_SET_CDF_UPDATE_MODE
        ret = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val);
    }

    if (size > 3) {
        val = data[3];
        // AV1E_SET_INTRA_DCT_ONLY
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, val);
    }

    if (size > 4) {
        val = data[4];
        // AV1E_SET_ENABLE_WARPED_MOTION
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, val);
    }

    if (size > 5) {
        val = data[5];
        // AV1E_SET_DELTALF_MODE
        ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int i;
    size_t y_size, uv_size;

    if (Size < 10) return 0;

    // Initialize encoder config
    memset(&cfg, 0, sizeof(cfg));
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);

    // Use fuzzing input to set some config parameters safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 30) + 1;
    cfg.g_timebase.den = (Data[3] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;
    cfg.g_threads = (Data[5] % 8) + 1;
    cfg.g_error_resilient = (Data[6] % 3);
    cfg.g_lag_in_frames = (Data[7] % 20);
    cfg.rc_end_usage = (Data[8] % 3); // VBR, CBR, QMODE
    cfg.g_pass = (Data[9] % 2) + 1;

    // Get the encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize the codec context
    memset(&ctx, 0, sizeof(ctx));
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy frame to encode
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Calculate plane sizes based on image dimensions
    y_size = (size_t)img.d_w * img.d_h;
    uv_size = y_size / 4;

    // Fill image with fuzzing data safely
    for (i = 0; i < (int)y_size; i++) {
        img.planes[0][i] = Data[i % Size];
    }
    for (i = 0; i < (int)uv_size; i++) {
        img.planes[1][i] = Data[(i + Size) % Size];
        img.planes[2][i] = Data[(i + (Size >> 1)) % Size];
    }

    // Encode the frame
    aom_codec_frame_flags_t flags = 0;
    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));

    res = aom_codec_encode(&ctx, &img, 0, 1, flags);
    if (res != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Process packets
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Packet processed
        }
        // No specific destroy function for individual packets in standard API, 
        // just let the loop continue or break. The data is managed by the codec.
        // If we need to free data inside pkt, we would check pkt.data and free it if needed,
        // but typically aom_codec_get_cx_data handles lifecycle or returns const data.
        // In many versions, we just don't free pkt.data here unless it was malloc'd by us.
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}