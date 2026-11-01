// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

// Define the control IDs if they are not present in the included headers
// These correspond to the AV1 and AOM encoder control commands
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH
#define AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH 0x10000001
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_QM_MIN
#define AOM_CODEC_CTRL_ID_AV1E_SET_QM_MIN 0x10000002
#endif
#ifndef AOM_CODEC_CTRL_ID_AOME_SET_MAX_INTRA_BITRATE_PCT
#define AOM_CODEC_CTRL_ID_AOME_SET_MAX_INTRA_BITRATE_PCT 0x10000003
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_DELTAQ_STRENGTH
#define AOM_CODEC_CTRL_ID_AV1E_SET_DELTAQ_STRENGTH 0x10000004
#endif
#ifndef AOM_CODEC_CTRL_ID_AOME_SET_SHARPNESS
#define AOM_CODEC_CTRL_ID_AOME_SET_SHARPNESS 0x10000005
#endif
#ifndef AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ
#define AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ 0x10000006
#endif

static void fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int val;
    size_t offset = 0;

    // Helper to safely read an int from the buffer
    #define READ_INT() \
        (offset + sizeof(int) <= size) ? \
        (val = *(const int*)(data + offset), offset += sizeof(int), val) : \
        (val = 0)

    // Function 1: AV1E_SET_ENABLE_TX_SIZE_SEARCH
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);

    // Function 2: AV1E_SET_QM_MIN
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_QM_MIN, val);

    // Function 3: AOME_SET_MAX_INTRA_BITRATE_PCT
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AOME_SET_MAX_INTRA_BITRATE_PCT, val);

    // Function 4: AV1E_SET_DELTAQ_STRENGTH
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_DELTAQ_STRENGTH, val);

    // Function 5: AOME_SET_SHARPNESS
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AOME_SET_SHARPNESS, val);

    // Function 6: AV1E_SET_MODE_COST_UPD_FREQ
    val = READ_INT();
    aom_codec_control(ctx, AOM_CODEC_CTRL_ID_AV1E_SET_MODE_COST_UPD_FREQ, val);
}

int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;

    // Initialize encoder config
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    memset(&img, 0, sizeof(img));
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.stride[0] = cfg.g_w;
    img.stride[1] = cfg.g_w / 2;
    img.stride[2] = cfg.g_w / 2;

    // Allocate image data
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data if available
    if (Size > 0) {
        size_t plane_size = (size_t)img.stride[0] * img.h;
        size_t copy_size = Size < plane_size ? Size : plane_size;
        memcpy(img.planes[0], Data, copy_size);
    }

    // Encode a frame
    err = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (err != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Get encoded packet
    // aom_codec_get_cx_data returns a pointer to the packet, not an error code directly in the assignment
    // We need to check the return value of the function call
    while (aom_codec_get_cx_data(&ctx, &pkt) != NULL) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
            // No explicit free function needed for the packet structure itself in this context
            // The memory is managed by the codec context
        }
    }

    // Now fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}