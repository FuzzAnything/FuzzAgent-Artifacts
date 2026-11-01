// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, &value);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Require at least 4 bytes to safely access Data[0] to Data[3]
    if (Size < 4) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set config parameters with bounds checking
    cfg.g_w = (Data[0] % 4096) + 16;
    cfg.g_h = (Data[1] % 4096) + 16;
    
    // g_timebase is a struct aom_rational { int num; int den; }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;

    // Use rc_target_bitrate for bitrate instead of g_bitrate
    int bitrate = (Data[3] % 100) + 1000;
    cfg.rc_target_bitrate = bitrate;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Control IDs based on libaom AV1 encoder constants
    int controls[] = {
        AV1E_SET_MAX_PARTITION_SIZE,
        AV1E_SET_ENABLE_INTERINTER_WEDGE,
        AV1E_SET_MIN_PARTITION_SIZE,
        AV1E_SET_ENABLE_AB_PARTITIONS,
        AV1E_SET_ENABLE_CHROMA_DELTAQ,
        AV1E_SET_RTC_EXTERNAL_RC
    };
    int num_controls = sizeof(controls) / sizeof(controls[0]);

    // Iterate through controls and invoke them with fuzzed values
    for (int i = 0; i < num_controls; i++) {
        int value;
        // Ensure we have enough data before accessing
        if (Size > 0) {
            value = Data[i % Size] % 2; // 0 or 1 for flags
        } else {
            value = 0;
        }
        
        // For partition sizes, allow a wider range
        if (controls[i] == AV1E_SET_MAX_PARTITION_SIZE || 
            controls[i] == AV1E_SET_MIN_PARTITION_SIZE) {
            if (Size > 1) {
                value = (Data[i % Size] * 256 + Data[(i + 1) % Size]) % 64;
            }
        }

        try_control(&ctx, controls[i], value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}