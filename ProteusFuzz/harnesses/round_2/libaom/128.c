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

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;

    // Use fuzzing data to configure encoder settings safely
    if (size > 0) cfg->g_w = (data[0] % 256) * 16;
    if (size > 1) cfg->g_h = (data[1] % 256) * 16;
    if (size > 2) cfg->g_timebase.num = data[2] + 1;
    if (size > 3) cfg->g_timebase.den = data[3] + 1;
    if (size > 4) cfg->g_threads = data[4] % 8 + 1;
    if (size > 5) cfg->rc_target_bitrate = (data[5] % 100) * 1000;

    res = aom_codec_enc_init(codec, iface, cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg, Data, Size) != 0) return 0;

    // Fuzz various control functions with derived values from input
    int val1 = (Size > 6) ? (Data[6] % 2) : 0;
    int val2 = (Size > 7) ? (Data[7] % 4) : 0;
    int val3 = (Size > 8) ? (Data[8] % 2) : 0;
    int val4 = (Size > 9) ? (Data[9] % 2) : 0;
    int val5 = (Size > 10) ? (Data[10] % 4) : 0;
    int val6 = (Size > 11) ? (Data[11] % 2) : 0;

    // AV1E_SET_REDUCED_REFERENCE_SET
    aom_codec_control(&codec, AV1E_SET_REDUCED_REFERENCE_SET, val1);

    // AV1E_SET_DELTAQ_MODE
    aom_codec_control(&codec, AV1E_SET_DELTAQ_MODE, val2);

    // AV1E_SET_ENABLE_QM
    aom_codec_control(&codec, AV1E_SET_ENABLE_QM, val3);

    // AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(&codec, AV1E_SET_FORCE_VIDEO_MODE, val4);

    // AV1E_SET_TILE_ROWS
    aom_codec_control(&codec, AV1E_SET_TILE_ROWS, val5);

    // AV1E_SET_FRAME_PARALLEL_DECODING
    aom_codec_control(&codec, AV1E_SET_FRAME_PARALLEL_DECODING, val6);

    // Optional: encode a dummy frame to exercise more code paths
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Manually fill the image planes with a constant value (128)
    // Corrected access: use img.planes array instead of plane_data
    for (int i = 0; i < img.planes; i++) {
        size_t plane_size = img.stride[i] * img.d_h;
        memset(img.planes[i], 128, plane_size);
    }

    aom_codec_ctx_t *ctx = &codec;
    aom_enc_frame_flags_t flags = 0;

    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_img_free(&img);

    cleanup_encoder(&codec);
    return 0;
}