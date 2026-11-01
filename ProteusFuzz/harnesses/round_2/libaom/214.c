// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_dec_cfg_t cfg;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(&ctx, &iter);
        // Just consume the frame to advance state, no need to free here as it's managed by ctx
        (void)img;
    }

    // We return a pointer to a local variable if we want to reuse it, 
    // but for fuzzing safety we usually pass the context by value or manage it carefully.
    // Since aom_codec_ctx_t is a struct, we can't return a pointer to a stack variable safely 
    // if the caller expects to own it. However, the standard pattern for these fuzzer helpers 
    // is to return the pointer to the struct if it was malloc'd, or we handle the lifecycle 
    // entirely within the fuzzer function. 
    // Let's change strategy: return the struct itself or handle it in place.
    // To match the previous logic of "init" and "cleanup", let's allocate on heap.
    aom_codec_ctx_t *heap_ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!heap_ctx) return NULL;
    memcpy(heap_ctx, &ctx, sizeof(ctx));
    // Note: The internal pointers in ctx (like priv) are allocated by the init function.
    // Simply memcpying might break internal references if they are relative to the stack.
    // The correct way is to init directly on the heap.
    
    // Re-doing init on heap properly:
    aom_codec_destroy(&ctx); // Clean up stack version
    
    memset(heap_ctx, 0, sizeof(aom_codec_ctx_t));
    ret = aom_codec_dec_init(heap_ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(heap_ctx);
        return NULL;
    }

    if (size > 0) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(heap_ctx, &iter);
        (void)img;
    }

    return heap_ctx;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;

    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    memset(&cfg, 0, sizeof(cfg));
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    (void)data;
    (void)size;

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_214(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Fuzz Decoder Controls
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        aom_tile_info tile_info;
        aom_tile_data tile_data;
        int single_tile = 0;
        int tile_count = 0;
        int tile_width = 0;
        int tile_height = 0;

        // AV1D_GET_TILE_DATA
        aom_codec_control(dec_ctx, AV1D_GET_TILE_DATA, &tile_data);
        
        // AV1D_GET_TILE_COUNT
        aom_codec_control(dec_ctx, AV1D_GET_TILE_COUNT, &tile_count);
        
        // AV1D_GET_TILE_SIZE
        aom_codec_control(dec_ctx, AV1D_GET_TILE_SIZE, &tile_width, &tile_height);
        
        // AOMD_GET_TILE_INFO
        aom_codec_control(dec_ctx, AOMD_GET_TILE_INFO, &tile_info);
        
        // AV1D_EXT_TILE_DEBUG
        int debug_val = 0;
        aom_codec_control(dec_ctx, AV1D_EXT_TILE_DEBUG, &debug_val);

        cleanup_decoder(dec_ctx);
    }

    // Fuzz Encoder Controls
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        int single_tile = 1;
        // AV1E_SET_SINGLE_TILE_DECODING
        aom_codec_control(enc_ctx, AV1E_SET_SINGLE_TILE_DECODING, &single_tile);
        
        cleanup_encoder(enc_ctx);
    }

    return 0;
}