// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void fuzz_version_info(void) {
    int version = vpx_codec_version();
    
    // Simply store the version without using the macros
    volatile int version_value = version;
    (void)version_value;
    
    // Call other version-related functions
    const char *version_str = vpx_codec_version_str();
    const char *extra_str = vpx_codec_version_extra_str();
    const char *build_config = vpx_codec_build_config();
    
    // Access the strings to ensure they're valid
    if (version_str) {
        volatile char first_char = version_str[0];
        (void)first_char;
    }
    if (extra_str) {
        volatile char first_char = extra_str[0];
        (void)first_char;
    }
    if (build_config) {
        volatile char first_char = build_config[0];
        (void)first_char;
    }
}

static void fuzz_interface_info(const uint8_t *data, size_t size) {
    // Get the VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Test with valid interface
    const char *iface_name = vpx_codec_iface_name(iface);
    if (iface_name) {
        volatile char first_char = iface_name[0];
        (void)first_char;
    }
    
    // Test with NULL interface (should handle gracefully)
    const char *null_name = vpx_codec_iface_name(NULL);
    (void)null_name;
    
    // Use fuzzer data to potentially affect behavior
    if (size > 0) {
        // Use first byte to decide whether to call functions multiple times
        uint8_t mode = data[0];
        for (int i = 0; i < (mode % 3); i++) {
            vpx_codec_iface_t *temp_iface = vpx_codec_vp8_cx();
            const char *temp_name = vpx_codec_iface_name(temp_iface);
            (void)temp_name;
        }
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    // Call version information functions
    fuzz_version_info();
    
    // Call interface-related functions with fuzzer data
    fuzz_interface_info(data, size);
    
    // Additional calls that might explore different states
    if (size > 1) {
        // Use second byte to determine additional calls
        uint8_t selector = data[1 % size];
        
        if (selector & 0x01) {
            // Call version functions again
            int version = vpx_codec_version();
            volatile int v = version;
            (void)v;
        }
        
        if (selector & 0x02) {
            // Call string functions again
            const char *version_str = vpx_codec_version_str();
            if (version_str) {
                volatile char c = version_str[0];
                (void)c;
            }
        }
        
        if (selector & 0x04) {
            // Get interface multiple times
            for (int i = 0; i < 2; i++) {
                vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
                (void)iface;
            }
        }
        
        if (selector & 0x08) {
            // Test with NULL multiple times
            for (int i = 0; i < 3; i++) {
                const char *name = vpx_codec_iface_name(NULL);
                (void)name;
            }
        }
    }
    
    return 0;
}