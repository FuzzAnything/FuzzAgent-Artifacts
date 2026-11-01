// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTRA at aomcx.h:2138:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTERINTRA at aomcx.h:2114:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_MASKED_COMP at aomcx.h:2105:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TUNE_CONTENT at aomcx.h:1972:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DUAL_FILTER at aomcx.h:2099:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER at aomcx.h:2075:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

// Helper function to initialize a test image
static int create_test_image(aom_image_t *img, int width, int height, const uint8_t *data, size_t size) {
    if (!img || width <= 0 || height <= 0) return 0;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        return 0;
    }
    
    // Fill with data if available, otherwise zeros
    if (data && size > 0) {
        size_t bytes_per_plane = (width * height) / 2; // Simplified for Y plane
        size_t copy_size = (size < bytes_per_plane) ? size : bytes_per_plane;
        memcpy(img->planes[0], data, copy_size);
        // Fill UV planes with zeros or pattern
        memset(img->planes[1], 0, bytes_per_plane);
        memset(img->planes[2], 0, bytes_per_plane);
    } else {
        memset(img->planes[0], 0, width * height);
        memset(img->planes[1], 0, (width * height) / 4);
        memset(img->planes[2], 0, (width * height) / 4);
    }
    
    return 1;
}

// Helper to cleanup image
static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_207(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz input
    int width = Data[0] % 256;
    int height = Data[1] % 256;
    
    // Ensure minimum dimensions
    if (width < 16) width = 16;
    if (height < 16) height = 16;
    
    // Cap dimensions to avoid excessive memory usage
    if (width > 1920) width = 1920;
    if (height > 1080) height = 1080;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Create test image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    if (!create_test_image(&img, width, height, Data + 2, Size - 2)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Encode the frame
    // aom_codec_encode signature: aom_codec_encode(ctx, img, pts, duration, flags)
    ret = aom_codec_encode(&codec, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_image(&img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // Try to invoke the target API functions
    // Note: These are type-checked control functions, they expect (ctx, ctrl_id, data)
    // The macro AOM_CTRL_USE_TYPE expands to: aom_codec_control_typechecked_##id(ctx, ctrl, data)
    // where 'ctrl' is the enum value (e.g., AV1E_SET_ENABLE_SMOOTH_INTRA)
    
    // 1. AV1E_SET_ENABLE_SMOOTH_INTRA (0 or 1)
    int val1 = (Size > 2) ? (Data[2] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTRA(&codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val1);
    (void)ret; // Ignore return for fuzzing purposes

    // 2. AV1E_SET_ENABLE_SMOOTH_INTERINTRA (0 or 1)
    int val2 = (Size > 3) ? (Data[3] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_SMOOTH_INTERINTRA(&codec, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val2);
    (void)ret;

    // 3. AV1E_SET_ENABLE_MASKED_COMP (0 or 1)
    int val3 = (Size > 4) ? (Data[4] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_MASKED_COMP(&codec, AV1E_SET_ENABLE_MASKED_COMP, val3);
    (void)ret;

    // 4. AV1E_SET_TUNE_CONTENT (0, 1, or 2)
    int val4 = (Size > 5) ? (Data[5] % 3) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_TUNE_CONTENT(&codec, AV1E_SET_TUNE_CONTENT, val4);
    (void)ret;

    // 5. AV1E_SET_ENABLE_DUAL_FILTER (0 or 1)
    int val5 = (Size > 6) ? (Data[6] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_DUAL_FILTER(&codec, AV1E_SET_ENABLE_DUAL_FILTER, val5);
    (void)ret;

    // 6. AV1E_SET_ENABLE_INTRA_EDGE_FILTER (0 or 1)
    int val6 = (Size > 7) ? (Data[7] % 2) : 0;
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(&codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val6);
    (void)ret;

    // Cleanup
    cleanup_image(&img);
    aom_codec_destroy(&codec);

    return 0;
}