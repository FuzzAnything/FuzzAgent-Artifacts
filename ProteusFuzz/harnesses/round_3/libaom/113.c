// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void cleanup_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment and get interface
    // Try to get the AV1 DX interface (may return NULL if not supported)
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Test aom_codec_get_caps and aom_codec_iface_name with potentially NULL iface
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Suppress unused warning

    const char *name = aom_codec_iface_name(iface);
    if (name) {
        // Just to use the variable and avoid unused warnings in some builds
        size_t len = strlen(name);
        (void)len;
    }

    // If interface is NULL (e.g., DX not available), we can't initialize a decoder with it.
    // We can still test peek_stream_info with NULL interface to test error handling.
    if (!iface) {
        aom_codec_stream_info_t si = {0};
        si.is_annexb = 1;
        aom_codec_err_t res = aom_codec_peek_stream_info(NULL, Data, Size, &si);
        (void)res;
        return 0;
    }

    // 2. Initialize decoder context
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    cfg.allow_lowbitdepth = 1;

    aom_codec_flags_t flags = 0;
    int ver = AOM_DECODER_ABI_VERSION;

    aom_codec_err_t init_res = aom_codec_dec_init_ver(ctx, iface, &cfg, flags, ver);
    
    // Test aom_codec_set_option even if init failed (to test error handling paths)
    // Only set option if context is not completely null/invalid, but API might handle it.
    // We try to set a dummy option.
    if (Size > 0) {
        // Create a safe string from fuzz data for option name/value if needed, 
        // but for safety we use static strings for keys and fuzz data for values if valid.
        // Here we just test with static strings to avoid complex parsing, 
        // but we can pass fuzz data as value if it's null-terminated.
        // To be safe, we ensure null termination for the value if we use fuzz data.
        char val_buf[256];
        size_t val_len = (Size < 255) ? Size : 255;
        memcpy(val_buf, Data, val_len);
        val_buf[val_len] = '\0';
        
        aom_codec_set_option(ctx, "dummy_option", val_buf);
    }

    // 3. Test aom_codec_peek_stream_info
    // We need a valid stream_info struct
    aom_codec_stream_info_t si = {0};
    // Toggle is_annexb based on fuzz data to explore states
    si.is_annexb = (Data[0] & 1) ? 1 : 0;
    
    aom_codec_err_t peek_res = aom_codec_peek_stream_info(iface, Data, Size, &si);
    (void)peek_res;

    // 4. Cleanup
    cleanup_ctx(ctx);

    return 0;
}