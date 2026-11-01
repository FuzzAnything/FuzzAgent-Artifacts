// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS at aomcx.h:2318:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR at aomcx.h:2357:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR at aomcx.h:2177:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE at aomcx.h:2342:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR at aomcx.h:2345:1 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

/*
 * Helper function to initialize the AV1 encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to clean up the encoder context.
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

/*
 * Helper function to safely read an integer from the fuzzing data buffer.
 */
static int32_t read_int32(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) {
        return 0;
    }
    return (int32_t)(Data[offset] | (Data[offset + 1] << 8) | 
                     (Data[offset + 2] << 16) | (Data[offset + 3] << 24));
}

static uint32_t read_uint32(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) {
        return 0;
    }
    return (uint32_t)(Data[offset] | (Data[offset + 1] << 8) | 
                      (Data[offset + 2] << 16) | (Data[offset + 3] << 24));
}

static uint8_t read_uint8(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 1) {
        return 0;
    }
    return Data[offset];
}

/*
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_285(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    
    // Ensure minimal data size to avoid trivial crashes
    if (Size < 1) {
        return 0;
    }

    // Initialize configuration
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use fuzzing data to drive various control operations
    // We use the first byte to select which API to call
    int op = Data[0] % 6;

    switch (op) {
        case 0:
            // AV1E_GET_NUM_OPERATING_POINTS
            {
                int num_ops;
                ret = aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        case 1:
            // AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
            {
                int32_t val = read_int32(Data, Size, 1);
                ret = aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        case 2:
            // AV1E_SET_LOOPFILTER_CONTROL
            {
                // Read 2 bytes for this value
                if (Size < 3) {
                    cleanup_encoder(&ctx);
                    return 0;
                }
                int32_t val = (int32_t)(Data[1] | (Data[2] << 8));
                ret = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        case 3:
            // AV1E_SET_FILM_GRAIN_TEST_VECTOR
            {
                int32_t val = read_int32(Data, Size, 1);
                ret = aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        case 4:
            // AV1E_SET_SVC_FRAME_DROP_MODE
            {
                uint32_t val = read_uint8(Data, Size, 1);
                ret = aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        case 5:
            // AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
            {
                // Read 2 bytes for this value
                if (Size < 3) {
                    cleanup_encoder(&ctx);
                    return 0;
                }
                int32_t val = (int32_t)(Data[1] | (Data[2] << 8));
                ret = aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val);
                if (ret != AOM_CODEC_OK) {
                    // Ignore errors
                }
            }
            break;

        default:
            break;
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}