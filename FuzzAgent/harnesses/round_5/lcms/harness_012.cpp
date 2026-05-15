#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "lcms2.h"

namespace harness_012 {
// Fuzzing harness for Little CMS (lcms) library
// Target: Dictionary management functions (cmsnamed.c module)
// Focus: cmsDictAlloc, cmsDictAddEntry, cmsDictDup, cmsDictGetEntryList, cmsDictNextEntry, cmsDictFree
// Coverage goal: Target completely uncovered dictionary module (100+ undiscovered complexity points)
// Lifecycle sequence: Dictionary allocation -> Entry addition with cmsMLU objects -> Dictionary duplication -> Entry list retrieval -> Iteration through entries -> Proper cleanup
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
//   harness_009.cpp - Date/time handling
//   harness_010.cpp - Stream-based profile I/O
//   harness_011.cpp - MD5 checksum functions
// New coverage: Dictionary management functionality for ICC profile metadata storage


// Helper function to convert ASCII string to wide string for dictionary entries
static wchar_t* ascii_to_wstr(const char* ascii_str) {
    if (!ascii_str) return NULL;
    
    size_t len = strlen(ascii_str);
    wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (wchar_t)(unsigned char)ascii_str[i];
    }
    wstr[len] = 0;
    return wstr;
}

