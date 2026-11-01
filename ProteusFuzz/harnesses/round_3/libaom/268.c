// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Set some basic configuration from fuzz data
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Extract control ID from fuzz input (0-5 mapping to 6 target functions)
    int control_id = (int)(Data[4] % 6);
    
    int actual_control;
    void *control_data = NULL;
    int data_val = 0;
    aom_img_fmt_t render_size_fmt = AOM_IMG_FMT_NONE;
    int render_w = 0;
    int render_h = 0;

    switch (control_id) {
        case 0: // AV1E_SET_DENOISE_NOISE_LEVEL
            actual_control = AV1E_SET_DENOISE_NOISE_LEVEL;
            if (Size > 5) data_val = Data[5];
            control_data = &data_val;
            break;
        case 1: // AV1E_SET_ENABLE_DNL_DENOISING
            actual_control = AV1E_SET_ENABLE_DNL_DENOISING;
            if (Size > 5) data_val = Data[5] % 2;
            control_data = &data_val;
            break;
        case 2: // AV1E_SET_ENABLE_ORDER_HINT
            actual_control = AV1E_SET_ENABLE_ORDER_HINT;
            if (Size > 5) data_val = Data[5] % 2;
            control_data = &data_val;
            break;
        case 3: // AV1E_SET_RENDER_SIZE
            actual_control = AV1E_SET_RENDER_SIZE;
            if (Size > 6) {
                render_w = (Data[5] % 1024) + 16;
                render_h = (Data[6] % 1024) + 16;
            } else {
                render_w = 16;
                render_h = 16;
            }
            // AV1E_SET_RENDER_SIZE expects an aom_render_size_t struct or similar
            // Since we don't have the exact struct definition, we use a union or raw bytes
            // In libaom, this usually takes a struct with w and h. We'll cast a local struct.
            {
                struct { int w; int h; } rs = {render_w, render_h};
                control_data = &rs;
            }
            break;
        case 4: // AV1E_GET_SEQ_LEVEL_IDX
            actual_control = AV1E_GET_SEQ_LEVEL_IDX;
            // GET operations typically return data via the pointer, 
            // but for fuzzing we can pass a dummy int to store the result
            if (Size > 5) data_val = 0;
            control_data = &data_val;
            break;
        case 5: // AV1E_SET_REDUCED_TX_TYPE_SET
            actual_control = AV1E_SET_REDUCED_TX_TYPE_SET;
            if (Size > 5) data_val = Data[5];
            control_data = &data_val;
            break;
        default:
            actual_control = AV1E_SET_DENOISE_NOISE_LEVEL;
            control_data = &data_val;
            break;
    }

    // Invoke the control function
    // Note: The return value is ignored as per fuzzing best practices to avoid early exits on errors
    aom_codec_control(&ctx, actual_control, control_data);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}