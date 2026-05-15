// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char* text) {
    // Silently ignore errors during fuzzing
    (void)context;
    (void)error_code;
    (void)text;
}

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        // Fallback to built-in sRGB profile if no data
        return cmsCreate_sRGBProfile();
    }
    
    // Try to open profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile != NULL) {
        return profile;
    }
    
    // If memory profile fails, create a simple sRGB profile
    return cmsCreate_sRGBProfile();
}

static cmsContext create_dummy_context(void) {
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        cmsSetLogErrorHandlerTHR(context, dummy_error_handler);
    }
    return context;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    
    // Create dummy profiles for testing
    size_t half_size = Size / 2;
    cmsHPROFILE profile1 = create_dummy_profile_from_data(Data, half_size);
    cmsHPROFILE profile2 = create_dummy_profile_from_data(Data + half_size, Size - half_size);
    
    // Create context for extended functions
    cmsContext context = create_dummy_context();
    
    // Test cmsCreateTransform
    if (selector & 0x01) {
        cmsUInt32Number input_format = (Data[1] % 4) == 0 ? TYPE_RGB_8 : 
                                      (Data[1] % 4) == 1 ? TYPE_RGB_16 :
                                      (Data[1] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number output_format = (Data[2] % 4) == 0 ? TYPE_RGB_8 : 
                                       (Data[2] % 4) == 1 ? TYPE_RGB_16 :
                                       (Data[2] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number intent = Data[3] % 4;
        cmsUInt32Number flags = Data[4] % 256;
        
        cmsHTRANSFORM transform = cmsCreateTransform(profile1, input_format, 
                                                    profile2, output_format, 
                                                    intent, flags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsCreateExtendedTransform
    if (selector & 0x02) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsBool bpc[2] = {Data[1] & 0x01, Data[2] & 0x01};
        cmsUInt32Number intents[2] = {Data[3] % 4, Data[4] % 4};
        cmsFloat64Number adaptation[2] = {(double)(Data[5] % 100) / 100.0, 
                                         (double)(Data[6] % 100) / 100.0};
        
        cmsUInt32Number input_format = (Data[7] % 4) == 0 ? TYPE_RGB_8 : 
                                      (Data[7] % 4) == 1 ? TYPE_RGB_16 :
                                      (Data[7] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number output_format = (Data[8] % 4) == 0 ? TYPE_RGB_8 : 
                                       (Data[8] % 4) == 1 ? TYPE_RGB_16 :
                                       (Data[8] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number flags = Data[9] % 256;
        
        cmsHTRANSFORM transform = cmsCreateExtendedTransform(context, 2, profiles,
                                                            bpc, intents, adaptation,
                                                            NULL, 0,
                                                            input_format, output_format,
                                                            flags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsCreateMultiprofileTransform
    if (selector & 0x04) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsUInt32Number input_format = (Data[1] % 4) == 0 ? TYPE_RGB_8 : 
                                      (Data[1] % 4) == 1 ? TYPE_RGB_16 :
                                      (Data[1] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number output_format = (Data[2] % 4) == 0 ? TYPE_RGB_8 : 
                                       (Data[2] % 4) == 1 ? TYPE_RGB_16 :
                                       (Data[2] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number intent = Data[3] % 4;
        cmsUInt32Number flags = Data[4] % 256;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, 2,
                                                                input_format, output_format,
                                                                intent, flags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsFormatterForPCSOfProfile
    if (selector & 0x08) {
        cmsUInt32Number nBytes = (Data[1] % 2) + 1;  // 1 or 2 bytes
        cmsBool isFloat = Data[2] & 0x01;
        
        cmsUInt32Number formatter1 = cmsFormatterForPCSOfProfile(profile1, nBytes, isFloat);
        cmsUInt32Number formatter2 = cmsFormatterForPCSOfProfile(profile2, nBytes, isFloat);
        (void)formatter1;
        (void)formatter2;
    }
    
    // Test cmsFormatterForColorspaceOfProfile
    if (selector & 0x10) {
        cmsUInt32Number nBytes = (Data[1] % 2) + 1;  // 1 or 2 bytes
        cmsBool isFloat = Data[2] & 0x01;
        
        cmsUInt32Number formatter1 = cmsFormatterForColorspaceOfProfile(profile1, nBytes, isFloat);
        cmsUInt32Number formatter2 = cmsFormatterForColorspaceOfProfile(profile2, nBytes, isFloat);
        (void)formatter1;
        (void)formatter2;
    }
    
    // Test cmsCreateProofingTransformTHR
    if (selector & 0x20) {
        cmsUInt32Number input_format = (Data[1] % 4) == 0 ? TYPE_RGB_8 : 
                                      (Data[1] % 4) == 1 ? TYPE_RGB_16 :
                                      (Data[1] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number output_format = (Data[2] % 4) == 0 ? TYPE_RGB_8 : 
                                       (Data[2] % 4) == 1 ? TYPE_RGB_16 :
                                       (Data[2] % 4) == 2 ? TYPE_CMYK_8 : TYPE_Lab_8;
        cmsUInt32Number intent = Data[3] % 4;
        cmsUInt32Number proofing_intent = Data[4] % 4;
        cmsUInt32Number flags = Data[5] % 256;
        
        cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(context, profile1, 
                                                               input_format, profile2,
                                                               output_format, profile1,
                                                               intent, proofing_intent,
                                                               flags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Cleanup
    if (context != NULL) {
        cmsDeleteContext(context);
    }
    
    // Only close profiles if they were created from memory (not built-in)
    if (Size > 0 && half_size > 0) {
        cmsCloseProfile(profile1);
    }
    if (Size > half_size && (Size - half_size) > 0) {
        cmsCloseProfile(profile2);
    }
    
    return 0;
}