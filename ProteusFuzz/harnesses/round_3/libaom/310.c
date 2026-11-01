// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    
    aom_codec_err_t ret;
    int val = 0;
    
    // Helper to extract int from data
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // 1. AV1E_SET_ENABLE_CFL_INTRA (typically expects int)
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    (void)ret;

    // 2. AOME_GET_LOOPFILTER_LEVEL (typically returns int via pointer, or expects pointer)
    int lf_level = 0;
    if (size >= sizeof(int)) {
        memcpy(&lf_level, data, sizeof(int));
    }
    ret = aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &lf_level);
    (void)ret;

    // 3. AV1E_SET_DELTALF_MODE (typically expects int)
    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    (void)ret;

    // 4. AV1E_SET_LOOPFILTER_CONTROL (might expect struct or multiple ints, simplified here)
    ret = aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
    (void)ret;

    // 5. AV1E_SET_ENABLE_INTRABC (typically expects int)
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    // 6. AV1_SET_SKIP_LOOP_FILTER (typically expects int)
    ret = aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, val);
    (void)ret;

    return 0;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    
    aom_codec_err_t ret;
    int val = 0;
    
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // Try decoder-specific variations if applicable, though the list is mostly encoder.
    // Reusing the same control requests to see if decoder handles them gracefully (should error).
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    (void)ret;
    
    int lf_level = 0;
    if (size >= sizeof(int)) {
        memcpy(&lf_level, data, sizeof(int));
    }
    ret = aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &lf_level);
    (void)ret;

    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    (void)ret;

    ret = aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
    (void)ret;

    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    ret = aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_310(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize Encoder
    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set minimal config to ensure init succeeds
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_bit_depth = 8;
    cfg.rc_target_bitrate = 100;
    cfg.rc_end_usage = AOM_CBR;
    
    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz encoder controls
    fuzz_encoder_control(&enc_ctx, Data, Size);

    // Cleanup Encoder
    aom_codec_destroy(&enc_ctx);

    // Initialize Decoder
    aom_codec_ctx_t dec_ctx;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    // Removed 'allowed_show_frame_delay' as it is not present in all struct versions
    
    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz decoder controls
    fuzz_decoder_control(&dec_ctx, Data, Size);

    // Cleanup Decoder
    aom_codec_destroy(&dec_ctx);

    return 0;
}