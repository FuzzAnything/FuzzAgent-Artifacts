// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char *error_text) {
    // Do nothing - suppress error messages during fuzzing
}

static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        cmsSetLogErrorHandler(dummy_error_handler);
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use a small portion of input for deterministic parameters
    size_t param_size = Size > 16 ? 16 : Size;
    
    // Create a context for allocation
    cmsContext ctx = create_dummy_context();
    if (!ctx) return 0;
    
    // Extract parameters from fuzz data
    cmsUInt32Number n = 0;
    cmsUInt32Number colorant_count = 0;
    char prefix[33] = {0};
    char suffix[33] = {0};
    
    if (param_size >= 8) {
        n = ((cmsUInt32Number)Data[0] << 24) | 
            ((cmsUInt32Number)Data[1] << 16) | 
            ((cmsUInt32Number)Data[2] << 8) | 
            (cmsUInt32Number)Data[3];
        
        colorant_count = ((cmsUInt32Number)Data[4] << 24) | 
                        ((cmsUInt32Number)Data[5] << 16) | 
                        ((cmsUInt32Number)Data[6] << 8) | 
                        (cmsUInt32Number)Data[7];
        
        // Limit to reasonable values to avoid excessive memory allocation
        n = n % 256;
        colorant_count = colorant_count % 16;
        
        // Use some data for prefix/suffix if available
        if (param_size >= 10) {
            size_t prefix_len = Data[8] % 32;
            size_t suffix_len = Data[9] % 32;
            
            if (prefix_len > 0 && param_size >= 10 + prefix_len) {
                memcpy(prefix, Data + 10, prefix_len);
                prefix[prefix_len] = '\0';
            } else {
                prefix[0] = '\0';
            }
            
            if (suffix_len > 0 && param_size >= 10 + prefix_len + suffix_len) {
                memcpy(suffix, Data + 10 + prefix_len, suffix_len);
                suffix[suffix_len] = '\0';
            } else {
                suffix[0] = '\0';
            }
        } else {
            prefix[0] = '\0';
            suffix[0] = '\0';
        }
    }
    
    // Test cmsAllocNamedColorList - pass empty strings instead of NULL
    const char* prefix_ptr = (prefix[0] == '\0') ? "" : prefix;
    const char* suffix_ptr = (suffix[0] == '\0') ? "" : suffix;
    
    cmsNAMEDCOLORLIST* list = cmsAllocNamedColorList(ctx, n, colorant_count, 
                                                    prefix_ptr, suffix_ptr);
    
    if (list) {
        // Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(list);
        
        // Test cmsNamedColorInfo for some indices
        if (count > 0) {
            // Try a few indices based on fuzz data
            for (int i = 0; i < 3 && i < (int)count; i++) {
                char name[256] = {0};
                char out_prefix[33] = {0};
                char out_suffix[33] = {0};
                cmsUInt16Number pcs[3] = {0};
                cmsUInt16Number colorant[16] = {0};
                
                cmsUInt32Number index = 0;
                if (param_size >= 12 + i) {
                    index = Data[11 + i] % count;
                }
                
                cmsNamedColorInfo(list, index, name, out_prefix, out_suffix, pcs, colorant);
            }
        }
        
        // Test cmsDupNamedColorList
        cmsNAMEDCOLORLIST* dup_list = cmsDupNamedColorList(list);
        
        // Test cmsNamedColorCount on duplicate
        if (dup_list) {
            cmsUInt32Number dup_count = cmsNamedColorCount(dup_list);
            
            // Test cmsNamedColorInfo on duplicate
            if (dup_count > 0 && param_size >= 15) {
                cmsUInt32Number index = Data[14] % dup_count;
                if (index < dup_count) {
                    char name[256] = {0};
                    char out_prefix[33] = {0};
                    char out_suffix[33] = {0};
                    cmsUInt16Number pcs[3] = {0};
                    cmsUInt16Number colorant[16] = {0};
                    
                    cmsNamedColorInfo(dup_list, index, name, out_prefix, out_suffix, pcs, colorant);
                }
            }
            
            // Test cmsFreeNamedColorList on duplicate
            cmsFreeNamedColorList(dup_list);
        }
        
        // Test cmsGetNamedColorList with dummy transform
        // Create a dummy file with fuzz data
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
            
            // Try to open as profile
            cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            if (hProfile) {
                // Create a simple transform
                cmsHPROFILE sRGB = cmsCreate_sRGBProfile();
                if (sRGB) {
                    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_RGB_8, 
                                                            sRGB, TYPE_RGB_8, 
                                                            INTENT_PERCEPTUAL, 0);
                    if (xform) {
                        // Test cmsGetNamedColorList
                        cmsNAMEDCOLORLIST* transform_list = cmsGetNamedColorList(xform);
                        if (transform_list) {
                            cmsUInt32Number transform_count = cmsNamedColorCount(transform_list);
                            // Don't free - owned by transform
                        }
                        cmsDeleteTransform(xform);
                    }
                    cmsCloseProfile(sRGB);
                }
                cmsCloseProfile(hProfile);
            }
            remove("./dummy_file");
        }
        
        // Test cmsFreeNamedColorList on original
        cmsFreeNamedColorList(list);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}