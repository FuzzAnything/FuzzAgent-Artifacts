// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Part 1: Test cmsGetNamedColorList and related functions
    // First, try to create a transform from the fuzz data
    if (Size > 0) {
        // Write data to file to create a profile
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            // Try to open as profile
            cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            if (hProfile) {
                // Create a simple transform
                cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_RGB_8, 
                                                         hProfile, TYPE_RGB_8, 
                                                         INTENT_PERCEPTUAL, 0);
                if (xform) {
                    // Test cmsGetNamedColorList
                    cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(xform);
                    
                    if (namedColorList) {
                        // Test cmsNamedColorCount
                        cmsUInt32Number count = cmsNamedColorCount(namedColorList);
                        
                        // Test cmsNamedColorInfo for some indices
                        if (count > 0) {
                            // Try a few indices based on fuzz data
                            for (cmsUInt32Number i = 0; i < 5 && i < count; i++) {
                                char Name[256];
                                char Prefix[33];
                                char Suffix[33];
                                cmsUInt16Number PCS[3];
                                cmsUInt16Number Colorant[16]; // Max colorants
                                
                                cmsNamedColorInfo(namedColorList, i, Name, Prefix, 
                                                 Suffix, PCS, Colorant);
                            }
                        }
                        
                        // Test cmsDupNamedColorList
                        cmsNAMEDCOLORLIST* dupList = cmsDupNamedColorList(namedColorList);
                        if (dupList) {
                            cmsFreeNamedColorList(dupList);
                        }
                    }
                    
                    cmsDeleteTransform(xform);
                }
                cmsCloseProfile(hProfile);
            }
            remove("./dummy_file");
        }
    }
    
    // Part 2: Test cmsAllocNamedColorList and related functions
    if (Size >= 8) {
        // Use fuzz data to determine parameters
        cmsUInt32Number n = ((cmsUInt32Number*)Data)[0] % 1000; // Limit to reasonable size
        cmsUInt32Number ColorantCount = ((cmsUInt32Number*)Data)[1] % 16; // Max 16 colorants
        
        // Extract prefix and suffix from data if available
        const char* prefix = "";
        const char* suffix = "";
        
        size_t offset = 8;
        if (Size >= offset + 64) {
            // Create null-terminated strings from data
            static char prefix_buf[33];
            static char suffix_buf[33];
            
            size_t prefix_len = (Size - offset >= 32) ? 32 : (Size - offset);
            memcpy(prefix_buf, Data + offset, prefix_len);
            prefix_buf[prefix_len] = '\0';
            prefix = prefix_buf;
            
            offset += 32;
            if (Size >= offset + 32) {
                size_t suffix_len = (Size - offset >= 32) ? 32 : (Size - offset);
                memcpy(suffix_buf, Data + offset, suffix_len);
                suffix_buf[suffix_len] = '\0';
                suffix = suffix_buf;
            }
        }
        
        // Allocate named color list
        cmsNAMEDCOLORLIST* newList = cmsAllocNamedColorList(NULL, n, ColorantCount, 
                                                           prefix, suffix);
        if (newList) {
            // Test cmsNamedColorCount on newly allocated list
            cmsUInt32Number newCount = cmsNamedColorCount(newList);
            
            // Test cmsNamedColorInfo (should fail since list is empty)
            if (newCount > 0) {
                char Name[256];
                char Prefix[33];
                char Suffix[33];
                cmsUInt16Number PCS[3];
                cmsUInt16Number Colorant[16];
                
                // Try first index
                cmsNamedColorInfo(newList, 0, Name, Prefix, Suffix, PCS, Colorant);
            }
            
            // Test cmsDupNamedColorList on newly created list
            cmsNAMEDCOLORLIST* dupNewList = cmsDupNamedColorList(newList);
            if (dupNewList) {
                cmsFreeNamedColorList(dupNewList);
            }
            
            // Clean up
            cmsFreeNamedColorList(newList);
        }
    }
    
    return 0;
}