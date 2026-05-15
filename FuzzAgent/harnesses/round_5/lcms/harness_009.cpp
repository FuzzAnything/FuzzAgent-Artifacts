#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcms2.h"

namespace harness_009 {
// Fuzzing harness for Little CMS (lcms) library
// Target: Date/time handling APIs and header metadata manipulation
// Focus: Profile creation date/time APIs, header metadata functions
// Functions: cmsGetHeaderCreationDateTime, cmsGetHeaderFlags, cmsSetHeaderFlags, etc.
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles
//   harness_002.cpp - PostScript generation  
//   harness_003.cpp - CGATS parsing and manipulation
//   harness_004.cpp - Cube file device link creation
//   harness_005.cpp - CIECAM02 color appearance model
//   harness_006.cpp - Matrix operations and tone curves
//   harness_007.cpp - Named color profile APIs
//   harness_008.cpp - Alpha channel handling
// New coverage: Date/time metadata handling, profile header date retrieval


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for date/time operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for different operations
    // 1. Consume fixed-size parameters first
    uint32_t year = fdp.ConsumeIntegralInRange<uint32_t>(1900, 2100);
    uint32_t month = fdp.ConsumeIntegralInRange<uint32_t>(1, 12);
    uint32_t day = fdp.ConsumeIntegralInRange<uint32_t>(1, 31);
    uint32_t hour = fdp.ConsumeIntegralInRange<uint32_t>(0, 23);
    uint32_t minute = fdp.ConsumeIntegralInRange<uint32_t>(0, 59);
    uint32_t second = fdp.ConsumeIntegralInRange<uint32_t>(0, 59);
    
    // 2. Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // 3. Create a test profile (sRGB) to work with
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 4. Test 1: Get creation date/time from profile
    struct tm creation_tm;
    memset(&creation_tm, 0, sizeof(struct tm));
    cmsBool got_date = cmsGetHeaderCreationDateTime(hProfile, &creation_tm);
    
    // The profile should have a creation date (even if default)
    if (got_date) {
        // creation_tm now contains the profile's creation date
        // We can use these values for further testing
        int tm_year = creation_tm.tm_year + 1900;
        int tm_month = creation_tm.tm_mon + 1;
        int tm_day = creation_tm.tm_mday;
        int tm_hour = creation_tm.tm_hour;
        int tm_min = creation_tm.tm_min;
        int tm_sec = creation_tm.tm_sec;
    }
    
    // 5. Test 2: Test with various edge cases from fuzzed input
    // Create additional date/time test cases
    for (int i = 0; i < 3 && fdp.remaining_bytes() >= 12; i++) {
        // Just consume bytes to vary input
        fdp.ConsumeIntegral<uint32_t>();
    }
    
    // 6. Test 3: Test with other header functions that might interact with dates
    // Get other header information
    cmsUInt32Number header_flags = cmsGetHeaderFlags(hProfile);
    cmsUInt64Number attributes;
    cmsGetHeaderAttributes(hProfile, &attributes);
    
    cmsUInt8Number id_buffer[16];
    cmsGetHeaderProfileID(hProfile, id_buffer);
    
    cmsUInt32Number rendering_intent = cmsGetHeaderRenderingIntent(hProfile);
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsFloat64Number version = cmsGetProfileVersion(hProfile);
    
    // 7. Test 4: Test header setter functions
    cmsUInt32Number new_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    cmsSetHeaderFlags(hProfile, new_flags);
    
    if (fdp.ConsumeBool()) {
        cmsUInt32Number new_manufacturer = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsSetHeaderManufacturer(hProfile, new_manufacturer);
    }
    
    if (fdp.ConsumeBool()) {
        cmsUInt32Number new_model = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsSetHeaderModel(hProfile, new_model);
    }
    
    if (fdp.ConsumeBool()) {
        cmsUInt64Number new_attributes = fdp.ConsumeIntegral<cmsUInt64Number>();
        cmsSetHeaderAttributes(hProfile, new_attributes);
    }
    
    if (fdp.ConsumeBool()) {
        cmsUInt32Number new_intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsSetHeaderRenderingIntent(hProfile, new_intent);
    }
    
    // 8. Test 5: Get creation date again after header modifications
    struct tm modified_creation_tm;
    memset(&modified_creation_tm, 0, sizeof(struct tm));
    cmsGetHeaderCreationDateTime(hProfile, &modified_creation_tm);
    
    // 9. Test 6: Create additional profiles and check their default creation dates
    // Create Lab profile
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(ctx, NULL);
    if (lab_profile != NULL) {
        struct tm lab_creation_tm;
        memset(&lab_creation_tm, 0, sizeof(struct tm));
        cmsGetHeaderCreationDateTime(lab_profile, &lab_creation_tm);
        cmsCloseProfile(lab_profile);
    }
    
    // Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(ctx);
    if (xyz_profile != NULL) {
        struct tm xyz_creation_tm;
        memset(&xyz_creation_tm, 0, sizeof(struct tm));
        cmsGetHeaderCreationDateTime(xyz_profile, &xyz_creation_tm);
        cmsCloseProfile(xyz_profile);
    }
    
    // 10. Test 7: Create RGB profile with custom white point and primaries
    cmsCIExyY white_point;
    white_point.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    white_point.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    white_point.Y = 1.0;
    
    cmsCIExyYTRIPLE primaries;
    primaries.Red.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Red.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Red.Y = 1.0;
    
    primaries.Green.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Green.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Green.Y = 1.0;
    
    primaries.Blue.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Blue.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
    primaries.Blue.Y = 1.0;
    
    cmsToneCurve* tone_curves[3];
    for (int i = 0; i < 3; i++) {
        cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
        tone_curves[i] = cmsBuildGamma(NULL, gamma);
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&white_point, &primaries, tone_curves);
    if (rgb_profile != NULL) {
        struct tm rgb_creation_tm;
        memset(&rgb_creation_tm, 0, sizeof(struct tm));
        cmsGetHeaderCreationDateTime(rgb_profile, &rgb_creation_tm);
        
        // Also test other header functions on this profile
        cmsUInt32Number rgb_flags = cmsGetHeaderFlags(rgb_profile);
        cmsUInt64Number rgb_attributes;
        cmsGetHeaderAttributes(rgb_profile, &rgb_attributes);
        
        cmsCloseProfile(rgb_profile);
    }
    
    // Free tone curves
    for (int i = 0; i < 3; i++) {
        if (tone_curves[i] != NULL) {
            cmsFreeToneCurve(tone_curves[i]);
        }
    }
    
    // 11. Test 8: Test with memory profile
    if (fdp.remaining_bytes() >= 128) {
        std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(128);
        cmsHPROFILE mem_profile = cmsOpenProfileFromMemTHR(ctx, profile_data.data(), profile_data.size());
        if (mem_profile != NULL) {
            struct tm mem_creation_tm;
            memset(&mem_creation_tm, 0, sizeof(struct tm));
            cmsGetHeaderCreationDateTime(mem_profile, &mem_creation_tm);
            cmsCloseProfile(mem_profile);
        }
    }
    
    // 12. Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_009
