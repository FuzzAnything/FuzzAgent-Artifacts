// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int try_init_encoder(aom_codec_ctx_t *codec, const aom_codec_iface_t *iface) {
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    // Set minimal valid config to ensure initialization works
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    
    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (!try_init_encoder(&codec, iface)) {
        return 0;
    }

    // Extract control values from fuzz input
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    aom_codec_control(&codec, AV1E_SET_INTRA_DCT_ONLY, val1);
    
    // Fuzz AV1E_SET_INTER_DCT_ONLY
    aom_codec_control(&codec, AV1E_SET_INTER_DCT_ONLY, val2);
    
    // Fuzz AV1E_SET_ENABLE_TX64
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX64, val3);
    
    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, val4);
    
    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(&codec, AV1E_SET_ENABLE_FLIP_IDTX, val5);
    
    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    aom_codec_control(&codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, val6);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}