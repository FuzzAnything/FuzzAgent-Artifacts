// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lcms2.h"

static cmsHPROFILE create_test_profile(void) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfile();
}

static uint32_t extract_uint32(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 4 > size) {
        // Not enough data, use default value
        *offset = size;
        return 0xDEADBEEF;
    }
    
    uint32_t value = 0;
    for (int i = 0; i < 4; i++) {
        value = (value << 8) | data[*offset + i];
    }
    *offset += 4;
    return value;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHPROFILE profile = create_test_profile();
    if (!profile) return 0;
    
    size_t offset = 0;
    
    // Test cmsSetHeaderFlags
    if (offset < Size) {
        uint32_t flags = extract_uint32(Data, Size, &offset);
        cmsSetHeaderFlags(profile, flags);
    }
    
    // Test cmsSetEncodedICCversion
    if (offset < Size) {
        uint32_t version = extract_uint32(Data, Size, &offset);
        cmsSetEncodedICCversion(profile, version);
    }
    
    // Test cmsGetHeaderCreator
    uint32_t creator = cmsGetHeaderCreator(profile);
    (void)creator; // Use the value to avoid compiler warnings
    
    // Test cmsGetEncodedICCversion
    uint32_t retrieved_version = cmsGetEncodedICCversion(profile);
    (void)retrieved_version;
    
    // Test cmsSetHeaderRenderingIntent
    if (offset < Size) {
        uint32_t intent = extract_uint32(Data, Size, &offset);
        cmsSetHeaderRenderingIntent(profile, intent);
    }
    
    // Test cmsSetHeaderModel
    if (offset < Size) {
        uint32_t model = extract_uint32(Data, Size, &offset);
        cmsSetHeaderModel(profile, model);
    }
    
    // Clean up
    cmsCloseProfile(profile);
    
    return 0;
}