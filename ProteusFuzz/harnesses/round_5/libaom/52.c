// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_ctx(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Get AV1 DX interface (may return NULL if not supported)
    aom_codec_iface_t *iface = aom_codec_av1_dx();

    // Test 1: aom_codec_iface_name
    // Works with valid iface or NULL
    const char *name = aom_codec_iface_name(iface);
    (void)name; // Suppress unused variable warning

    // Test 2: aom_codec_get_caps
    // Works with valid iface or NULL
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps;

    // Test 3: aom_codec_peek_stream_info
    aom_codec_stream_info_t si = {0};
    si.is_annexb = 1; // Must be initialized

    if (iface != NULL) {
        aom_codec_err_t peek_res = aom_codec_peek_stream_info(iface, Data, Size, &si);
        (void)peek_res;

        // Try with is_annexb = 0
        si.is_annexb = 0;
        aom_codec_err_t peek_res2 = aom_codec_peek_stream_info(iface, Data, Size, &si);
        (void)peek_res2;
    }

    // Test 4: Decoder initialization and option setting
    if (iface != NULL) {
        aom_codec_ctx_t ctx = {0};
        aom_codec_dec_cfg_t cfg = {0};

        // Initialize configuration from fuzz data if possible
        if (Size >= 4) {
            cfg.threads = Data[0] % 16;
            cfg.w = (Data[1] << 8) | Data[2];
            cfg.h = Data[3];
            cfg.allow_lowbitdepth = (Size > 4) ? (Data[4] % 2) : 0;
        }

        // Initialize decoder
        aom_codec_err_t init_res = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);

        if (init_res == AOM_CODEC_OK) {
            // Test 5: aom_codec_set_option
            // Try setting a dummy option
            const char *opt_name = "dummy_option";
            const char *opt_val = "dummy_value";
            aom_codec_err_t set_res = aom_codec_set_option(&ctx, opt_name, opt_val);
            (void)set_res;

            // Try with data from buffer as option name/value if printable
            if (Size > 2) {
                char buf_name[16] = {0};
                char buf_val[16] = {0};
                
                // Calculate safe lengths for copying (max 15 chars + null terminator)
                size_t n_len = (Size > 15) ? 15 : Size;
                size_t v_len = (Size - n_len > 15) ? 15 : (Size - n_len);
                
                // Ensure we don't read past the buffer
                if (n_len > 0) {
                    memcpy(buf_name, Data, n_len);
                    buf_name[n_len] = '\0';
                }
                
                if (v_len > 0 && n_len < Size) {
                    memcpy(buf_val, Data + n_len, v_len);
                    buf_val[v_len] = '\0';
                }

                aom_codec_err_t set_res2 = aom_codec_set_option(&ctx, buf_name, buf_val);
                (void)set_res2;
            }

            // Cleanup
            cleanup_ctx(&ctx);
        } else {
            // If init failed, ensure no leak (though ctx should be empty)
            cleanup_ctx(&ctx);
        }
    }

    return 0;
}