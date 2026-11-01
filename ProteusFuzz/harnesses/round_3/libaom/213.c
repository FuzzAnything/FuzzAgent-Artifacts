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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get the AV1 encoder interface using the correct function name
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where possible
    if (size > 0) cfg.g_w = (data[0] % 64) * 16 + 16;
    if (size > 1) cfg.g_h = (data[1] % 64) * 16 + 16;
    if (size > 2) cfg.g_timebase.num = data[2] + 1;
    if (size > 3) cfg.g_timebase.den = data[3] + 1;
    if (size > 4) cfg.g_threads = (data[4] % 16) + 1;
    if (size > 5) cfg.rc_target_bitrate = (data[5] * 256 + 1000);
    if (size > 6) cfg.g_error_resilient = data[6] % 2;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    int enable_palette = 0;
    if (size > 7) enable_palette = data[7] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);

    // Fuzz AV1E_SET_RENDER_SIZE
    // FIX: AV1E_SET_RENDER_SIZE expects a pointer to an int array (width, height),
    // not two separate integer arguments.
    int render_size[2];
    render_size[0] = 0; // width
    render_size[1] = 0; // height
    if (size > 8) render_size[0] = (data[8] % 64) * 16 + 16;
    if (size > 9) render_size[1] = (data[9] % 64) * 16 + 16;
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_size);

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int level_idx = 0;
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    int external_rc = 0;
    if (size > 10) external_rc = data[10] % 2;
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, external_rc);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    int min_partition = 0;
    if (size > 11) min_partition = data[11] % 16;
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_partition);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    int enable_paeth = 0;
    if (size > 12) enable_paeth = data[12] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth);

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run encoder control fuzzing
    fuzz_encoder_controls(Data, Size);

    return 0;
}