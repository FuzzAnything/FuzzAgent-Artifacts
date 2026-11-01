// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_image.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // AV1E_SET_QM_MAX
    if (size > 0) {
        int val = (int)data[0];
        aom_codec_control(ctx, AV1E_SET_QM_MAX, &val);
    }

    // AOME_SET_ENABLEAUTOBWDREF
    if (size > 1) {
        int val = (int)data[1];
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, &val);
    }

    // AV1E_SET_MAX_REFERENCE_FRAMES
    if (size > 2) {
        int val = (int)data[2];
        aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &val);
    }

    // AOME_SET_ENABLEAUTOALTREF
    if (size > 3) {
        int val = (int)data[3];
        aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, &val);
    }

    // AV1_GET_REFERENCE
    if (size > 4) {
        av1_ref_frame_t ref = {0};
        ref.idx = (int)data[4];
        ref.use_external_ref = (size > 5) ? (int)data[5] : 0;
        // Initialize image structure minimally
        aom_image_t img = {0};
        img.w = (size > 6) ? (unsigned int)data[6] : 64;
        img.h = (size > 7) ? (unsigned int)data[7] : 64;
        img.fmt = AOM_IMG_FMT_I420;
        img.bit_depth = 8;
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 1;
        img.planes[0] = NULL;
        img.planes[1] = NULL;
        img.planes[2] = NULL;
        img.stride[0] = 0;
        img.stride[1] = 0;
        img.stride[2] = 0;
        ref.img = img;

        aom_codec_control(ctx, AV1_GET_REFERENCE, &ref);
    }

    // AV1_SET_REFERENCE
    if (size > 8) {
        av1_ref_frame_t ref = {0};
        ref.idx = (int)data[8];
        ref.use_external_ref = (size > 9) ? (int)data[9] : 0;
        // For SET, we might need a valid image or just pass the struct
        // Depending on implementation, this might fail if img is not valid,
        // which is expected in fuzzing.
        aom_codec_control(ctx, AV1_SET_REFERENCE, &ref);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder context
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Set some minimal config based on input if available
    if (Size > 0) {
        cfg.g_w = (unsigned int)Data[0] % 1024 + 16;
        if (Size > 1) cfg.g_h = (unsigned int)Data[1] % 1024 + 16;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}