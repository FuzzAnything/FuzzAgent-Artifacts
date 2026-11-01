// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg, size_t size) {
    if (!ctx) return 0;

    switch (control_id) {
        case AV1E_SET_ENABLE_INTRABC: {
            int val = 0;
            if (size >= sizeof(int)) {
                memcpy(&val, arg, sizeof(int));
            }
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
            break;
        }
        case AV1E_SET_DENOISE_NOISE_LEVEL: {
            int val = 0;
            if (size >= sizeof(int)) {
                memcpy(&val, arg, sizeof(int));
            }
            aom_codec_control(ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val);
            break;
        }
        case AV1E_SET_ENABLE_PAETH_INTRA: {
            int val = 0;
            if (size >= sizeof(int)) {
                memcpy(&val, arg, sizeof(int));
            }
            aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
            break;
        }
        case AV1E_SET_ENABLE_AB_PARTITIONS: {
            int val = 0;
            if (size >= sizeof(int)) {
                memcpy(&val, arg, sizeof(int));
            }
            aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
            break;
        }
        case AV1E_SET_SVC_PARAMS: {
            if (size < sizeof(aom_svc_params_t)) break;
            aom_svc_params_t params;
            memcpy(&params, arg, sizeof(aom_svc_params_t));
            // Clamp values to valid ranges to avoid immediate crashes, though fuzzing wants edge cases
            if (params.number_spatial_layers < 1) params.number_spatial_layers = 1;
            if (params.number_temporal_layers < 1) params.number_temporal_layers = 1;
            aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &params);
            break;
        }
        case AV1E_SET_ENABLE_DNL_DENOISING: {
            int val = 0;
            if (size >= sizeof(int)) {
                memcpy(&val, arg, sizeof(int));
            }
            aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
            break;
        }
        default:
            break;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for basic configuration
    if (Size < 4) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration based on input to vary states
    // Safe access with bounds checking
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 60) + 1;
    cfg.g_timebase.den = (Data[3] % 1000) + 1;
    
    // Use modulo to ensure valid thread count and error resilient flag
    cfg.g_threads = (Size > 4 ? Data[4] : 0) % 8 + 1;
    cfg.g_error_resilient = (Size > 5 ? Data[5] : 0) % 2;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and argument from the input buffer
    int control_id = 0;
    void *arg = NULL;
    size_t arg_size = 0;

    if (Size >= sizeof(int)) {
        memcpy(&control_id, Data, sizeof(int));
        arg = (void *)(Data + sizeof(int));
        arg_size = Size - sizeof(int);
    } else {
        // Fallback to default control if input is too small (should not happen due to Size < 4 check)
        control_id = AV1E_SET_ENABLE_INTRABC;
        arg = &Data[0];
        arg_size = Size;
    }

    // Map random integers to valid control IDs if needed, or rely on the switch case
    // The switch case inside fuzz_codec_control handles specific IDs.
    // We pass the raw integer, but the function only acts on known IDs.
    // To ensure we hit the targets, we can force specific IDs based on input modulo.
    int target_ids[] = {
        AV1E_SET_ENABLE_INTRABC,
        AV1E_SET_DENOISE_NOISE_LEVEL,
        AV1E_SET_ENABLE_PAETH_INTRA,
        AV1E_SET_ENABLE_AB_PARTITIONS,
        AV1E_SET_SVC_PARAMS,
        AV1E_SET_ENABLE_DNL_DENOISING
    };
    int num_targets = sizeof(target_ids) / sizeof(target_ids[0]);
    
    if (Size > 0) {
        int selected_id = target_ids[Data[0] % num_targets];
        fuzz_codec_control(&ctx, selected_id, arg, arg_size);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}