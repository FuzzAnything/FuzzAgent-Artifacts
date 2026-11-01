// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_302(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CQ; 
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int val = (int)(Data[4] % 256);

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    aom_codec_control(&codec, AV1E_SET_INTRA_DCT_ONLY, val);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    aom_codec_control(&codec, AV1E_SET_ENABLE_RECT_TX, val);

    // Fuzz AV1E_SET_COLOR_RANGE
    int color_range = val % 2; 
    aom_codec_control(&codec, AV1E_SET_COLOR_RANGE, color_range);

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    int matrix = val % 10; 
    aom_codec_control(&codec, AV1E_SET_MATRIX_COEFFICIENTS, matrix);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_COMP, val);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}