// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

static int init_encoder(aom_codec_ctx_t *codec, const aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure basic parameters safely
    if (Size >= 4) {
        uint32_t width = ((uint32_t)Data[0] << 8) | Data[1];
        uint32_t height = ((uint32_t)Data[2] << 8) | Data[3];
        // Ensure dimensions are non-zero and within reasonable limits
        if (width == 0) width = 64;
        if (height == 0) height = 64;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
        cfg.g_w = width;
        cfg.g_h = height;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (init_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    // Fuzzing values for control commands
    int val = 0;
    if (Size >= 1) {
        val = Data[0];
    }

    // 1. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    aom_codec_control(&codec, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);

    // 2. AV1E_SET_ENABLE_INTERINTRA_WEDGE
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);

    // 3. AV1E_SET_RENDER_SIZE
    // The API expects an int array of size 2: {width, height}
    int render_size[2] = {0, 0};
    if (Size >= 4) {
        int rw = ((uint32_t)Data[0] << 8) | Data[1];
        int rh = ((uint32_t)Data[2] << 8) | Data[3];
        if (rw == 0) rw = 64;
        if (rh == 0) rh = 64;
        if (rw > 4096) rw = 4096;
        if (rh > 4096) rh = 4096;
        render_size[0] = rw;
        render_size[1] = rh;
    } else {
        render_size[0] = 64;
        render_size[1] = 64;
    }
    aom_codec_control(&codec, AV1E_SET_RENDER_SIZE, render_size);

    // 4. AV1E_SET_ENABLE_SUPERRES
    aom_codec_control(&codec, AV1E_SET_ENABLE_SUPERRES, val);

    // 5. AV1E_SET_MIN_PARTITION_SIZE
    int min_part = 0;
    if (Size >= 1) {
        min_part = Data[0];
        if (min_part > 128) min_part = 128;
    }
    aom_codec_control(&codec, AV1E_SET_MIN_PARTITION_SIZE, min_part);

    // 6. AV1E_SET_REDUCED_TX_TYPE_SET
    int tx_set = 0;
    if (Size >= 2) {
        tx_set = ((uint32_t)Data[1] << 8) | Data[2];
    }
    aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, tx_set);

    cleanup_codec(&codec);
    return 0;
}