#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_010 {
// Fuzzing harness for Little CMS 2 library
// Target: Dictionary manipulation functions with high uncovered complexity
// Strategy: Target cmsDictDup (0% coverage with 37 undiscovered branches) and related dictionary functions
//           with 57+ undiscovered branches total. Use FuzzedDataProvider for comprehensive input splitting.
// APIs targeted: cmsDictDup (primary), cmsDictAddEntry, cmsDictGetEntryList, cmsDictNextEntry, cmsDictAlloc, cmsDictFree


// Helper function to convert ASCII string to wide string
static wchar_t* ascii_to_wstr(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (wchar_t)str[i];
    }
    wstr[len] = 0;
    return wstr;
}

// Helper function to free wide string
static void free_wstr(wchar_t* wstr) {
    if (wstr) free(wstr);
}

// Helper to create random cmsMLU for display names/values
static cmsMLU* create_random_mlu(cmsContext ctx, FuzzedDataProvider& fdp) {
    cmsMLU* mlu = cmsMLUalloc(ctx, 0);
    if (!mlu) return NULL;
    
    // Add some random translations
    uint8_t num_translations = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
    for (uint8_t i = 0; i < num_translations; i++) {
        std::string lang = fdp.ConsumeBytesAsString(2);
        std::string country = fdp.ConsumeBytesAsString(2);
        std::string text_str = fdp.ConsumeRandomLengthString(32);
        wchar_t* wtext = ascii_to_wstr(text_str.c_str());
        if (wtext) {
            cmsMLUsetWide(mlu, lang.c_str(), country.c_str(), wtext);
            free_wstr(wtext);
        }
    }
    
    return mlu;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size: need at least some bytes for basic operations
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    cmsContext ctx = NULL; // Use default context
    
    // Step 1: Determine dictionary size and configuration
    uint8_t dict_size = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    bool use_mlu = fdp.ConsumeBool();
    bool test_empty_dict = (dict_size == 0);
    
    // Step 2: Create dictionary with cmsDictAlloc
    cmsHANDLE hDict1 = cmsDictAlloc(ctx);
    if (!hDict1) return 0;
    
    // Step 3: Populate dictionary with entries using cmsDictAddEntry
    for (uint8_t i = 0; i < dict_size; i++) {
        // Consume key and value strings
        std::string key_str = fdp.ConsumeRandomLengthString(32);
        std::string value_str = fdp.ConsumeRandomLengthString(64);
        
        wchar_t* wkey = ascii_to_wstr(key_str.c_str());
        wchar_t* wvalue = ascii_to_wstr(value_str.c_str());
        
        cmsMLU* display_name = NULL;
        cmsMLU* display_value = NULL;
        
        if (use_mlu && fdp.ConsumeBool()) {
            display_name = create_random_mlu(ctx, fdp);
        }
        if (use_mlu && fdp.ConsumeBool()) {
            display_value = create_random_mlu(ctx, fdp);
        }
        
        // Add entry to dictionary
        cmsBool success = cmsDictAddEntry(hDict1, wkey, wvalue, display_name, display_value);
        // Note: We don't check success here to allow testing error paths
        
        // Clean up
        free_wstr(wkey);
        free_wstr(wvalue);
        if (display_name) cmsMLUfree(display_name);
        if (display_value) cmsMLUfree(display_value);
        
        // Check if we have enough input left
        if (fdp.remaining_bytes() < 16) break;
    }
    
    // Step 4: Get entry list with cmsDictGetEntryList
    const cmsDICTentry* entry_list = cmsDictGetEntryList(hDict1);
    
    // Step 5: Iterate through entries with cmsDictNextEntry
    if (entry_list) {
        const cmsDICTentry* current = entry_list;
        int iteration_count = 0;
        while (current && iteration_count < 100) { // Safety limit
            // Access entry fields (read-only)
            const wchar_t* name = current->Name;
            const wchar_t* value = current->Value;
            const cmsMLU* display_name = current->DisplayName;
            const cmsMLU* display_value = current->DisplayValue;
            
            // Just touch the data to ensure coverage
            (void)name;
            (void)value;
            (void)display_name;
            (void)display_value;
            
            current = cmsDictNextEntry(current);
            iteration_count++;
        }
    }
    
    // Step 6: Duplicate dictionary with cmsDictDup (primary target)
    cmsHANDLE hDict2 = cmsDictDup(hDict1);
    
    // If duplication succeeded, test the duplicated dictionary
    if (hDict2) {
        // Get entry list from duplicated dictionary
        const cmsDICTentry* dup_entry_list = cmsDictGetEntryList(hDict2);
        
        // Iterate through duplicated dictionary entries
        if (dup_entry_list) {
            const cmsDICTentry* current = dup_entry_list;
            int iteration_count = 0;
            while (current && iteration_count < 100) {
                current = cmsDictNextEntry(current);
                iteration_count++;
            }
        }
        
        // Test edge case: duplicate an empty dictionary
        if (test_empty_dict) {
            cmsHANDLE hEmptyDict = cmsDictAlloc(ctx);
            if (hEmptyDict) {
                cmsHANDLE hEmptyDup = cmsDictDup(hEmptyDict);
                if (hEmptyDup) {
                    cmsDictFree(hEmptyDup);
                }
                cmsDictFree(hEmptyDict);
            }
        }
        
        // Step 7: Add more entries to duplicated dictionary
        if (!test_empty_dict && fdp.remaining_bytes() > 16) {
            std::string extra_key = fdp.ConsumeRandomLengthString(16);
            std::string extra_value = fdp.ConsumeRandomLengthString(32);
            wchar_t* wextra_key = ascii_to_wstr(extra_key.c_str());
            wchar_t* wextra_value = ascii_to_wstr(extra_value.c_str());
            
            if (wextra_key && wextra_value) {
                cmsDictAddEntry(hDict2, wextra_key, wextra_value, NULL, NULL);
            }
            
            free_wstr(wextra_key);
            free_wstr(wextra_value);
        }
        
        // Step 8: Free duplicated dictionary
        cmsDictFree(hDict2);
    }
    
    // Step 9: Test additional dictionary operations
    
    // Test with NULL values for cmsDictAddEntry (using fuzzer input)
    if (fdp.ConsumeBool() && !test_empty_dict && fdp.remaining_bytes() > 8) {
        std::string null_test_key_str = fdp.ConsumeRandomLengthString(16);
        wchar_t* null_test_key = ascii_to_wstr(null_test_key_str.c_str());
        if (null_test_key) {
            cmsDictAddEntry(hDict1, null_test_key, NULL, NULL, NULL);
            free_wstr(null_test_key);
        }
    }
    
    // Test with empty string values (using fuzzer input)
    if (fdp.ConsumeBool() && !test_empty_dict && fdp.remaining_bytes() > 8) {
        std::string empty_test_key_str = fdp.ConsumeRandomLengthString(16);
        wchar_t* empty_test_key = ascii_to_wstr(empty_test_key_str.c_str());
        wchar_t empty_value[1] = {0};
        if (empty_test_key) {
            cmsDictAddEntry(hDict1, empty_test_key, empty_value, NULL, NULL);
            free_wstr(empty_test_key);
        }
    }
    
    // Step 10: Free original dictionary
    cmsDictFree(hDict1);
    
    return 0;
}

} // namespace harness_010
