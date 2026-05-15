#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_006 {
// Fuzzing harness for LittleCMS (lcms) dictionary management functions
// This harness specifically targets dictionary management functions from cmsnamed.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsDictAlloc, cmsDictAddEntry, cmsDictGetEntryList, 
// cmsDictNextEntry, cmsDictDup, cmsDictFree
// MODULE: cmsnamed.c [current coverage: 0%]
// STRATEGY: Target dictionary creation, manipulation, and query operations lifecycle
// DIFFERENTIATION: Unlike existing harnesses focused on transforms, PostScript, virtual profiles, I/O operations, and IT8 handling, target dictionary management for tag/value storage


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful dictionary operations
    // Minimum: context + dictionary allocation + at least one entry with MLU strings
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize Context: Create a working context for dictionary operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Create Dictionary: Allocate a new dictionary
    cmsHANDLE hDict = cmsDictAlloc(context);
    if (hDict == NULL) {
        cmsDeleteContext(context);
        return 0; // Cannot allocate dictionary
    }
    
    // 3. Add Entries: Add multiple dictionary entries with different key/value pairs
    // Determine number of entries to add (1-5 to keep it reasonable)
    size_t num_entries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    std::vector<cmsMLU*> mlu_names;
    std::vector<cmsMLU*> mlu_values;
    
    for (size_t i = 0; i < num_entries && fdp.remaining_bytes() > 32; i++) {
        // Create MLU strings for DisplayName and DisplayValue
        cmsMLU* display_name = cmsMLUalloc(context, 1); // 1 item
        cmsMLU* display_value = cmsMLUalloc(context, 1);
        
        if (display_name != NULL && display_value != NULL) {
            // Add English entries to MLU strings
            std::string name_str = fdp.ConsumeRandomLengthString(64);
            std::string value_str = fdp.ConsumeRandomLengthString(128);
            
            // Convert to wide strings for dictionary API
            std::wstring wname(name_str.begin(), name_str.end());
            std::wstring wvalue(value_str.begin(), value_str.end());
            
            // Set MLU entries (simplified - real usage would set language/country codes)
            // For fuzzing purposes, we'll just set the strings
            if (wname.length() > 0 && wvalue.length() > 0) {
                // Store MLU objects for later cleanup
                mlu_names.push_back(display_name);
                mlu_values.push_back(display_value);
                
                // Add entry to dictionary
                cmsBool add_result = cmsDictAddEntry(hDict, 
                                                    wname.c_str(), 
                                                    wvalue.c_str(),
                                                    display_name,
                                                    display_value);
                // Don't check result - fuzzer should explore both success and failure paths
            } else {
                // Clean up MLU objects if we can't use them
                cmsMLUfree(display_name);
                cmsMLUfree(display_value);
            }
        } else {
            // Clean up any allocated MLU objects
            if (display_name) cmsMLUfree(display_name);
            if (display_value) cmsMLUfree(display_value);
        }
    }
    
    // 4. Query Dictionary: Get entry list and iterate through entries
    const cmsDICTentry* entry_list = cmsDictGetEntryList(hDict);
    if (entry_list != NULL) {
        // Iterate through all entries
        const cmsDICTentry* current = entry_list;
        while (current != NULL) {
            // Access entry fields (Name, Value, DisplayName, DisplayValue)
            // Just accessing them is enough for coverage
            current = cmsDictNextEntry(current);
        }
    }
    
    // 5. Duplicate Dictionary: Test dictionary duplication
    cmsHANDLE hDictDup = cmsDictDup(hDict);
    if (hDictDup != NULL) {
        // Query the duplicated dictionary as well
        const cmsDICTentry* dup_entry_list = cmsDictGetEntryList(hDictDup);
        if (dup_entry_list != NULL) {
            const cmsDICTentry* dup_current = dup_entry_list;
            while (dup_current != NULL) {
                dup_current = cmsDictNextEntry(dup_current);
            }
        }
        
        // Free the duplicated dictionary
        cmsDictFree(hDictDup);
    }
    
    // 6. Cleanup:
    // Free the original dictionary
    cmsDictFree(hDict);
    
    // Free MLU objects
    for (cmsMLU* mlu : mlu_names) {
        cmsMLUfree(mlu);
    }
    for (cmsMLU* mlu : mlu_values) {
        cmsMLUfree(mlu);
    }
    
    // Delete context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_006
