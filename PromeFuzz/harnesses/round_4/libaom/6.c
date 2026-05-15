// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static aom_codec_err_t dummy_decode_callback(void *user_priv, const void *data, size_t data_sz, aom_codec_frame_flags_t flags) {
    (void)user_priv;
    (void)data;
    (void)data_sz;
    (void)flags;
    return AOM_CODEC_OK;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Test aom_codec_iface_name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name;

    // Test aom_codec_get_caps
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps;

    // Test aom_codec_peek_stream_info
    aom_codec_stream_info_t stream_info;
    stream_info.is_annexb = Data[0] & 1; // Use first byte to set is_annexb
    aom_codec_err_t peek_err = aom_codec_peek_stream_info(
        iface, 
        Data, 
        Size, 
        &stream_info
    );
    (void)peek_err;

    // Test aom_codec_dec_init_ver with different configurations
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    
    // Initialize cfg with values from fuzzer input
    if (Size >= 5) {
        cfg.threads = Data[1] % 8 + 1; // 1-8 threads
        cfg.w = (Data[2] << 8) | Data[3];
        cfg.h = (Data[4] << 8) | Data[5];
        if (cfg.w == 0) cfg.w = 128;
        if (cfg.h == 0) cfg.h = 128;
    } else {
        cfg.threads = 1;
        cfg.w = 128;
        cfg.h = 128;
    }

    // Try initialization with different flags
    aom_codec_flags_t flags = 0;
    if (Size >= 2) {
        flags = Data[1] & 0xFF;
    }

    // First try with NULL config
    aom_codec_err_t init_err = aom_codec_dec_init_ver(
        &ctx,
        iface,
        NULL,
        flags,
        AOM_DECODER_ABI_VERSION
    );

    if (init_err == AOM_CODEC_OK) {
        // Test aom_codec_decode with the initialized decoder
        void *user_priv = NULL;
        if (Size >= 3) {
            user_priv = (void *)(uintptr_t)Data[2];
        }
        
        aom_codec_err_t decode_err = aom_codec_decode(
            &ctx,
            Data,
            Size,
            user_priv
        );
        (void)decode_err;

        // Clean up
        aom_codec_destroy(&ctx);
    }

    // Try initialization with config
    init_err = aom_codec_dec_init_ver(
        &ctx,
        iface,
        &cfg,
        flags,
        AOM_DECODER_ABI_VERSION
    );

    if (init_err == AOM_CODEC_OK) {
        // Test aom_codec_decode again with different parameters
        for (size_t i = 0; i < (Size > 16 ? 16 : Size); i++) {
            size_t chunk_size = Size / (i + 1);
            if (chunk_size > 0) {
                aom_codec_err_t decode_err = aom_codec_decode(
                    &ctx,
                    Data,
                    chunk_size,
                    NULL
                );
                (void)decode_err;
            }
        }

        // Clean up
        aom_codec_destroy(&ctx);
    }

    // Try with different ABI versions (some invalid)
    if (Size >= 3) {
        int abi_version = (int)Data[2];
        aom_codec_err_t init_err = aom_codec_dec_init_ver(
            &ctx,
            iface,
            NULL,
            0,
            abi_version
        );
        if (init_err == AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
        }
    }

    return 0;
}