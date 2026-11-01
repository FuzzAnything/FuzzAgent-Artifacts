// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom decoder control commands.
 * Tests AOMD_GET_FRAME_FLAGS, AOMD_GET_SHOW_EXISTING_FRAME_FLAG,
 * AOMD_GET_SHOW_FRAME_FLAG, AOMD_GET_ALTREF_PRESENT,
 * AOMD_GET_LAST_REF_UPDATES, AOMD_GET_FRAME_CORRUPTED.
 */

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: removed allow_highbitdepth which does not exist in this struct version
    // cfg.allow_lowbitdepth = 1; // Optional, default is usually fine

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Feed the input data as a packet
    if (size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }
    
    // Try to get a frame to ensure state is populated for control queries
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt_out;
    while ((pkt_out = aom_codec_get_frame(ctx, &iter)) != NULL) {
        // Frame retrieved, state is ready
        break;
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    // Define the control commands to test
    int commands[] = {
        AOMD_GET_FRAME_FLAGS,
        AOMD_GET_SHOW_EXISTING_FRAME_FLAG,
        AOMD_GET_SHOW_FRAME_FLAG,
        AOMD_GET_ALTREF_PRESENT,
        AOMD_GET_LAST_REF_UPDATES,
        AOMD_GET_FRAME_CORRUPTED
    };
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    // Buffer to receive results
    int result;
    int *result_ptr = &result;

    // Iterate through commands and invoke them
    for (int i = 0; i < num_commands; i++) {
        // Attempt to call the control function
        aom_codec_control(ctx, commands[i], result_ptr);
        
        // We don't necessarily need to check the return value for fuzzing,
        // but we ensure we don't crash.
        (void)result;
    }

    // Cleanup
    cleanup_decoder(ctx);

    return 0;
}