// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile(void) {
    // Create a simple RGB profile for testing
    cmsCIExyY whitepoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
    cmsToneCurve* gamma_curve[3] = {gamma, gamma, gamma};
    
    cmsHPROFILE profile = cmsCreateRGBProfile(&whitepoint, &primaries, gamma_curve);
    
    cmsFreeToneCurve(gamma);
    return profile;
}

static cmsTagSignature get_tag_from_data(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(cmsTagSignature) <= size) {
        return *(cmsTagSignature*)(data + offset);
    }
    return cmsSigRedColorantTag; // Default tag
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a test profile
    cmsHPROFILE profile = create_test_profile();
    if (!profile) return 0;
    
    // Use fuzzer data to determine which operations to perform
    uint8_t operation = Data[0] % 6;
    size_t data_offset = 1;
    
    // Extract tag signatures from fuzzer data
    cmsTagSignature tag1 = get_tag_from_data(Data, Size, data_offset);
    data_offset += sizeof(cmsTagSignature);
    
    cmsTagSignature tag2 = get_tag_from_data(Data, Size, data_offset);
    data_offset += sizeof(cmsTagSignature);
    
    switch (operation) {
        case 0: { // Test cmsSaveProfileToMem
            cmsUInt32Number bytes_needed = 0;
            
            // First call to get required size
            cmsSaveProfileToMem(profile, NULL, &bytes_needed);
            
            if (bytes_needed > 0) {
                void* buffer = malloc(bytes_needed);
                if (buffer) {
                    cmsSaveProfileToMem(profile, buffer, &bytes_needed);
                    free(buffer);
                }
            }
            break;
        }
        
        case 1: { // Test cmsLinkTag
            cmsLinkTag(profile, tag1, tag2);
            break;
        }
        
        case 2: { // Test cmsWriteTag
            // Create a simple gamma curve for writing
            cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
            if (gamma) {
                cmsWriteTag(profile, tag1, gamma);
                cmsFreeToneCurve(gamma);
            }
            
            // Test tag deletion with NULL data
            if (Size % 2 == 0) {
                cmsWriteTag(profile, tag1, NULL);
            }
            break;
        }
        
        case 3: { // Test cmsWriteRawTag
            size_t remaining_size = (Size > data_offset) ? Size - data_offset : 0;
            if (remaining_size > 0) {
                cmsWriteRawTag(profile, tag1, Data + data_offset, 
                              (cmsUInt32Number)(remaining_size > UINT32_MAX ? UINT32_MAX : remaining_size));
            }
            break;
        }
        
        case 4: { // Test cmsIsTag
            cmsIsTag(profile, tag1);
            cmsIsTag(profile, tag2);
            break;
        }
        
        case 5: { // Test cmsMD5computeID
            cmsMD5computeID(profile);
            break;
        }
    }
    
    // Clean up
    cmsCloseProfile(profile);
    
    return 0;
}