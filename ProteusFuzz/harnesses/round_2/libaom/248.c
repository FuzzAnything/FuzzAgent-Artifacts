// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t value = 0;
    if (size >= 4) {
        memcpy(&value, data, 4);
    }

    // AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, value);
    
    // AV1E_SET_ENABLE_DNL_DENOISING
    aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value);
    
    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
    
    // AV1E_SET_REDUCED_TX_TYPE_SET
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value);
    
    // AV1E_SET_ENABLE_PAETH_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, value);

    // AV1E_GET_SEQ_LEVEL_IDX (Get operation)
    int32_t level_idx = 0;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t value = 0;
    if (size >= 4) {
        memcpy(&value, data, 4);
    }

    // Attempting to set encoder-specific flags on a decoder context to test robustness
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, value);
    aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, value);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, value);
    aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, value);
    
    int32_t level_idx = 0;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

    return 1;
}

int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    // Determine if we are fuzzing encoder or decoder based on first byte
    int is_encoder = (Data[0] % 2 == 0);
    
    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        // Fuzz some config values with bounds checking
        cfg.g_w = (Data[1] % 2048) + 16;
        cfg.g_h = (Data[2] % 2048) + 16;
        cfg.g_threads = (Data[3] % 8) + 1;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = ((Data[4] << 8) | Data[5]) + 100;

        ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        fuzz_encoder_control(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        // Initialize decoder config manually
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = (Data[1] % 8) + 1;
        dec_cfg.allow_lowbitdepth = 1;

        ret = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        fuzz_decoder_control(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    }

    return 0;
}