// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Uses aom_codec_control() with the corresponding AV1E_* control IDs.
 */

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    const aom_codec_iface_t *iface;
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    int control_ids[] = {
        AV1E_SET_ENABLE_INTERINTRA_WEDGE,
        AV1E_SET_ENABLE_SMOOTH_INTRA,
        AV1E_SET_ENABLE_DUAL_FILTER,
        AV1E_SET_TUNE_CONTENT,
        AV1E_SET_ENABLE_MASKED_COMP,
        AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);
    int i;

    // 1. Prepare environment
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure basic settings
    if (Size > 0) cfg.g_w = (Data[0] % 128) + 16; // Width
    if (Size > 1) cfg.g_h = (Data[1] % 128) + 16; // Height
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    
    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image frame to encode
    // Initialize pointers to NULL
    img.planes[0] = NULL;
    img.planes[1] = NULL;
    img.planes[2] = NULL;
    img.stride[0] = 0;
    img.stride[1] = 0;
    img.stride[2] = 0;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    // Note: 'format' member is not present in aom_image_t in this version.
    // The format is usually inferred from the encoder config or passed via flags.
    // We will rely on the encoder context configuration.
    
    // Allocate memory for the image planes
    img.planes[0] = malloc(cfg.g_w * cfg.g_h);
    img.planes[1] = malloc(((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2));
    img.planes[2] = malloc(((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2));
    
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Set strides
    img.stride[0] = cfg.g_w;
    img.stride[1] = (cfg.g_w + 1) / 2;
    img.stride[2] = (cfg.g_w + 1) / 2;

    // Fill image with fuzzing data
    memset(img.planes[0], 0, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 0, ((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2));
    memset(img.planes[2], 0, ((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2));
    
    if (Size > 0) {
        size_t copy_size = Size < (cfg.g_w * cfg.g_h) ? Size : cfg.g_w * cfg.g_h;
        memcpy(img.planes[0], Data, copy_size);
    }

    // 3. Explore program states by invoking control functions diversely
    for (i = 0; i < num_controls; i++) {
        int value;
        
        // Generate diverse values based on input data
        if (Size > (size_t)(i + 1)) {
            value = Data[i + 1];
        } else {
            value = i % 2; // Default to 0 or 1
        }

        // Invoke the control function
        ret = aom_codec_control(&ctx, control_ids[i], value);
        
        // Handle return values (ignore errors for fuzzing to explore paths)
        (void)ret;
    }

    // Encode a frame to ensure the controls took effect
    // Fixed: aom_codec_encode expects 5 arguments: ctx, img, pts, duration, flags
    // Using 0 for pts, 1 for duration, and 0 for flags (AOM_EFLAG_NONE)
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (ret == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process packet if needed
            }
            // The packet data is managed by the codec context.
            // We don't free the packet itself here as it's internal.
        }
    }

cleanup:
    // 2. Cleanup
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Call the fuzzing logic
    fuzz_encoder_controls(Data, Size);
    
    return 0;
}