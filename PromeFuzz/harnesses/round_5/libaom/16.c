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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"

static volatile int sink;

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Call functions that don't require any input
    const char *extra_str = aom_codec_version_extra_str();
    if (extra_str) sink = extra_str[0];
    
    int version = aom_codec_version();
    sink = version;
    
    const char *build_config = aom_codec_build_config();
    if (build_config) sink = build_config[0];
    
    const char *version_str = aom_codec_version_str();
    if (version_str) sink = version_str[0];
    
    // Test aom_codec_iface_name with NULL
    const char *iface_name = aom_codec_iface_name(NULL);
    if (iface_name) sink = iface_name[0];
    
    // Test aom_codec_iface_name with AV1 encoder interface if available
    // Note: aom_codec_av1_cx() might not be declared in aom_codec.h alone,
    // but we'll try to use it if the header includes it
    #ifdef aom_codec_av1_cx
    iface_name = aom_codec_iface_name(aom_codec_av1_cx());
    if (iface_name) sink = iface_name[0];
    #endif
    
    // Test aom_codec_iface_name with AV1 decoder interface if available
    #ifdef aom_codec_av1_dx
    iface_name = aom_codec_iface_name(aom_codec_av1_dx());
    if (iface_name) sink = iface_name[0];
    #endif
    
    // Test aom_obu_type_to_string with various values from fuzz input
    if (Size > 0) {
        // Use the first byte as OBU_TYPE
        OBU_TYPE type = (OBU_TYPE)Data[0];
        const char *obu_str = aom_obu_type_to_string(type);
        if (obu_str) sink = obu_str[0];
        
        // Try a few more values if we have enough data
        for (size_t i = 1; i < Size && i < 8; i++) {
            type = (OBU_TYPE)Data[i];
            obu_str = aom_obu_type_to_string(type);
            if (obu_str) sink = obu_str[0];
        }
        
        // Also try some boundary values
        obu_str = aom_obu_type_to_string((OBU_TYPE)0);
        if (obu_str) sink = obu_str[0];
        
        obu_str = aom_obu_type_to_string((OBU_TYPE)255);
        if (obu_str) sink = obu_str[0];
    }
    
    return 0;
}