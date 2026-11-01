// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Try setting the control with the provided value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    
    // If setting failed, try getting the value (for GET controls)
    if (ret != AOM_CODEC_OK) {
        int dummy_val;
        aom_codec_control(ctx, control_id, &dummy_val);
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid buffer overflows
    if (Size < 10) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    
    // Use fuzzer data to configure basic parameters safely
    cfg.g_w = (Data[0] % 100) + 16; // Width between 16 and 115
    cfg.g_h = (Data[1] % 100) + 16; // Height between 16 and 115
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_lag_in_frames = (Data[4] % 10);
    cfg.rc_target_bitrate = (Data[5] % 100) + 100;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Extract control values from input data
    int val1 = (Data[6] % 2); // 0 or 1
    int val2 = (Data[7] % 2);
    int val3 = (Data[8] % 2);
    int val4 = (Data[9] % 2);
    
    // Fuzz the target control functions
    // AV1E_SET_ENABLE_ONESIDED_COMP
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val1);
    
    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val2);
    
    // AV1E_SET_POSTENCODE_DROP_RTC
    fuzz_encoder_control(&ctx, AV1E_SET_POSTENCODE_DROP_RTC, val3);
    
    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val4);
    
    // AV1E_GET_BASELINE_GF_INTERVAL
    int gf_interval;
    aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval);
    
    // AV1E_GET_NUM_OPERATING_POINTS
    int num_op;
    aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_op);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}