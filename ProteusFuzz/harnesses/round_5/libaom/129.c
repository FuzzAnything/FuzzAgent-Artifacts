// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

// Global state for buffer management
static aom_image_t *g_img_list[10];
static int g_buffer_count = 0;
static int g_buffer_index = 0;

// Helper to allocate a raw image
static void init_buffer(int idx) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 128, 128, 1);
    if (img != NULL) {
        g_img_list[idx] = img;
        g_buffer_count++;
    }
}

// Callback: Get a frame buffer
// Signature: int (*)(void *cb_priv, unsigned long flags, aom_codec_frame_buffer_t *buf)
static int get_frame_buffer_cb(void *cb_priv, unsigned long flags, aom_codec_frame_buffer_t *buf) {
    (void)cb_priv;
    (void)flags;
    
    if (g_buffer_index < g_buffer_count && g_img_list[g_buffer_index] != NULL) {
        // Initialize the frame buffer struct with the image data
        buf->data = g_img_list[g_buffer_index];
        buf->priv = NULL;
        // Note: 'release' member does not exist in aom_codec_frame_buffer_t in this version.
        // The buffer is managed by the application via the get/release callbacks passed to set_frame_buffer_functions.
        g_buffer_index++;
        return 0; // Success
    }
    return 1; // Failure
}

// Callback: Release a frame buffer
// Signature: int (*)(void *cb_priv, aom_codec_frame_buffer_t *buf)
static int release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *buf) {
    (void)cb_priv;
    (void)buf;
    // In a real app, we might return the buffer to a pool. 
    // Here we just do nothing as we don't reuse in this simple fuzz logic.
    return 0;
}

static void cleanup_buffers(void) {
    for (int i = 0; i < g_buffer_count; i++) {
        if (g_img_list[i] != NULL) {
            aom_img_free(g_img_list[i]);
            g_img_list[i] = NULL;
        }
    }
    g_buffer_count = 0;
    g_buffer_index = 0;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Cleanup any previous state
    cleanup_buffers();

    // 1. Prepare environment
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface == NULL) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    // Initialize decoder context
    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Fuzz aom_codec_set_frame_buffer_functions
    // Allocate some dummy buffers
    int num_buffers = (Size % 5) + 1;
    for (int i = 0; i < num_buffers; i++) {
        init_buffer(i);
    }

    // Call the target function with correct callback signatures
    aom_codec_err_t res = aom_codec_set_frame_buffer_functions(
        &ctx, get_frame_buffer_cb, release_frame_buffer_cb, NULL);
    
    // The return value indicates if the setup was successful.
    // We don't decode here to keep the test focused on the API setup and image functions.

    // 3. Fuzz aom_img_plane_width
    if (g_buffer_count > 0) {
        aom_image_t *test_img = g_img_list[0];
        // Test valid plane indices
        for (int p = 0; p < 3; p++) {
            int w = aom_img_plane_width(test_img, p);
            (void)w; 
        }
        // Test invalid plane index
        int w_invalid = aom_img_plane_width(test_img, 10);
        (void)w_invalid;
    } else {
        // Create a temporary image to test aom_img_plane_width
        aom_image_t *tmp_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
        if (tmp_img != NULL) {
            int w = aom_img_plane_width(tmp_img, 0);
            (void)w;
            aom_img_free(tmp_img);
        }
    }

    // 4. Fuzz aom_img_free
    aom_image_t *img_to_free = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 32, 32, 1);
    if (img_to_free != NULL) {
        aom_img_free(img_to_free);
        img_to_free = NULL;
    }
    
    // Test freeing NULL
    aom_img_free(NULL);

    // 5. Cleanup
    aom_codec_destroy(&ctx);
    cleanup_buffers();

    return 0;
}