// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static void fuzz_decoder_init(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return;
    
    // Use fuzz data to determine configuration
    aom_codec_dec_cfg_t cfg;
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        data += sizeof(cfg);
        size -= sizeof(cfg);
    } else {
        cfg.w = 640;
        cfg.h = 480;
        cfg.threads = 1;
    }
    
    // Use fuzz data for flags and version
    aom_codec_flags_t flags = 0;
    int ver = AOM_DECODER_ABI_VERSION;
    
    if (size >= sizeof(flags)) {
        memcpy(&flags, data, sizeof(flags));
        data += sizeof(flags);
        size -= sizeof(flags);
        flags &= 0xFF; // Keep only valid flag bits
    }
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, ver);
    
    if (err == AOM_CODEC_OK) {
        // Try to decode with remaining data
        if (size > 0) {
            aom_codec_decode(&ctx, data, size, NULL);
        }
        aom_codec_destroy(&ctx);
    }
}

static void fuzz_interface_functions(void) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface) return;
    
    // Test interface name function
    const char *name = aom_codec_iface_name(iface);
    (void)name; // Suppress unused warning
    
    // Test get caps function
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Suppress unused warning
}

static void fuzz_peek_stream_info(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    if (!iface || size == 0) return;
    
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.is_annexb = (data[0] & 0x01); // Use first bit for is_annexb
    
    // Skip first byte for is_annexb
    if (size > 1) {
        aom_codec_peek_stream_info(iface, data + 1, size - 1, &si);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test interface functions
    fuzz_interface_functions();
    
    // Test stream info parsing
    fuzz_peek_stream_info(Data, Size);
    
    // Test decoder initialization and decoding
    fuzz_decoder_init(Data, Size);
    
    return 0;
}