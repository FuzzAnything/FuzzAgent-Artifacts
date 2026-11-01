// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzzer_test_encoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int result = 0;

    // Initialize configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to fuzz configuration parameters
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = (Data[1 % Size] % 1024) + 16;
        cfg.g_timebase.num = (Data[2 % Size] % 100) + 1;
        cfg.g_timebase.den = (Data[3 % Size] % 100) + 1;
        cfg.rc_target_bitrate = (Data[6 % Size] * 256 + Data[7 % Size]) * 1000;
        cfg.g_error_resilient = (Data[8 % Size] % 2);
        cfg.g_lag_in_frames = (Data[9 % Size] % 10);
        cfg.g_threads = (Data[10 % Size] % 8) + 1;
        // Use AOM_CBR, AOM_VBR, AOM_Q directly instead of aom_rc_mode_t cast if enum is not exposed
        cfg.rc_end_usage = (int)(Data[11 % Size] % 3); 
    }

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    if (Size > 12) {
        int screen_content_mode = (Data[12 % Size] % 2);
        ret = aom_codec_control(&codec, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, screen_content_mode);
        if (ret != AOM_CODEC_OK) {
            result = 1;
        }
    }

    // Fuzz AV1E_SET_SVC_REF_FRAME_COMP_PRED
    if (Size > 15) {
        aom_svc_ref_frame_comp_pred_t comp_pred;
        comp_pred.use_comp_pred[0] = (Data[13 % Size] % 2);
        comp_pred.use_comp_pred[1] = (Data[14 % Size] % 2);
        comp_pred.use_comp_pred[2] = (Data[15 % Size] % 2);
        ret = aom_codec_control(&codec, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &comp_pred);
        if (ret != AOM_CODEC_OK) {
            result = 1;
        }
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Note: aom_film_grain_params_t might not be available in all versions or headers.
    // Using a void pointer approach if the specific struct is not defined in the provided context.
    // However, based on the error, the type is missing. We will skip this specific control
    // if the type is not available, or try to use a generic approach if possible.
    // Since the error indicates the type is undeclared, we will omit this specific block 
    // or use a dummy value if the API allows, but strictly following the error, we skip it.
    // Re-evaluating: The user provided data types did not include aom_film_grain_params_t.
    // We will skip this specific control to ensure compilation, or assume it's not available.
    // To be safe and compile, we remove the block relying on the missing type.
    
    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    if (Size > 19) {
        int enable_angle_delta = (Data[19 % Size] % 2);
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_ANGLE_DELTA, enable_angle_delta);
        if (ret != AOM_CODEC_OK) {
            result = 1;
        }
    }

    // Fuzz AOME_SET_TUNING
    if (Size > 20) {
        int tuning = (Data[20 % Size] % 3); // 0: PSNR, 1: SSIM, 2: CUSTOM
        ret = aom_codec_control(&codec, AOME_SET_TUNING, tuning);
        if (ret != AOM_CODEC_OK) {
            result = 1;
        }
    }

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    if (Size > 21) {
        int enable_1to4_partitions = (Data[21 % Size] % 2);
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_1TO4_PARTITIONS, enable_1to4_partitions);
        if (ret != AOM_CODEC_OK) {
            result = 1;
        }
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return result;
}

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Run the encoder control fuzzing
    fuzzer_test_encoder_control(Data, Size);

    return 0;
}