// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/* 
 * Fuzz driver for libaom targeting specific control functions.
 * Corrected API usage based on compiler feedback.
 */

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void fuzz_control(aom_codec_ctx_t *ctx, int control_id, void *arg) {
    if (!ctx) return;
    
    // Invoke the control. 
    // Using the correct constant names from aom.h and aomdx.h
    aom_codec_control(ctx, control_id, arg);
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize decoder
    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    // Prepare dummy image for operations requiring image data
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = 64;
    img.h = 64;
    img.fmt = AOM_IMG_FMT_I420;
    img.bit_depth = 8;
    img.planes[0] = malloc(64 * 64);
    img.planes[1] = malloc(32 * 32);
    img.planes[2] = malloc(32 * 32);
    img.stride[0] = 64;
    img.stride[1] = 32;
    img.stride[2] = 32;
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        cleanup_decoder(ctx);
        return 0;
    }
    memset(img.planes[0], 0, 64*64);
    memset(img.planes[1], 0, 32*32);
    memset(img.planes[2], 0, 32*32);

    // Decode a frame to ensure state is valid for control calls
    aom_codec_decode(ctx, Data, Size, NULL);
    
    // Get packets to trigger internal processing
    // Corrected: aom_codec_get_frame takes 2 arguments and returns aom_image_t*
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame = NULL;
    
    while ((frame = aom_codec_get_frame(ctx, &iter)) != NULL) {
        // Frame decoded, state is ready for control queries
        // We don't need to check pkt.kind here as we got a frame
        break;
    }

    // Define control IDs based on standard libaom constants
    // Corrected constant names:
    // AV1D_GET_FRAME_HEADER_INFO (from aomdx.h)
    // AV1_COPY_NEW_FRAME_IMAGE (from aom.h)
    // AOMD_GET_S_FRAME_INFO (from aomdx.h)
    // AOMD_GET_SHOW_FRAME_FLAG (from aomdx.h)
    // AV1_GET_NEW_FRAME_IMAGE (from aom.h)
    // AOMD_GET_FRAME_FLAGS (from aomdx.h)
    
    // 1. AV1D_GET_FRAME_HEADER_INFO
    fuzz_control(ctx, AV1D_GET_FRAME_HEADER_INFO, NULL);

    // 2. AV1_COPY_NEW_FRAME_IMAGE
    fuzz_control(ctx, AV1_COPY_NEW_FRAME_IMAGE, &img);

    // 3. AOMD_GET_S_FRAME_INFO
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    fuzz_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

    // 4. AOMD_GET_SHOW_FRAME_FLAG
    int show_frame_flag = 0;
    fuzz_control(ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);

    // 5. AV1_GET_NEW_FRAME_IMAGE
    aom_image_t *new_img = NULL;
    fuzz_control(ctx, AV1_GET_NEW_FRAME_IMAGE, &new_img);
    // Note: new_img points to internal memory, do not free

    // 6. AOMD_GET_FRAME_FLAGS
    uint32_t frame_flags = 0;
    fuzz_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);

    // Cleanup
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    cleanup_decoder(ctx);

    return 0;
}