// Helper function to create cmsMLU object with ASCII content
static cmsMLU* create_test_mlu(cmsContext ctx, const char* text, const char* language = "en", const char* country = "US") {
    cmsMLU* mlu = cmsMLUalloc(ctx, 2); // Allocate with 2 entries
    if (!mlu) return NULL;
    
    // Set ASCII content
    if (!cmsMLUsetASCII(mlu, language, country, text)) {
        cmsMLUfree(mlu);
        return NULL;
    }
    
    return mlu;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for dictionary operations
    // We need: context creation + at least one dictionary entry with basic strings
    const size_t MIN_SIZE = 64;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // Step 2: Allocate dictionary
    cmsHANDLE hDict = cmsDictAlloc(ctx);
    if (hDict == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Step 3: Determine number of entries to add (0-10)
    uint8_t num_entries = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
    // Store created MLU objects for cleanup
    std::vector<cmsMLU*> mlu_objects;
    std::vector<wchar_t*> wchar_strings;
    
    // Step 4: Add entries to dictionary
    for (uint8_t i = 0; i < num_entries; i++) {
        // Consume strings for entry name and value
        std::string name_str = fdp.ConsumeRandomLengthString(64);
        std::string value_str = fdp.ConsumeRandomLengthString(128);
        
        // Convert to wide strings
        wchar_t* name_wstr = ascii_to_wstr(name_str.c_str());
        wchar_t* value_wstr = ascii_to_wstr(value_str.c_str());
        
        if (!name_wstr || !value_wstr) {
            free(name_wstr);
            free(value_wstr);
            continue; // Skip this entry if conversion fails
        }
        
        // Create optional display name and display value MLU objects
        cmsMLU* display_name_mlu = NULL;
        cmsMLU* display_value_mlu = NULL;
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
            std::string display_name_str = fdp.ConsumeRandomLengthString(32);
            display_name_mlu = create_test_mlu(ctx, display_name_str.c_str());
            if (display_name_mlu) {
                mlu_objects.push_back(display_name_mlu);
            }
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
            std::string display_value_str = fdp.ConsumeRandomLengthString(32);
            display_value_mlu = create_test_mlu(ctx, display_value_str.c_str());
            if (display_value_mlu) {
                mlu_objects.push_back(display_value_mlu);
            }
        }
        
        // Add entry to dictionary
        cmsBool add_result = cmsDictAddEntry(hDict, name_wstr, value_wstr, 
                                           display_name_mlu, display_value_mlu);
        
        // Store wide strings for cleanup
        wchar_strings.push_back(name_wstr);
        wchar_strings.push_back(value_wstr);
        
        // Test edge cases
        if (fdp.ConsumeBool()) {
            // Test with NULL name (should fail assertion)
            // cmsDictAddEntry(hDict, NULL, value_wstr, NULL, NULL);
            
            // Test with empty strings
            wchar_t empty_str[] = L"";
            cmsDictAddEntry(hDict, empty_str, empty_str, NULL, NULL);
        }
    }
    
    // Step 5: Test dictionary duplication
    if (fdp.ConsumeBool()) {
        cmsHANDLE hDictCopy = cmsDictDup(hDict);
        if (hDictCopy != NULL) {
            // Test that duplicate has same entries
            // (Can't compare directly, but we can iterate)
            
            // Clean up duplicate
            cmsDictFree(hDictCopy);
        }
    }
    
    // Step 6: Test entry list retrieval and iteration
    const cmsDICTentry* entry_list = cmsDictGetEntryList(hDict);
    if (entry_list != NULL) {
        // Iterate through all entries
        const cmsDICTentry* current = entry_list;
        int entry_count = 0;
        
        while (current != NULL && entry_count < 100) { // Safety limit
            // Access entry fields
            wchar_t* name = current->Name;
            wchar_t* value = current->Value;
            cmsMLU* display_name = current->DisplayName;
            cmsMLU* display_value = current->DisplayValue;
            
            // Test MLU access if present
            if (display_name != NULL) {
                char buffer[256];
                cmsUInt32Number bytes_needed = cmsMLUgetASCII(display_name, "en", "US", buffer, sizeof(buffer));
                // bytes_needed includes null terminator
            }
            
            if (display_value != NULL) {
                char buffer[256];
                cmsUInt32Number bytes_needed = cmsMLUgetASCII(display_value, "en", "US", buffer, sizeof(buffer));
            }
            
            // Move to next entry
            current = cmsDictNextEntry(current);
            entry_count++;
        }
    }
    
    // Step 7: Test dictionary operations with different scenarios
    
    // Create a second dictionary with different content
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
        cmsHANDLE hDict2 = cmsDictAlloc(ctx);
        if (hDict2 != NULL) {
            // Add some entries with only ASCII content (no MLU)
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
                std::string name2 = fdp.ConsumeRandomLengthString(16);
                std::string value2 = fdp.ConsumeRandomLengthString(32);
                
                wchar_t* name_wstr2 = ascii_to_wstr(name2.c_str());
                wchar_t* value_wstr2 = ascii_to_wstr(value2.c_str());
                
                if (name_wstr2 && value_wstr2) {
                    cmsDictAddEntry(hDict2, name_wstr2, value_wstr2, NULL, NULL);
                    
                    free(name_wstr2);
                    free(value_wstr2);
                }
            }
            
            // Duplicate and free
            cmsHANDLE hDict2Copy = cmsDictDup(hDict2);
            if (hDict2Copy != NULL) {
                cmsDictFree(hDict2Copy);
            }
            
            cmsDictFree(hDict2);
        }
    }
    
    // Step 8: Test with profile integration (optional)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 128) {
        // Create a simple sRGB profile
        cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
        if (hProfile != NULL) {
            // Note: Dictionary functions are standalone, not directly profile-related
            // but we can test context consistency
            
            // Get profile context ID
            cmsContext profile_ctx = cmsGetProfileContextID(hProfile);
            
            // Create dictionary using profile context
            cmsHANDLE hProfileDict = cmsDictAlloc(profile_ctx);
            if (hProfileDict != NULL) {
                // Add a simple entry
                wchar_t profile_name[] = L"ProfileEntry";
                wchar_t profile_value[] = L"TestValue";
                cmsDictAddEntry(hProfileDict, profile_name, profile_value, NULL, NULL);
                
                cmsDictFree(hProfileDict);
            }
            
            cmsCloseProfile(hProfile);
        }
    }
    
    // Step 9: Cleanup
    
    // Free dictionary (this will free all internal resources including MLU objects)
    cmsDictFree(hDict);
    
    // Free our allocated wide strings
    for (wchar_t* wstr : wchar_strings) {
        free(wstr);
    }
    
    // Free MLU objects (those NOT added to dictionary)
    for (cmsMLU* mlu : mlu_objects) {
        cmsMLUfree(mlu);
    }
    
    // Delete context
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_012
