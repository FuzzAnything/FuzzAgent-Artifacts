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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;

    int32_t value;
    if (size >= sizeof(int32_t)) {
        memcpy(&value, data, sizeof(int32_t));
    } else {
        value = 0;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, value);
    
    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, value);
    
    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    
    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, value);
    
    // Fuzz AV1E_SET_ENABLE_SUPERRES
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, value);
    
    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int32_t seq_level_idx;
    aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);

    return 1;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to configure encoder settings
    if (Size > 0) cfg.g_w = Data[0] * 16;
    if (Size > 1) cfg.g_h = Data[1] * 16;
    if (Size > 2) cfg.g_timebase.num = Data[2] + 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] + 1;
    if (Size > 4) cfg.g_threads = Data[4] % 8 + 1;
    if (Size > 5) cfg.g_error_resilient = Data[5] % 2;
    if (Size > 6) cfg.rc_target_bitrate = (Data[6] + 1) * 1000;
    if (Size > 7) cfg.rc_end_usage = Data[7] % 2 ? AOM_CBR : AOM_VBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_control(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}