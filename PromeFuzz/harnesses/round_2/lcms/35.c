// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    write_dummy_file(Data, Size);
    
    // Extract parameters from fuzzer input
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    // Helper to extract 32-bit integers
    #define EXTRACT_U32() (remaining >= 4 ? (ptr += 4, remaining -= 4, *(cmsUInt32Number*)(ptr-4)) : 0)
    // Helper to extract 64-bit floats (doubles)
    #define EXTRACT_F64() (remaining >= 8 ? (ptr += 8, remaining -= 8, *(cmsFloat64Number*)(ptr-8)) : 0.0)
    
    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Extract parameters for various functions
    cmsUInt32Number nLUTPoints = EXTRACT_U32() % 256 + 2;
    cmsFloat64Number bright = EXTRACT_F64();
    cmsFloat64Number contrast = EXTRACT_F64();
    cmsFloat64Number hue = EXTRACT_F64();
    cmsFloat64Number saturation = EXTRACT_F64();
    cmsUInt32Number tempSrc = EXTRACT_U32();
    cmsUInt32Number tempDest = EXTRACT_U32();
    
    cmsUInt32Number inputFormat = EXTRACT_U32();
    cmsUInt32Number outputFormat = EXTRACT_U32();
    cmsUInt32Number intent = EXTRACT_U32() % 4;
    cmsUInt32Number proofingIntent = EXTRACT_U32() % 4;
    cmsUInt32Number dwFlags = EXTRACT_U32();
    cmsFloat64Number adaptation = EXTRACT_F64();
    
    // Create dummy profiles using different methods
    cmsHPROFILE profile1 = cmsCreateBCHSWabstractProfile(nLUTPoints, bright, contrast, hue, saturation, tempSrc, tempDest);
    cmsHPROFILE profile2 = cmsCreateBCHSWabstractProfileTHR(context, nLUTPoints, bright, contrast, hue, saturation, tempSrc, tempDest);
    
    // Also try to open a profile from the dummy file
    cmsHPROFILE fileProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    // Use different profile combinations
    cmsHPROFILE inputProfile = profile1;
    cmsHPROFILE outputProfile = profile2;
    cmsHPROFILE proofingProfile = fileProfile ? fileProfile : profile1;
    
    // Test cmsSetAdaptationState
    cmsFloat64Number prevAdaptation = cmsSetAdaptationState(adaptation);
    
    // Test various transform creation functions
    cmsHTRANSFORM transform1 = cmsCreateTransform(inputProfile, inputFormat, outputProfile, outputFormat, intent, dwFlags);
    
    cmsHTRANSFORM transform2 = cmsCreateTransformTHR(context, inputProfile, inputFormat, outputProfile, outputFormat, intent, dwFlags);
    
    cmsHTRANSFORM transform3 = cmsCreateProofingTransform(inputProfile, inputFormat, outputProfile, outputFormat, proofingProfile, intent, proofingIntent, dwFlags);
    
    // Test with NULL profiles (device link)
    cmsHTRANSFORM transform4 = cmsCreateTransformTHR(context, inputProfile, inputFormat, NULL, outputFormat, intent, dwFlags);
    
    // Clean up transforms
    if (transform1) cmsDeleteTransform(transform1);
    if (transform2) cmsDeleteTransform(transform2);
    if (transform3) cmsDeleteTransform(transform3);
    if (transform4) cmsDeleteTransform(transform4);
    
    // Clean up profiles
    if (profile1) cmsCloseProfile(profile1);
    if (profile2) cmsCloseProfile(profile2);
    if (fileProfile) cmsCloseProfile(fileProfile);
    
    // Restore adaptation state
    cmsSetAdaptationState(prevAdaptation);
    
    // Clean up context
    cmsDeleteContext(context);
    
    cleanup_file();
    
    return 0;
}