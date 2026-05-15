#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

namespace harness_012 {
/*
 * Harness 012 - Dictionary Operations Fuzzer for Little CMS 2
 * 
 * Target APIs:
 * - cmsDictAlloc (Target: Initialization, 53+ branches)
 * - cmsDictAddEntry (Target: Add entries to dictionary, 27+ branches)
 * - cmsDictDup (Target: Duplicate dictionary, 18+ branches)
 * - cmsDictNextEntry (Target: Iterate through dictionary entries, 2+ branches)
 * - cmsDictGetEntryList (Target: Get entry list, 2+ branches)
 * - cmsDictFree (Required Helper: Cleanup, 3+ branches)
 * 
 * Invocation sequence:
 * 1. Call cmsDictAlloc to create a new dictionary context
 * 2. Call cmsDictAddEntry multiple times to add key-value pairs to the dictionary
 * 3. Call cmsDictDup to create a duplicate of the dictionary
 * 4. Call cmsDictGetEntryList to retrieve the list of entries
 * 5. Call cmsDictNextEntry to iterate through dictionary entries
 * 6. Call cmsDictFree to clean up both original and duplicated dictionaries
 * 
 * These functions represent 100+ points of undiscovered complexity in the cmsnamed.c module.
 * Dictionary operations are a distinct semantic domain from existing harnesses
 * (MD5, tone curve, profile transforms) and will improve overall API coverage.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for dictionary operations
    // Need at least: context flag + num_entries + some string data
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    bool use_context = fdp.ConsumeBool();
    uint8_t num_entries = fdp.ConsumeIntegralInRange<uint8_t>(1, 10); // Reasonable number of entries
    bool include_display_names = fdp.ConsumeBool();
    
    // Create context if requested
    cmsContext context = use_context ? cmsCreateContext(NULL, NULL) : NULL;
    
    // Allocate dictionary
    cmsHANDLE hDict = cmsDictAlloc(context);
    if (hDict == NULL) {
        if (context != NULL) {
            cmsDeleteContext(context);
        }
        return 0;
    }
    
    // Add entries to the dictionary
    for (uint8_t i = 0; i < num_entries; i++) {
        // Check if we have enough data for this entry
        if (fdp.remaining_bytes() < 4) {
            break;
        }
        
        // Consume string lengths (limited to prevent excessive memory usage)
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        size_t value_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        
        // Check if we have enough data for strings
        if (fdp.remaining_bytes() < name_len + value_len) {
            break;
        }
        
        // Consume name and value as bytes, then convert to wide chars
        std::vector<uint8_t> name_bytes = fdp.ConsumeBytes<uint8_t>(name_len);
        std::vector<uint8_t> value_bytes = fdp.ConsumeBytes<uint8_t>(value_len);
        
        // Create wide character strings (simplified conversion - actual implementation
        // would need proper encoding conversion, but for fuzzing we can create simple wchar arrays)
        // We'll create simple wchar_t arrays with ASCII values for fuzzing purposes
        wchar_t name_wchar[65] = {0};
        wchar_t value_wchar[129] = {0};
        
        // Convert bytes to wide chars (simplified ASCII conversion)
        for (size_t j = 0; j < name_len && j < 64; j++) {
            name_wchar[j] = (wchar_t)(name_bytes[j] & 0x7F); // Keep ASCII range
            if (name_wchar[j] == 0) name_wchar[j] = L'X'; // Avoid null in middle
        }
        name_wchar[64] = 0;
        
        for (size_t j = 0; j < value_len && j < 128; j++) {
            value_wchar[j] = (wchar_t)(value_bytes[j] & 0x7F); // Keep ASCII range
            if (value_wchar[j] == 0) value_wchar[j] = L'Y'; // Avoid null in middle
        }
        value_wchar[128] = 0;
        
        // Create cmsMLU objects for DisplayName and DisplayValue if requested
        cmsMLU* display_name = NULL;
        cmsMLU* display_value = NULL;
        
        if (include_display_names && fdp.remaining_bytes() > 16) {
            // Create simple MLU objects for testing
            display_name = cmsMLUalloc(context, 2);
            if (display_name != NULL) {
                // Add some dummy text to MLU (simplified for fuzzing)
                const char* dummy_text = "DisplayName";
                cmsMLUsetASCII(display_name, "en", "US", dummy_text);
            }
            
            display_value = cmsMLUalloc(context, 2);
            if (display_value != NULL) {
                const char* dummy_text = "DisplayValue";
                cmsMLUsetASCII(display_value, "en", "US", dummy_text);
            }
        }
        
        // Add entry to dictionary
        cmsDictAddEntry(hDict, name_wchar, value_wchar, display_name, display_value);
        
        // Clean up MLU objects
        if (display_name != NULL) {
            cmsMLUfree(display_name);
        }
        if (display_value != NULL) {
            cmsMLUfree(display_value);
        }
    }
    
    // Duplicate the dictionary
    cmsHANDLE hDictDup = cmsDictDup(hDict);
    
    // If duplication succeeded, test entry list iteration
    if (hDictDup != NULL) {
        // Get entry list from original dictionary
        const cmsDICTentry* entry_list = cmsDictGetEntryList(hDict);
        
        // Iterate through entries using cmsDictNextEntry
        const cmsDICTentry* current = entry_list;
        while (current != NULL) {
            // Access entry fields (read-only, just to exercise the API)
            // current->Name, current->Value, current->DisplayName, current->DisplayValue
            current = cmsDictNextEntry(current);
        }
        
        // Also iterate through duplicated dictionary
        const cmsDICTentry* dup_entry_list = cmsDictGetEntryList(hDictDup);
        const cmsDICTentry* dup_current = dup_entry_list;
        while (dup_current != NULL) {
            dup_current = cmsDictNextEntry(dup_current);
        }
        
        // Free the duplicated dictionary
        cmsDictFree(hDictDup);
    }
    
    // Free the original dictionary
    cmsDictFree(hDict);
    
    // Delete context if we created one
    if (context != NULL) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_012
