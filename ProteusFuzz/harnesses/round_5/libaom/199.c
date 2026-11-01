// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;
    
    int idx = 0;
    int val;
    
    // Helper to read an int from fuzz data safely
    #define READ_INT() (idx + sizeof(int) <= size ? (idx += sizeof(int), *(int*)(data + idx - sizeof(int))) : (rand() % 256))
    
    // 1. AV1E_SET_MIN_CR
    // Expected: int (min_cr)
    val = READ_INT();
    aom_codec_control(ctx, AV1E_SET_MIN_CR, val);
    
    // 2. AV1E_SET_REDUCED_REFERENCE_SET
    // Expected: int (flag: 0 or 1)
    val = READ_INT() % 2;
    aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
    
    // 3. AV1E_SET_ENABLE_DIST_8X8
    // Note: This constant does not exist in the provided header. 
    // Using AV1E_SET_ENABLE_RECT_TX as a placeholder for valid control testing
    // or skipping if strictly required to match the non-existent name.
    // Given the task is to fuzz the library, we use a valid similar control.
    val = READ_INT() % 2;
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val);
    
    // 4. AV1E_SET_CHROMA_SUBSAMPLING_X
    // Expected: int (0 or 1 typically)
    val = READ_INT() % 2;
    aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
    
    // 5. AV1E_SET_MV_COST_UPD_FREQ
    // Expected: int (frequency)
    val = READ_INT();
    if (val < 0) val = 0;
    aom_codec_control(ctx, AV1E_SET_MV_COST_UPD_FREQ, val);
    
    // 6. AV1E_SET_QM_V
    // Expected: int (quantization matrix value)
    val = READ_INT();
    aom_codec_control(ctx, AV1E_SET_QM_V, val);
    
    return 0;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;
    
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }
    
    // Fuzz some config parameters based on input data
    if (size > 0) cfg.g_w = (data[0] % 255) + 16;
    if (size > 1) cfg.g_h = (data[1] % 255) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 255) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 255) + 1;
    if (size > 4) cfg.g_threads = (data[4] % 8) + 1;
    
    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0; // Need at least some data for config
    
    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;
    
    // Create a dummy frame to encode
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                     ctx->config.enc->g_w, ctx->config.enc->g_h, 16);
    if (!img) {
        aom_codec_destroy(ctx);
        free(ctx);
        return 0;
    }
    
    // Fill image with fuzz data
    memset(img->planes[0], Data[0], img->d_w * img->d_h);
    memset(img->planes[1], Data[1], img->d_w * img->d_h / 4);
    memset(img->planes[2], Data[2], img->d_w * img->d_h / 4);
    
    aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, 1);
    
    // Invoke the target control functions
    fuzz_encoder_controls(ctx, Data, Size);
    
    // Try to encode a frame to exercise the state
    // Fixed: aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags
    aom_codec_encode(ctx, img, 0, 1, 0);
    
    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(ctx);
    free(ctx);
    
    return 0;
}