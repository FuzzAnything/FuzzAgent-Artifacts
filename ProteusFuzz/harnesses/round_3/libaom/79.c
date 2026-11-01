// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_SET_ENABLE_INTRA_EDGE_FILTER
 * - AV1E_SET_ENABLE_SUPERRES
 * - AV1E_SET_MIN_PARTITION_SIZE
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_ENABLE_ORDER_HINT
 */

static aom_codec_ctx_t codec_ctx;
static aom_codec_enc_cfg_t cfg;

static int init_encoder(const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t err;

    if (size < 2) return 0;

    // Use first byte to determine width, second for height to vary states
    int width = data[0] % 256;
    int height = data[1] % 256;
    
    // Ensure minimum dimensions for valid frames
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_usage = AOM_CQ; // Use CQ mode

    err = aom_codec_enc_init(&codec_ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&codec_ctx);
        return 0;
    }

    return 1;
}

static void cleanup_encoder(void) {
    aom_codec_destroy(&codec_ctx);
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder with varying parameters based on input
    if (!init_encoder(Data, Size)) {
        return 0;
    }

    // Use input data to determine which control to invoke and what value to pass
    // We map the first byte to a specific control command
    int cmd = Data[0] % 6;
    int value;

    // Generate a value based on the second byte if available, else default
    if (Size > 1) {
        value = Data[1];
    } else {
        value = 0;
    }

    aom_codec_err_t err;

    switch (cmd) {
        case 0:
            // AV1E_SET_COEFF_COST_UPD_FREQ
            // Value: frequency (0-100 range usually safe for testing)
            err = aom_codec_control(&codec_ctx, AV1E_SET_COEFF_COST_UPD_FREQ, value % 100);
            break;
        case 1:
            // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
            // Value: 0 or 1
            err = aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value % 2);
            break;
        case 2:
            // AV1E_SET_ENABLE_SUPERRES
            // Value: 0 or 1
            err = aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_SUPERRES, value % 2);
            break;
        case 3:
            // AV1E_SET_MIN_PARTITION_SIZE
            // Value: partition size (e.g., 4, 8, 16, 32, 64)
            {
                int sizes[] = {4, 8, 16, 32, 64, 128};
                int size_idx = value % 6;
                err = aom_codec_control(&codec_ctx, AV1E_SET_MIN_PARTITION_SIZE, sizes[size_idx]);
            }
            break;
        case 4:
            // AV1E_SET_RTC_EXTERNAL_RC
            // Value: 0 or 1
            err = aom_codec_control(&codec_ctx, AV1E_SET_RTC_EXTERNAL_RC, value % 2);
            break;
        case 5:
            // AV1E_SET_ENABLE_ORDER_HINT
            // Value: 0 or 1
            err = aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_ORDER_HINT, value % 2);
            break;
        default:
            break;
    }

    // Handle potential errors gracefully (fuzzers should not crash on API errors)
    if (err != AOM_CODEC_OK) {
        // Just continue, we are testing robustness
    }

    // Attempt to encode a dummy frame to ensure state consistency
    // This helps catch crashes that might happen after setting invalid parameters
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    
    // Fill with dummy data
    memset(img.planes[0], 0x80, 16*16);
    memset(img.planes[1], 0x80, 8*8);
    memset(img.planes[2], 0x80, 8*8);

    aom_codec_frame_flags_t flags = 0;
    // Use correct constant name AOME_SET_CPUUSED
    aom_codec_control(&codec_ctx, AOME_SET_CPUUSED, 0);
    
    // Encode
    aom_codec_encode(&codec_ctx, &img, 0, 1, flags);
    
    // Cleanup frame
    aom_img_free(&img);

    cleanup_encoder();

    return 0;
}