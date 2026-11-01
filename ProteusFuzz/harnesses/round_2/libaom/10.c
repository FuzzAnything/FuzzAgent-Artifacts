// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom decoder control functions.
 */

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;
    aom_image_t *img;
    aom_codec_iter_t it = NULL;

    if (size == 0) {
        return NULL;
    }

    // Initialize default decoder config
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) {
        return NULL;
    }

    // Initialize the codec context
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return NULL;
    }

    // Feed the fuzz data to the decoder to generate frames
    res = aom_codec_decode(&ctx, data, size, NULL);
    // Note: We ignore the return value here as fuzzing often triggers errors.
    // We just need the context to be in a state where we can query it.

    // Attempt to get a frame to ensure state advancement
    while ((img = aom_codec_get_frame(&ctx, &it)) != NULL) {
        // In newer libaom versions, frames are managed automatically or via specific release
        // If aom_codec_frame_release exists, use it, otherwise just break to avoid crash
        // Since the error log indicated aom_codec_frame_release is undeclared in this env,
        // we skip explicit release if it's not available or assume auto-cleanup on destroy
        // for the fuzzing context.
        break; // Process one frame
    }

    // Allocate a heap copy of the context to return, 
    // as the function signature requires returning a pointer to a context
    // that the caller can destroy.
    aom_codec_ctx_t *ctx_heap = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx_heap) {
        aom_codec_destroy(&ctx);
        return NULL;
    }
    memcpy(ctx_heap, &ctx, sizeof(aom_codec_ctx_t));

    return ctx_heap;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize decoder with fuzz data
    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    
    if (!ctx) {
        return 0;
    }

    // 1. AOMD_GET_S_FRAME_INFO
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    #ifdef AOMD_GET_S_FRAME_INFO
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    #endif

    // 2. AOMD_GET_FRAME_FLAGS
    #ifdef AOMD_GET_FRAME_FLAGS
    int frame_flags = 0;
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    #endif

    // 3. AV1D_GET_FRAME_HEADER_INFO
    #ifdef AV1D_GET_FRAME_HEADER_INFO
    // We need a valid struct for this. If the struct type is not known or 
    // not defined in the included headers, we cannot safely declare it.
    // However, aomdx.h usually defines aom_av1_frame_header_info_t.
    // If it's missing, the preprocessor block won't execute the macro.
    // We assume the header defines the struct if the macro is defined.
    aom_av1_frame_header_info_t header_info;
    memset(&header_info, 0, sizeof(header_info));
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AV1D_GET_FRAME_HEADER_INFO, &header_info);
    #endif

    // 4. AOMD_GET_SHOW_FRAME_FLAG
    #ifdef AOMD_GET_SHOW_FRAME_FLAG
    int show_frame_flag = 0;
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);
    #endif

    // 5. AOMD_GET_LAST_REF_USED
    #ifdef AOMD_GET_LAST_REF_USED
    aom_ref_frame_info_t ref_info;
    memset(&ref_info, 0, sizeof(ref_info));
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOMD_GET_LAST_REF_USED, &ref_info);
    #endif

    // 6. Generic aom_codec_control with random IDs to explore error paths
    int random_id = (Size > 0) ? (Data[0] & 0xFF) : 1;
    if (random_id == 0) random_id = 1; // Must be non-zero

    int dummy_arg = 0;
    aom_codec_control(ctx, random_id, &dummy_arg);

    // Cleanup
    cleanup_decoder(ctx);

    return 0;
}