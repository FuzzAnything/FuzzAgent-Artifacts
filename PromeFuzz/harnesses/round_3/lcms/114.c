// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for potential profile operations
    write_dummy_file(Data, Size);
    
    // Generate values for function arguments from fuzzing input
    int our_notation = (int)(Data[0] | (Size > 1 ? Data[1] << 8 : 0));
    cmsColorSpaceSignature color_space = (cmsColorSpaceSignature)(
        Data[0] | 
        (Size > 1 ? Data[1] << 8 : 0) |
        (Size > 2 ? Data[2] << 16 : 0) |
        (Size > 3 ? Data[3] << 24 : 0)
    );
    
    // Test _cmsICCcolorSpace
    cmsColorSpaceSignature icc_result = _cmsICCcolorSpace(our_notation);
    (void)icc_result;
    
    // Test cmsChannelsOf (deprecated)
    cmsUInt32Number channels = cmsChannelsOf(color_space);
    (void)channels;
    
    // Test cmsChannelsOfColorSpace
    cmsInt32Number channels2 = cmsChannelsOfColorSpace(color_space);
    (void)channels2;
    
    // Test _cmsLCMScolorSpace
    int lcms_result = _cmsLCMScolorSpace(color_space);
    (void)lcms_result;
    
    // Test profile-related functions
    cmsHPROFILE hProfile = NULL;
    
    // Try to open profile from dummy file
    hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    if (hProfile != NULL) {
        // Test cmsGetColorSpace
        cmsColorSpaceSignature profile_space = cmsGetColorSpace(hProfile);
        (void)profile_space;
        
        // Test cmsSetColorSpace with various signatures
        // Use different parts of input for different signatures
        for (int i = 0; i < 4 && i * 4 < (int)Size; i++) {
            cmsColorSpaceSignature sig = (cmsColorSpaceSignature)(
                Data[i * 4 % Size] |
                (Size > i * 4 + 1 ? Data[(i * 4 + 1) % Size] << 8 : 0) |
                (Size > i * 4 + 2 ? Data[(i * 4 + 2) % Size] << 16 : 0) |
                (Size > i * 4 + 3 ? Data[(i * 4 + 3) % Size] << 24 : 0)
            );
            cmsSetColorSpace(hProfile, sig);
        }
        
        cmsCloseProfile(hProfile);
    } else {
        // Create a new profile if file open failed
        hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
        if (hProfile != NULL) {
            // Test cmsGetColorSpace on newly created profile
            cmsColorSpaceSignature new_space = cmsGetColorSpace(hProfile);
            (void)new_space;
            
            // Test cmsSetColorSpace
            cmsSetColorSpace(hProfile, color_space);
            
            // Test cmsGetColorSpace again after setting
            cmsColorSpaceSignature updated_space = cmsGetColorSpace(hProfile);
            (void)updated_space;
            
            cmsCloseProfile(hProfile);
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}