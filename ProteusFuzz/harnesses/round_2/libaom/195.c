// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

// Helper to safely free aom_fixed_buf_t if it exists
static void safe_free_fixed_buf(aom_fixed_buf_t *buf) {
    if (buf != NULL) {
        if (buf->buf != NULL) {
            free(buf->buf);
        }
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to set some configuration parameters safely
    // Avoid setting width/height to 0 to prevent immediate failures
    if (Size >= 2) {
        uint16_t w = (Data[0] % 512) + 16;
        uint16_t h = (Data[1] % 512) + 16;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Invoke target functions diversely

    // Function 1: AV1E_SET_ERROR_RESILIENT_MODE
    int error_resilient_mode = (Size > 2) ? (Data[2] % 2) : 1;
    ret = aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    (void)ret; // Ignore return for fuzzing state exploration

    // Function 2: AV1E_SET_PARTITION_INFO_PATH
    // This typically requires a path string. We use a dummy file as per instructions.
    const char *partition_path = "./dummy_file";
    ret = aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, partition_path);
    (void)ret;

    // Function 3: AV1E_SET_ENABLE_DNL_DENOISING
    int enable_dnl = (Size > 3) ? (Data[3] % 2) : 1;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_dnl);
    (void)ret;

    // Function 5: AV1E_SET_NUM_TG
    // Number of temporal groups, usually small integer
    int num_tg = (Size > 4) ? (Data[4] % 4) : 1;
    ret = aom_codec_control(&ctx, AV1E_SET_NUM_TG, num_tg);
    (void)ret;

    // Function 4: aom_codec_error
    // Test with valid context
    const char *err_str = aom_codec_error(&ctx);
    (void)err_str; // Just consume the result

    // Test with null context (safe by design)
    const char *err_str_null = aom_codec_error(NULL);
    (void)err_str_null;

    // Function 6: aom_codec_get_global_headers
    // Retrieve global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers != NULL) {
        // Use the data to ensure it's valid (optional, just to touch memory)
        if (global_headers->buf != NULL && global_headers->sz > 0) {
            volatile uint8_t *p = global_headers->buf;
            (void)p[0];
        }
        safe_free_fixed_buf(global_headers);
    }

    // 3. Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}