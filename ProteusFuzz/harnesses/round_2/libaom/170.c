// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS at aomdx.h:564:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_SHOW_EXISTING_FRAME_FLAG at aomdx.h:583:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG at aomdx.h:589:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT at aomdx.h:567:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES at aomdx.h:483:1 in aomdx.h
// aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED at aomdx.h:486:1 in aomdx.h
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

// Helper to safely initialize a decoder context
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Feed some data to the decoder to ensure it's in a valid state for queries
    if (size > 0) {
        // Decode one frame to potentially set up internal state
        aom_codec_decode(ctx, data, size, NULL);
    }

    return ctx;
}

// Helper to cleanup
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

// Helper to write dummy file if needed
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write dummy file as per instructions
    write_dummy_file(Data, Size);

    // Initialize decoder context
    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    int arg = 0;
    aom_codec_err_t result;

    // Fuzz the target functions using the correct signatures defined in headers
    // The functions are static inline in the headers, so we call them directly.
    // Signature: aom_codec_control_typechecked_XXX(ctx, ctrl, data)
    
    // 1. AOMD_GET_FRAME_FLAGS
    result = aom_codec_control_typechecked_AOMD_GET_FRAME_FLAGS(ctx, AOMD_GET_FRAME_FLAGS, &arg);
    (void)result;

    // 2. AOMD_GET_SHOW_EXISTING_FRAME_FLAG
    result = aom_codec_control_typechecked_AOMD_GET_SHOW_EXISTING_FRAME_FLAG(ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &arg);
    (void)result;

    // 3. AOMD_GET_SHOW_FRAME_FLAG
    result = aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG(ctx, AOMD_GET_SHOW_FRAME_FLAG, &arg);
    (void)result;

    // 4. AOMD_GET_ALTREF_PRESENT
    result = aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT(ctx, AOMD_GET_ALTREF_PRESENT, &arg);
    (void)result;

    // 5. AOMD_GET_LAST_REF_UPDATES
    result = aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES(ctx, AOMD_GET_LAST_REF_UPDATES, &arg);
    (void)result;

    // 6. AOMD_GET_FRAME_CORRUPTED
    result = aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED(ctx, AOMD_GET_FRAME_CORRUPTED, &arg);
    (void)result;

    // Cleanup
    cleanup_decoder(ctx);

    return 0;
}