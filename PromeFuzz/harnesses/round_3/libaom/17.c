// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "aom/aom_codec.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Call functions that don't require any input parameters
    const char *version_extra = aom_codec_version_extra_str();
    (void)version_extra; // Avoid unused variable warning
    
    int version = aom_codec_version();
    (void)version; // Avoid unused variable warning
    
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Avoid unused variable warning
    
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Avoid unused variable warning
    
    // Call aom_codec_iface_name with NULL pointer only
    // The function handles null pointer gracefully
    const char *iface_name = aom_codec_iface_name(NULL);
    (void)iface_name; // Avoid unused variable warning
    
    // Call aom_obu_type_to_string with various enum values
    // Use fuzzer input to generate different OBU_TYPE values
    if (Size > 0) {
        // Use first byte to generate OBU_TYPE value
        // Cast to OBU_TYPE enum (note: this may generate invalid values)
        OBU_TYPE obu_type = (OBU_TYPE)Data[0];
        const char *obu_str = aom_obu_type_to_string(obu_type);
        (void)obu_str; // Avoid unused variable warning
    }
    
    // Try more OBU_TYPE values if we have more data
    for (size_t i = 1; i < Size && i < 20; i++) {
        OBU_TYPE obu_type = (OBU_TYPE)Data[i];
        const char *obu_str = aom_obu_type_to_string(obu_type);
        (void)obu_str; // Avoid unused variable warning
    }
    
    return 0;
}