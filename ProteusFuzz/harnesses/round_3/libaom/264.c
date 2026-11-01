// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    // Extract configuration parameters from fuzz input
    int width = (Data[0] % 256) + 16;
    int height = (Data[1] % 256) + 16;
    int fps_num = (Data[2] % 30) + 1;
    int fps_den = (Data[3] % 30) + 1;
    int threads = (Data[4] % 4) + 1;
    int cpu_used = (Data[5] % 8);
    
    // Ensure dimensions are multiples of 8 for AV1
    width = (width / 8) * 8;
    height = (height / 8) * 8;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, cpu_used) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = threads;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy frame to encode
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill image with fuzz data
    for (int y = 0; y < img.h; y++) {
        for (int x = 0; x < img.w; x++) {
            int idx = (y * img.stride[0]) + x;
            img.planes[0][idx] = Data[idx % Size];
        }
    }
    // Fill UV planes
    for (int p = 1; p < 3; p++) {
        for (int y = 0; y < img.h >> 1; y++) {
            for (int x = 0; x < img.w >> 1; x++) {
                int idx = (y * img.stride[p]) + x;
                img.planes[p][idx] = Data[(idx + Size/2) % Size];
            }
        }
    }

    aom_enc_frame_flags_t flags = 0;
    if (Size > 10 && Data[10] & 1) flags |= AOM_EFLAG_FORCE_KF;

    // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
    aom_codec_encode(&codec, &img, 0, 1, flags);

    // Fuzz the specific control functions mentioned in the task
    // We map the input data to different control IDs and values
    
    // Control 1: AV1E_SET_S_FRAME_MODE
    if (Size > 0) {
        int val = Data[0] % 2;
        aom_codec_control(&codec, AV1E_SET_S_FRAME_MODE, val);
    }

    // Control 2: AV1E_SET_ENABLE_OVERLAY
    if (Size > 1) {
        int val = Data[1] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_OVERLAY, val);
    }

    // Control 3: AV1E_SET_ENABLE_TX64
    if (Size > 2) {
        int val = Data[2] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_TX64, val);
    }

    // Control 4: AV1E_SET_ENABLE_MASKED_COMP
    if (Size > 3) {
        int val = Data[3] % 2;
        aom_codec_control(&codec, AV1E_SET_ENABLE_MASKED_COMP, val);
    }

    // Control 5: AOME_SET_ENABLEAUTOALTREF
    if (Size > 4) {
        int val = Data[4] % 2;
        aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF, val);
    }

    // Control 6: AV1E_SET_REDUCED_TX_TYPE_SET
    if (Size > 5) {
        int val = Data[5] % 2;
        aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    }

    // Drain the encoder
    // Fixed: Use aom_codec_cx_pkt_t for getting frames, not aom_codec_frame_info_t
    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_frame(&codec, &pkt) == AOM_CODEC_OK) {
        // Consume frames
    }

    aom_codec_destroy(&codec);
    aom_img_free(&img);

    return 0;
}