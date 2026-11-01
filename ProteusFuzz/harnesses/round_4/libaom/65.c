// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int test_decoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    int tile_cols = 0;

    if (size >= sizeof(int)) {
        memcpy(&tile_cols, data, sizeof(int));
        // Ensure tile_cols is within a reasonable range for testing
        if (tile_cols < 1) tile_cols = 1;
        if (tile_cols > 64) tile_cols = 64;
    }

    // Initialize decoder config with default values
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed invalid member arnr_maxframes

    res = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // AV1_SET_DECODE_TILE_COL expects an int pointer
    res = aom_codec_control(&ctx, AV1_SET_DECODE_TILE_COL, &tile_cols);
    
    aom_codec_destroy(&ctx);
    return 0;
}

static int test_encoder_control(const uint8_t *data, size_t size, int control_id) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int value = 0;
    int *value_ptr = &value;

    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
    }

    res = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Set some basic config to ensure encoder can initialize
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    // Removed invalid member g_row_mt

    res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Invoke the specific control
    res = aom_codec_control(&ctx, control_id, value_ptr);

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to select which API to fuzz
    int selector = Data[0] % 6;

    switch (selector) {
        case 0:
            // Fuzz AV1_SET_DECODE_TILE_COL
            test_decoder_control(Data + 1, Size - 1);
            break;
        case 1:
            // Fuzz AV1E_SET_RTC_EXTERNAL_RC
            test_encoder_control(Data + 1, Size - 1, AV1E_SET_RTC_EXTERNAL_RC);
            break;
        case 2:
            // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
            test_encoder_control(Data + 1, Size - 1, AV1E_SET_TARGET_SEQ_LEVEL_IDX);
            break;
        case 3:
            // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
            test_encoder_control(Data + 1, Size - 1, AV1E_SET_ENABLE_REF_FRAME_MVS);
            break;
        case 4:
            // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
            test_encoder_control(Data + 1, Size - 1, AV1E_SET_MAX_REFERENCE_FRAMES);
            break;
        case 5:
            // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
            test_encoder_control(Data + 1, Size - 1, AV1E_SET_ENABLE_INTERINTRA_WEDGE);
            break;
        default:
            break;
    }

    return 0;
}