// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int get_int(const uint8_t *d, size_t s, int default_val) {
    if (s < 4) return default_val;
    return *(int32_t*)d;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec) return 0;

    // 1. AV1E_SET_ENABLE_INTRABC
    if (size >= 4) {
        int val = get_int(data, size, 0);
        val = val ? 1 : 0;
        aom_codec_control(codec, AV1E_SET_ENABLE_INTRABC, val);
    }

    // 2. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (size >= 8) {
        int val = get_int(data + 4, size - 4, 0);
        val = val ? 1 : 0;
        aom_codec_control(codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    }

    // 3. AV1E_SET_CHROMA_SAMPLE_POSITION
    if (size >= 12) {
        int val = get_int(data + 8, size - 8, 0);
        if (val < 0) val = 0;
        if (val > 3) val = 3;
        aom_codec_control(codec, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
    }

    // 4. AV1E_SET_TRANSFER_CHARACTERISTICS
    if (size >= 16) {
        int val = get_int(data + 12, size - 12, 2);
        if (val < 0) val = 0;
        if (val > 255) val = 255;
        aom_codec_control(codec, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
    }

    // 5. AV1E_SET_TUNE_CONTENT
    if (size >= 20) {
        int val = get_int(data + 16, size - 16, 0);
        if (val < 0) val = 0;
        if (val > 2) val = 2;
        aom_codec_control(codec, AV1E_SET_TUNE_CONTENT, val);
    }

    // 6. AV1E_SET_ENABLE_TX64
    if (size >= 24) {
        int val = get_int(data + 20, size - 20, 0);
        val = val ? 1 : 0;
        aom_codec_control(codec, AV1E_SET_ENABLE_TX64, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    if (Size > 0) cfg.g_w = (Data[0] % 1280) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 1280) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Size > 4) ? (Data[4] % 8) + 1 : 1;
    cfg.rc_target_bitrate = (Size > 5) ? (Data[5] % 10000) + 100 : 1000;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&codec, Data, Size);

    aom_codec_destroy(&codec);

    return 0;
}