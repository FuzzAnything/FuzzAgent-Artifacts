// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg, size_t arg_size) {
    if (!ctx) return 0;
    
    aom_codec_err_t err;
    
    // Pass the argument directly. The specific control expects specific types.
    // This helper is generic, but the caller must ensure 'arg' is correctly typed.
    err = aom_codec_control(ctx, control_id, arg);
    
    return 0;
}

int LLVMFuzzerTestOneInput_321(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzzed data where possible
    cfg.g_w = (Size > 0) ? (Data[0] % 256) + 16 : 128;
    cfg.g_h = (Size > 1) ? (Data[1] % 256) + 16 : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Control 1: AV1E_SET_ENABLE_FLIP_IDTX (expects int)
    if (Size > 0) {
        int val = Data[0] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, &val);
    }

    // Control 2: AV1E_SET_NOISE_SENSITIVITY (expects int)
    if (Size > 1) {
        int val = (int)Data[1];
        aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, &val);
    }

    // Control 3: AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR (expects int)
    if (Size > 2) {
        int val = (int)Data[2];
        aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, &val);
    }

    // Control 4: AV1E_SET_ENABLE_INTRABC (expects int)
    if (Size > 3) {
        int val = Data[3] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, &val);
    }

    // Control 5: AV1E_SET_RATE_DISTRIBUTION_INFO
    // The exact struct type is internal to libaom (aom_codec_rate_distribution_info_t)
    // and not always exposed in public headers. We skip the sizeof check and pass
    // the raw buffer if it's large enough to be potentially valid, relying on the
    // codec's internal validation.
    if (Size >= 64) {
        aom_codec_control(&ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, (void*)Data);
    }

    // Control 6: AV1E_SET_AUTO_INTRA_TOOLS_OFF (expects int)
    if (Size > 5) {
        int val = Data[5] & 1;
        aom_codec_control(&ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, &val);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}