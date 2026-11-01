// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

// Helper to safely destroy a codec context if it's valid
static void safe_destroy(aom_codec_ctx_t *ctx) {
    if (ctx != NULL && ctx->priv != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_229(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get interface pointers
    aom_codec_iface_t *iface_av1_cx = aom_codec_av1_cx();
    aom_codec_iface_t *iface_av1_dx = aom_codec_av1_dx();

    // Test aom_codec_version
    int version = aom_codec_version();
    (void)version; // Use to avoid unused warning

    // Test aom_codec_iface_name
    const char *name_cx = aom_codec_iface_name(iface_av1_cx);
    const char *name_dx = aom_codec_iface_name(iface_av1_dx);
    (void)name_cx;
    (void)name_dx;

    // Test aom_codec_get_caps
    aom_codec_caps_t caps_cx = aom_codec_get_caps(iface_av1_cx);
    aom_codec_caps_t caps_dx = aom_codec_get_caps(iface_av1_dx);
    (void)caps_cx;
    (void)caps_dx;

    // Prepare configuration
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Fuzz configuration values safely with strict bounds checking
    // We need at least 16 bytes to read 4 uint32_t values
    if (Size >= 16) {
        // Read values byte-by-byte or use memcpy to avoid alignment issues
        uint32_t val_threads, val_w, val_h, val_lowbit;
        
        memcpy(&val_threads, Data, sizeof(uint32_t));
        memcpy(&val_w, Data + 4, sizeof(uint32_t));
        memcpy(&val_h, Data + 8, sizeof(uint32_t));
        memcpy(&val_lowbit, Data + 12, sizeof(uint32_t));

        cfg.threads = val_threads;
        cfg.w = val_w;
        cfg.h = val_h;
        cfg.allow_lowbitdepth = val_lowbit;
    }

    // Limit values to reasonable ranges to avoid immediate crashes on invalid geometry
    if (cfg.w > 16384) cfg.w = 16384;
    if (cfg.h > 16384) cfg.h = 16384;
    if (cfg.threads > 16) cfg.threads = 16;
    cfg.allow_lowbitdepth = (cfg.allow_lowbitdepth != 0) ? 1 : 0;

    // Flags
    aom_codec_flags_t flags = 0;
    if (Size > 16) {
        uint64_t raw_flags;
        // Read up to sizeof(aom_codec_flags_t) safely
        size_t flag_size = sizeof(aom_codec_flags_t);
        if (Size - 16 < flag_size) flag_size = Size - 16;
        memcpy(&raw_flags, Data + 16, flag_size);
        flags = (aom_codec_flags_t)raw_flags;
    }

    // 2. Invoke target functions diversely

    // Try initializing with AV1 CX (Software)
    if (iface_av1_cx != NULL) {
        aom_codec_err_t ret = aom_codec_dec_init_ver(&ctx, iface_av1_cx, &cfg, flags, AOM_DECODER_ABI_VERSION);
        if (ret == AOM_CODEC_OK) {
            // Try setting options if initialized successfully
            // Construct a simple key/value from input data if possible
            if (Size > 20) {
                char key[32] = {0};
                char val[32] = {0};
                size_t k_len = (Size - 20 > 31) ? 31 : (Size - 20);
                
                // Ensure null termination
                if (k_len > 0) {
                    memcpy(key, Data + 20, k_len);
                    key[k_len] = '\0';
                }
                
                // Calculate remaining bytes for value
                size_t remaining = Size - (20 + k_len + 1); // +1 for separator
                size_t v_len = (remaining > 31) ? 31 : remaining;
                
                if (v_len > 0) {
                    memcpy(val, Data + 20 + k_len + 1, v_len);
                    val[v_len] = '\0';
                }
                
                aom_codec_set_option(&ctx, key, val);
            }
            // Cleanup
            safe_destroy(&ctx);
        }
    }

    // Try initializing with AV1 DX (Hardware) - might fail if no DX support, which is expected
    if (iface_av1_dx != NULL) {
        memset(&ctx, 0, sizeof(ctx)); // Reset context
        aom_codec_err_t ret = aom_codec_dec_init_ver(&ctx, iface_av1_dx, &cfg, flags, AOM_DECODER_ABI_VERSION);
        if (ret == AOM_CODEC_OK) {
            // Try setting options
            if (Size > 20) {
                char key[32] = {0};
                char val[32] = {0};
                size_t k_len = (Size - 20 > 31) ? 31 : (Size - 20);
                
                if (k_len > 0) {
                    memcpy(key, Data + 20, k_len);
                    key[k_len] = '\0';
                }
                
                size_t remaining = Size - (20 + k_len + 1);
                size_t v_len = (remaining > 31) ? 31 : remaining;
                
                if (v_len > 0) {
                    memcpy(val, Data + 20 + k_len + 1, v_len);
                    val[v_len] = '\0';
                }
                
                aom_codec_set_option(&ctx, key, val);
            }
            safe_destroy(&ctx);
        }
    }

    // Test with NULL interface (should handle gracefully)
    aom_codec_err_t ret_null = aom_codec_dec_init_ver(&ctx, NULL, &cfg, flags, AOM_DECODER_ABI_VERSION);
    (void)ret_null;

    // Test aom_codec_set_option with NULL context (should return error)
    aom_codec_set_option(NULL, "test", "value");

    // Test aom_codec_get_caps with NULL
    aom_codec_caps_t null_caps = aom_codec_get_caps(NULL);
    (void)null_caps;

    // Test aom_codec_iface_name with NULL
    const char *null_name = aom_codec_iface_name(NULL);
    (void)null_name;

    // 3. Cleanup
    // Contexts are destroyed inside the blocks above, but ensure final state is clean
    if (ctx.priv != NULL) {
        aom_codec_destroy(&ctx);
    }

    return 0;
}