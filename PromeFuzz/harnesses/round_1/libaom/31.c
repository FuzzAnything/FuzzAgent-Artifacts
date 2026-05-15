// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static void test_aom_codec_get_caps(aom_codec_iface_t *iface) {
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning
}

static void test_aom_codec_iface_name(aom_codec_iface_t *iface) {
    const char *name = aom_codec_iface_name(iface);
    (void)name; // Use name to avoid unused variable warning
}

static void test_aom_codec_peek_stream_info(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_stream_info_t si;
    si.is_annexb = (size > 0) ? (data[0] & 1) : 0;
    
    aom_codec_err_t err = aom_codec_peek_stream_info(iface, data, size, &si);
    (void)err; // Use err to avoid unused variable warning
}

static void test_aom_codec_dec_init_ver_and_get_stream_info(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    
    // Initialize config with fuzzed data
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
    } else {
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.w = 128;
        cfg.h = 128;
        cfg.allow_lowbitdepth = 1;
    }
    
    aom_codec_flags_t flags = (size > 0) ? data[0] : 0;
    
    // Try to initialize decoder
    aom_codec_err_t init_err = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
    
    if (init_err == AOM_CODEC_OK) {
        // Try to get stream info
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.is_annexb = (size > 0) ? (data[0] & 1) : 0;
        
        aom_codec_err_t info_err = aom_codec_get_stream_info(&ctx, &si);
        (void)info_err; // Use info_err to avoid unused variable warning
        
        // Clean up
        aom_codec_destroy(&ctx);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;
    
    // Test aom_codec_get_caps with valid interface
    test_aom_codec_get_caps(iface);
    
    // Test aom_codec_get_caps with NULL
    test_aom_codec_get_caps(NULL);
    
    // Test aom_codec_iface_name with valid interface
    test_aom_codec_iface_name(iface);
    
    // Test aom_codec_iface_name with NULL
    test_aom_codec_iface_name(NULL);
    
    // Test aom_codec_peek_stream_info
    test_aom_codec_peek_stream_info(Data, Size);
    
    // Test decoder initialization and stream info
    test_aom_codec_dec_init_ver_and_get_stream_info(Data, Size);
    
    return 0;
}