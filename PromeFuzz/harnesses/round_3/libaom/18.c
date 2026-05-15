// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Call simple version functions that require no setup
    const char *extra_str = aom_codec_version_extra_str();
    (void)extra_str; // Use the result to avoid compiler warnings
    
    int version = aom_codec_version();
    (void)version;
    
    const char *build_config = aom_codec_build_config();
    (void)build_config;
    
    const char *version_str = aom_codec_version_str();
    (void)version_str;
    
    // Test aom_codec_iface_name with NULL pointer (should handle gracefully)
    const char *null_name = aom_codec_iface_name(NULL);
    (void)null_name;
    
    // Test aom_obu_type_to_string with various OBU_TYPE values
    // Use fuzzer input to generate diverse OBU_TYPE values
    if (Size > 0) {
        // Use the first byte to generate an OBU_TYPE value
        // OBU_TYPE is an enum, so we cast the byte value to it
        int obu_type_val = Data[0] % 16; // Limit to reasonable range
        const char *obu_str = aom_obu_type_to_string((int)obu_type_val);
        (void)obu_str;
        
        // Test with some specific values if we have enough data
        if (Size > 1) {
            // Test with value from second byte
            obu_type_val = Data[1] % 16;
            obu_str = aom_obu_type_to_string((int)obu_type_val);
            (void)obu_str;
        }
        
        // Test with potentially invalid value (outside normal enum range)
        if (Size > 2) {
            obu_type_val = Data[2]; // Could be any value 0-255
            obu_str = aom_obu_type_to_string((int)obu_type_val);
            (void)obu_str;
        }
    }
    
    // Test with some fixed values to ensure coverage
    const char *test_str = aom_obu_type_to_string(0);
    (void)test_str;
    
    test_str = aom_obu_type_to_string(1);
    (void)test_str;
    
    test_str = aom_obu_type_to_string(8);
    (void)test_str;
    
    test_str = aom_obu_type_to_string(15);
    (void)test_str;
    
    return 0;
}