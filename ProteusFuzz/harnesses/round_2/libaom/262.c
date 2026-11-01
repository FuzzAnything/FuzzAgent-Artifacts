// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_DENOISE_BLOCK_SIZE
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_ENABLE_RESTORATION
 * - AV1E_ENABLE_RATE_GUIDE_DELTAQ
 * - AV1E_SET_SKIP_POSTPROC_FILTERING
 * - AV1E_SET_DENOISE_NOISE_LEVEL
 *
 * Note: The user provided signatures were empty. This driver uses the standard
 * libaom control IDs defined in aom/aom_encoder.h to invoke the underlying
 * functionality.
 */

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static int invoke_control(aom_codec_ctx_t *codec, int control_id, int value) {
    return aom_codec_control(codec, control_id, value);
}

int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder
    if (init_encoder(&codec, &cfg) != 0) {
        return 0;
    }

    // Use input data to determine control IDs and values
    // We map the first byte to a control ID index (0-5)
    int control_index = Data[0] % 6;
    int value = (Size > 1) ? (Data[1] << 8 | (Size > 2 ? Data[2] : 0)) : 0;
    
    // Normalize value to reasonable ranges for specific controls if needed
    // For fuzzing, we often want to test edge cases, so we might keep raw values
    // but ensure they don't cause immediate crashes due to overflow if the API expects small ints.
    // The API usually takes 'int', so we can pass the derived value directly.
    
    int control_id = 0;
    
    switch (control_index) {
        case 0: // AV1E_SET_DENOISE_BLOCK_SIZE
            control_id = AV1E_SET_DENOISE_BLOCK_SIZE;
            break;
        case 1: // AV1E_SET_NOISE_SENSITIVITY
            control_id = AV1E_SET_NOISE_SENSITIVITY;
            break;
        case 2: // AV1E_SET_ENABLE_RESTORATION
            control_id = AV1E_SET_ENABLE_RESTORATION;
            break;
        case 3: // AV1E_ENABLE_RATE_GUIDE_DELTAQ
            control_id = AV1E_ENABLE_RATE_GUIDE_DELTAQ;
            break;
        case 4: // AV1E_SET_SKIP_POSTPROC_FILTERING
            control_id = AV1E_SET_SKIP_POSTPROC_FILTERING;
            break;
        case 5: // AV1E_SET_DENOISE_NOISE_LEVEL
            control_id = AV1E_SET_DENOISE_NOISE_LEVEL;
            break;
        default:
            control_id = AV1E_SET_DENOISE_BLOCK_SIZE;
            break;
    }

    // Invoke the control function
    // We ignore the return value for fuzzing stability, as invalid values might return errors
    invoke_control(&codec, control_id, value);

    // Cleanup
    cleanup_encoder(&codec);

    return 0;
}