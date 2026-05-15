#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <cwchar>
#include "lcms2.h"

namespace harness_021 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Dictionary management APIs for ICC profiles
 * Source modules: Dictionary management functions (likely in cmsio1.c or similar)
 * Semantic domain: Dictionary allocation, entry management, and traversal
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript CRD generation
 *   002-virtual profiles/gamut detection
 *   003-CGATS/IT8 basic operations
 *   004-CIECAM02 appearance model
 *   005-TAC detection/sampling
 *   006-device link creation/I/O
 *   007-BCHSW abstract profiles
 *   008-advanced CGATS/IT8
 *   009-cube file device link creation/stream I/O
 *   010-pipeline/MPE operations
 *   011-PostScript color resources
 *   012-MD5/half-precision
 *   013-context management/plugin system
 *   014-color science algorithms and metrics
 *   015-named color list management (basic)
 *   016-? (check if exists)
 *   017-? (check if exists)
 *   018-? (check if exists)
 *   019-? (check if exists)
 *   020-named color list management (advanced edge cases)
 * This harness focuses on: Dictionary creation, entry addition, iteration, duplication
 */


// Include Little CMS headers

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create a context for dictionary operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Phase 1: Test dictionary allocation and basic operations
    
    // Allocate a dictionary
    cmsHANDLE hDict = cmsDictAlloc(context);
    if (hDict == NULL) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Phase 2: Add entries to the dictionary
    
    // Determine how many entries to add
    cmsUInt32Number num_entries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 20);
    
    for (cmsUInt32Number i = 0; i < num_entries; i++) {
        // Check if we have enough data left
        if (fdp.remaining_bytes() < 50) {
            break;
        }
        
        // Consume entry name and value as wide strings
        std::string name_str = fdp.ConsumeRandomLengthString(63);
        std::string value_str = fdp.ConsumeRandomLengthString(127);
        
        // Convert to wide strings (required by cmsDictAddEntry)
        std::wstring wname(name_str.begin(), name_str.end());
        std::wstring wvalue(value_str.begin(), value_str.end());
        
        // Test with NULL DisplayName and DisplayValue sometimes
        cmsMLU* displayName = NULL;
        cmsMLU* displayValue = NULL;
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            // Create MLU for display name
            displayName = cmsMLUalloc(context, 1);
            if (displayName != NULL) {
                // Add English/US translation
                cmsMLUsetWide(displayName, "en", "US", (wchar_t*)wname.c_str());
            }
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            // Create MLU for display value
            displayValue = cmsMLUalloc(context, 1);
            if (displayValue != NULL) {
                // Add English/US translation
                cmsMLUsetWide(displayValue, "en", "US", (wchar_t*)wvalue.c_str());
            }
        }
        
        // Add entry to dictionary
        cmsBool add_result = cmsDictAddEntry(
            hDict, 
            wname.c_str(), 
            wvalue.c_str(), 
            displayName, 
            displayValue
        );
        
        // Clean up MLUs if we created them
        if (displayName != NULL) {
            cmsMLUfree(displayName);
        }
        if (displayValue != NULL) {
            cmsMLUfree(displayValue);
        }
        
        // Result not critical for fuzzing
        (void)add_result;
    }
    
    // Phase 3: Test dictionary entry traversal
    
    if (num_entries > 0) {
        // Get entry list
        const cmsDICTentry* entry = cmsDictGetEntryList(hDict);
        
        // Iterate through all entries
        while (entry != NULL) {
            // Access entry fields (they may be NULL)
            if (entry->Name != NULL) {
                size_t name_len = wcslen(entry->Name);
                (void)name_len;
            }
            
            if (entry->Value != NULL) {
                size_t value_len = wcslen(entry->Value);
                (void)value_len;
            }
            
            if (entry->DisplayName != NULL) {
                // Try to get display name in English
                wchar_t buffer[256];
                cmsUInt32Number len = cmsMLUgetWide(entry->DisplayName, "en", "US", buffer, 256);
                (void)len;
            }
            
            if (entry->DisplayValue != NULL) {
                // Try to get display value in English
                wchar_t buffer[256];
                cmsUInt32Number len = cmsMLUgetWide(entry->DisplayValue, "en", "US", buffer, 256);
                (void)len;
            }
            
            // Move to next entry
            entry = cmsDictNextEntry(entry);
        }
    }
    
    // Phase 4: Test dictionary duplication
    
    // Duplicate the dictionary
    cmsHANDLE hDictDup = cmsDictDup(hDict);
    
    if (hDictDup != NULL) {
        // Verify duplication by traversing duplicated dictionary
        const cmsDICTentry* dup_entry = cmsDictGetEntryList(hDictDup);
        
        // Count entries in duplicated dictionary
        cmsUInt32Number dup_count = 0;
        while (dup_entry != NULL) {
            dup_count++;
            dup_entry = cmsDictNextEntry(dup_entry);
        }
        (void)dup_count;
        
        // Add more entries to duplicated dictionary
        if (fdp.remaining_bytes() > 50) {
            std::string extra_name = fdp.ConsumeRandomLengthString(31);
            std::string extra_value = fdp.ConsumeRandomLengthString(63);
            std::wstring wextra_name(extra_name.begin(), extra_name.end());
            std::wstring wextra_value(extra_value.begin(), extra_value.end());
            
            cmsBool extra_result = cmsDictAddEntry(
                hDictDup, 
                wextra_name.c_str(), 
                wextra_value.c_str(), 
                NULL, 
                NULL
            );
            (void)extra_result;
        }
        
        // Free duplicated dictionary
        cmsDictFree(hDictDup);
    }
    
    // Phase 5: Test edge cases and error conditions
    
    // Test with NULL dictionary handle for get entry list
    if (fdp.ConsumeBool()) {
        const cmsDICTentry* null_entry = cmsDictGetEntryList(NULL);
        (void)null_entry;
    }
    
    // Test with NULL dictionary handle for next entry
    if (fdp.ConsumeBool()) {
        const cmsDICTentry* null_next = cmsDictNextEntry(NULL);
        (void)null_next;
    }
    
    // Test adding entry with NULL value and empty name edge cases
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        std::string test_name = fdp.ConsumeRandomLengthString(31);
        std::wstring wtest_name(test_name.begin(), test_name.end());
        
        // Test with NULL value
        cmsBool null_value_result = cmsDictAddEntry(
            hDict, 
            wtest_name.c_str(), 
            NULL, 
            NULL, 
            NULL
        );
        (void)null_value_result;
        
        // Note: NULL Name is not allowed per API contract (_cmsAssert(Name != NULL) in cmsDictAddEntry)
        // Testing with empty string instead to exercise edge cases without violating API
        std::string test_value = fdp.ConsumeRandomLengthString(31);
        std::wstring wtest_value(test_value.begin(), test_value.end());
        std::string empty_name = "";
        std::wstring wempty_name(empty_name.begin(), empty_name.end());
        
        cmsBool empty_name_result = cmsDictAddEntry(
            hDict, 
            wempty_name.c_str(), 
            wtest_value.c_str(), 
            NULL, 
            NULL
        );
        (void)empty_name_result;
    }
    
    // Phase 6: Test dictionary integration with profiles
    
    if (fdp.remaining_bytes() > 500) {
        // Create a simple profile
        cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(context);
        
        if (hProfile != NULL) {
            // Note: There's no direct API to attach dictionary to profile in public API
            // but we can test that dictionary operations don't interfere with profile operations
            
            // Do some profile operations while dictionary exists
            cmsUInt32Number tag_count = cmsGetTagCount(hProfile);
            (void)tag_count;
            
            cmsCloseProfile(hProfile);
        }
    }
    
    // Phase 7: Test multiple allocation/free cycles
    
    cmsUInt32Number num_cycles = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 5);
    
    for (cmsUInt32Number cycle = 0; cycle < num_cycles; cycle++) {
        if (fdp.remaining_bytes() < 30) break;
        
        // Create a temporary dictionary
        cmsHANDLE temp_dict = cmsDictAlloc(context);
        
        if (temp_dict != NULL) {
            // Add a few entries
            cmsUInt32Number temp_entries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
            
            for (cmsUInt32Number j = 0; j < temp_entries; j++) {
                if (fdp.remaining_bytes() < 40) break;
                
                std::string temp_name = fdp.ConsumeRandomLengthString(15);
                std::string temp_value = fdp.ConsumeRandomLengthString(31);
                std::wstring wtemp_name(temp_name.begin(), temp_name.end());
                std::wstring wtemp_value(temp_value.begin(), temp_value.end());
                
                cmsDictAddEntry(temp_dict, wtemp_name.c_str(), wtemp_value.c_str(), NULL, NULL);
            }
            
            // Free temporary dictionary
            cmsDictFree(temp_dict);
        }
    }
    
    // Phase 8: Cleanup
    
    // Free the main dictionary
    cmsDictFree(hDict);
    
    // Delete context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_021
