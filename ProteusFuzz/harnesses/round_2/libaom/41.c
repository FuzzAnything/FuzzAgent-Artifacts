// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static int try_fuzz_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->priv) return 0;

    // AOMD_GET_FRAME_CORRUPTED
    int corrupted;
    if (aom_codec_control(ctx, AOMD_GET_FRAME_CORRUPTED, &corrupted) == AOM_CODEC_OK) {
        (void)corrupted;
    }

    // AOMD_GET_ALTREF_PRESENT
    int altref_present;
    if (aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref_present) == AOM_CODEC_OK) {
        (void)altref_present;
    }

    // AOMD_GET_LAST_REF_UPDATES
    int last_ref_updates;
    if (aom_codec_control(ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates) == AOM_CODEC_OK) {
        (void)last_ref_updates;
    }

    // AOMD_GET_LAST_REF_USED
    int last_ref_used;
    if (aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used) == AOM_CODEC_OK) {
        (void)last_ref_used;
    }

    // AOMD_GET_LAST_QUANTIZER
    int last_quantizer;
    if (aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer) == AOM_CODEC_OK) {
        (void)last_quantizer;
    }

    // AV1D_SET_EXT_REF_PTR
    // We construct a dummy external reference frame structure if data is available
    if (size >= sizeof(av1_ext_ref_frame_t)) {
        av1_ext_ref_frame_t ext_ref;
        memcpy(&ext_ref, data, sizeof(ext_ref));
        
        // Ensure the image pointer is not NULL to avoid immediate crash, 
        // but pass the fuzzed structure to test internal handling
        if (ext_ref.img == NULL && size > 0) {
            // Create a minimal dummy image if needed
            if (size > 100) {
                aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
                if (img) {
                    ext_ref.img = img;
                    ext_ref.num = 1;
                    if (aom_codec_control(ctx, AV1D_SET_EXT_REF_PTR, &ext_ref) == AOM_CODEC_OK) {
                        aom_img_free(img);
                    }
                }
            }
        } else if (ext_ref.img != NULL) {
             if (aom_codec_control(ctx, AV1D_SET_EXT_REF_PTR, &ext_ref) == AOM_CODEC_OK) {
                 // If we didn't allocate it here, we shouldn't free it, 
                 // but in fuzzing we often allocate to ensure valid pointers.
             }
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    // Initialize decoder configuration
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;
    cfg.allow_lowbitdepth = 1;

    // Get the AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize the codec context
    aom_codec_err_t ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Decode a single frame to ensure the decoder has processed something
    // This is necessary for some control commands (like GET_LAST_QUANTIZER) to return valid data
    // We use aom_codec_decode instead of the non-existent aom_codec_dec_frame
    if (Size > 0) {
        aom_codec_decode(&ctx, (const uint8_t *)Data, Size, NULL);
    }
    
    // Try to get a frame to trigger internal state updates
    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Frame is automatically managed by the decoder, no need to free manually here
        // Break after one frame to keep fuzzing fast
        break;
    }

    // Execute the fuzzing logic for the target control functions
    try_fuzz_decoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}