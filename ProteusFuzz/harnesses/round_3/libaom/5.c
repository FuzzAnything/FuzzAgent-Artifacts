// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

static void safe_free_image(aom_image_t *img) {
    if (img) {
        aom_img_remove_metadata(img);
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Use input data to derive parameters safely
    unsigned int width = (Data[0] % 256) + 1;
    unsigned int height = (Data[1] % 256) + 1;
    unsigned int align = (Data[2] % 64) + 1; // Keep alignment small and valid
    aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[3] % 10); // Limit to known formats roughly

    // Ensure dimensions are within safe limits for aom_img_alloc
    if (width > 0x08000000 || height > 0x08000000 || align > 65536) {
        return 0;
    }

    // 1. Test aom_img_alloc
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        // Allocation failed, try with a pre-allocated struct
        aom_image_t local_img;
        memset(&local_img, 0, sizeof(local_img));
        img = aom_img_alloc(&local_img, fmt, width, height, align);
        if (!img) return 0;
        
        // If img points to local_img, we must not free the struct itself via aom_img_free
        // if self_allocd is 0. However, aom_img_free handles self_allocd.
        // To be safe for fuzzing, we'll rely on aom_img_free logic.
        // But if we passed a stack variable, aom_img_free might not free the struct itself.
        // Let's stick to heap allocation for simplicity in cleanup.
        if (img != &local_img) {
             safe_free_image(img);
        } else {
             aom_img_remove_metadata(img);
             // Do not call aom_img_free on stack variable if self_allocd is 0
             // aom_img_alloc sets self_allocd to 0 if img is provided.
             // So we just clean metadata.
        }
        return 0;
    }

    // 2. Test aom_img_num_metadata
    size_t num_meta = aom_img_num_metadata(img);
    (void)num_meta; // Use the value to avoid unused warning, logic is side-effect free

    // 3. Test aom_img_get_metadata with various indices
    // Try index 0, num_meta, num_meta + 1, and a random index derived from data
    for (int i = 0; i < 5; i++) {
        size_t idx = 0;
        if (Size > (size_t)(i + 4)) {
            idx = (size_t)Data[i + 4];
        }
        const aom_metadata_t *meta = aom_img_get_metadata(img, idx);
        if (meta) {
            // If metadata exists, we could inspect it, but for fuzzing just accessing is enough
            (void)meta;
        }
    }

    // 4. Test aom_codec_get_frame (requires a valid codec context)
    // We need to initialize a decoder context to test this properly.
    // We'll try to initialize a dummy decoder.
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.allow_lowbitdepth = 1; // Fixed field name

        if (aom_codec_dec_init(&ctx, iface, &cfg, 0) == AOM_CODEC_OK) {
            aom_codec_iter_t iter = NULL;
            
            // Try to get frame without decoding anything (should return NULL)
            aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
            if (frame) {
                // If we got a frame (unlikely without input), clean it up
                // Note: Frames returned by aom_codec_get_frame are owned by the codec
                // until the next decode or close. We should not free them directly.
                // But we can check metadata on them.
                size_t f_num = aom_img_num_metadata(frame);
                (void)f_num;
                const aom_metadata_t *f_meta = aom_img_get_metadata(frame, 0);
                (void)f_meta;
            }

            // Feed some dummy data to decoder to see if we can get frames
            // We use the input data as a potential bitstream
            aom_codec_cx_pkt_t pkt;
            memset(&pkt, 0, sizeof(pkt));
            pkt.kind = AOM_CODEC_CX_FRAME_PKT;
            // Fix: pkt.data.frame is a pointer to aom_image_t. 
            // We must initialize the union member correctly.
            // Since pkt is memset to 0, the pointer is already NULL. 
            // Explicit assignment was causing type mismatch in some strict builds if not cast.
            // We rely on memset initialization.
            
            // Try decoding the fuzz input
            aom_codec_err_t res = aom_codec_decode(&ctx, Data, Size, &pkt);
            
            if (res == AOM_CODEC_OK) {
                iter = NULL;
                while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
                    // Test metadata functions on the decoded frame
                    size_t d_num = aom_img_num_metadata(frame);
                    for (size_t k = 0; k <= d_num + 1; k++) {
                        const aom_metadata_t *d_meta = aom_img_get_metadata(frame, k);
                        (void)d_meta;
                    }
                    // Do NOT free frame here, codec owns it.
                }
            }

            aom_codec_destroy(&ctx);
        }
    }

    // 5. Test aom_img_remove_metadata
    // Add some dummy metadata to test removal? 
    // aom_img_add_metadata is not in the target list, but we can test removal on existing (likely null) metadata
    aom_img_remove_metadata(img);

    // 6. Cleanup
    safe_free_image(img);

    return 0;
}