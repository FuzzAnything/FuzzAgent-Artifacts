// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Initialize codec context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    cfg.g_timebase.den = Data[3] ? Data[3] : 30;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] * 1000) : 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_set_option
    if (Size > 10) {
        char key[64], val[64];
        size_t key_len = (Size > 20) ? (Data[10] % 20) : 5;
        size_t val_len = (Size > 25) ? (Data[11] % 20) : 5;
        if (key_len > 63) key_len = 63;
        if (val_len > 63) val_len = 63;
        memcpy(key, Data + 12, key_len);
        key[key_len] = '\0';
        memcpy(val, Data + 12 + key_len, val_len);
        val[val_len] = '\0';
        aom_codec_set_option(&ctx, key, val);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE
    if (Size > 15) {
        int mode = Data[15] % 3; // 0, 1, or 2
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, mode);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE
    if (Size > 16) {
        int max_part_size = Data[16] % 8; // 0 to 7
        aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, max_part_size);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_EXTERNAL_PARTITION
    if (Size > 17) {
        // We need to create a dummy external partition model
        aom_ext_part_funcs_t funcs = {0};
        funcs.decision_mode = (Data[17] % 2) ? AOM_EXT_PART_RECURSIVE : AOM_EXT_PART_WHOLE_TREE;
        aom_codec_control(&ctx, AV1E_SET_EXTERNAL_PARTITION, &funcs);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX
    if (Size > 18) {
        int enable = (Data[18] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable);
    }

    // Create a dummy image for encoding
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img.planes[0]) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = img.stride[0] * img.h;
    if (Size > 20 && Size >= plane_size) {
        memcpy(img.planes[0], Data + 20, plane_size);
    } else {
        memset(img.planes[0], 0, plane_size);
    }

    // Encode a few frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Size > 22) ? (Data[22] % 100 + 1) : 1;
    aom_enc_frame_flags_t flags = (Size > 23) ? (Data[23] % 4) : 0;

    for (int i = 0; i < 3; i++) {
        aom_codec_encode(&ctx, &img, pts, duration, flags);
        pts += duration;
        if (i == 0) {
            // Try flushing
            aom_codec_encode(&ctx, NULL, 0, 0, 0);
        }
    }

    // Cleanup
    cleanup_image(&img);
    cleanup_codec(&ctx);

    return 0;
}