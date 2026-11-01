// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int test_codec_control(aom_codec_ctx_t *codec, int control_id, int value) {
    if (!codec) return 0;
    
    // Attempt to set the control
    aom_codec_err_t res = aom_codec_control(codec, control_id, value);
    if (res != AOM_CODEC_OK) {
        // Ignore errors as they are expected during fuzzing
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Ensure minimum size to avoid buffer overflows
    if (Size < 10) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure basic parameters with bounds checking
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

    // Initialize encoder
    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Define valid control IDs available in libaom
    int control_ids[] = {
        AV1E_SET_ENABLE_OBMC,
        AV1E_SET_INTER_DCT_ONLY,
        AV1E_SET_MV_COST_UPD_FREQ,
        AV1E_SET_REDUCED_TX_TYPE_SET,
        // Fallback to a known valid control if specific test controls are missing
        AV1E_SET_ENABLE_RECT_TX 
    };
    
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);
    
    // Ensure we have enough data for the loop iterations
    size_t max_index = num_controls + 10;
    if (Size < max_index) {
        aom_codec_destroy(&codec);
        return 0;
    }

    for (int i = 0; i < num_controls; i++) {
        int value = (int)Data[i + 4];
        test_codec_control(&codec, control_ids[i], value);
    }

    // Try with different value ranges
    for (int i = 0; i < num_controls; i++) {
        int value = (int)Data[i + 10];
        // Test with boolean values
        test_codec_control(&codec, control_ids[i], value ? 1 : 0);
        // Test with small integers
        test_codec_control(&codec, control_ids[i], value % 10);
    }

    // Cleanup
    aom_codec_destroy(&codec);
    
    return 0;
}