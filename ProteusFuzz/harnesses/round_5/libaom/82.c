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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure the encoder
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    int64_t rtc_val = 0;
    if (Size > 12) rtc_val = *(const int64_t*)(Data + 4);
    aom_codec_control(&codec, AV1E_SET_RTC_EXTERNAL_RC, rtc_val);

    // Fuzz AV1E_SET_ENABLE_PALETTE
    int palette_val = (Size > 12) ? Data[12] : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_PALETTE, palette_val);

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    int level_idx = (Size > 13) ? Data[13] : 0;
    aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int64_t get_level_idx = 0;
    aom_codec_control(&codec, AV1E_GET_SEQ_LEVEL_IDX, &get_level_idx);

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    int filter_intra_val = (Size > 14) ? Data[14] : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra_val);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    int wedge_val = (Size > 15) ? Data[15] : 0;
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, wedge_val);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}