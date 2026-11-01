// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aom.h"

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure encoder settings
    if (size > 0) cfg->g_w = (data[0] % 256) + 16;
    if (size > 1) cfg->g_h = (data[1] % 256) + 16;
    if (size > 2) cfg->g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg->g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg->g_threads = (data[4] % 8) + 1;
    if (size > 5) cfg->g_lag_in_frames = (data[5] % 25);
    if (size > 6) cfg->rc_target_bitrate = (data[6] % 1000) + 100;

    err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&codec, &cfg, Data, Size)) return 0;

    // Extract control parameters from fuzzing data
    int enable_cfl_intra = (Size > 0) ? (Data[0] % 2) : 0;
    int enable_ext_tile_debug = (Size > 1) ? (Data[1] % 2) : 0;
    int frame_parallel_decoding = (Size > 2) ? (Data[2] % 2) : 0;
    int enable_ab_partitions = (Size > 3) ? (Data[3] % 2) : 0;
    int chroma_sample_pos = (Size > 4) ? (Data[4] % 3) : 0; // 0, 1, or 2
    int tile_rows = (Size > 5) ? (Data[5] % 16) : 1;       // 1 to 16

    // Invoke control commands
    aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);
    aom_codec_control(&codec, AV1E_ENABLE_EXT_TILE_DEBUG, enable_ext_tile_debug);
    aom_codec_control(&codec, AV1E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding);
    aom_codec_control(&codec, AV1E_SET_ENABLE_AB_PARTITIONS, enable_ab_partitions);
    aom_codec_control(&codec, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_sample_pos);
    aom_codec_control(&codec, AV1E_SET_TILE_ROWS, tile_rows);

    // Attempt to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Manually fill image with zeros since aom_img_fill is not available
    int plane;
    for (plane = 0; plane < img.planes; plane++) {
        memset(img.planes[plane], 0, img.stride[plane] * img.h);
    }

    // Encode the frame (5 arguments: ctx, img, pts, duration, flags)
    aom_codec_encode(&codec, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder(&codec);

    return 0;
}