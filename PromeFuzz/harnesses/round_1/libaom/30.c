// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static void test_decode(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return;
    
    // Initialize decoder with default config
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result == AOM_CODEC_OK) {
        // Try to decode the data
        aom_codec_decode(&ctx, data, size, NULL);
        
        // Clean up
        aom_codec_destroy(&ctx);
    }
}

static void test_peek_stream_info(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return;
    
    aom_codec_stream_info_t si;
    si.is_annexb = 0;  // Must be initialized as per API requirements
    
    // Try to peek stream info with the fuzzed data
    aom_codec_peek_stream_info(iface, data, size, &si);
}

static void test_interface_functions(void) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return;
    
    // Test interface name function
    const char *name = aom_codec_iface_name(iface);
    (void)name;  // Use variable to avoid unused warning
    
    // Test capabilities function
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps;  // Use variable to avoid unused warning
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test interface functions (don't depend on input data)
    test_interface_functions();
    
    // Test peek stream info with the fuzzed data
    test_peek_stream_info(Data, Size);
    
    // Test decode with the fuzzed data
    test_decode(Data, Size);
    
    return 0;
}