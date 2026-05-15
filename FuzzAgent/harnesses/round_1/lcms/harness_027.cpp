#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_027 {
// Fuzzing harness for Little CMS 2 library
// Target: Multi-Language Unicode (MLU) functions identified as critical coverage gaps
// Strategy: Specifically target 4 completely uncovered MLU functions in cmsnamed.c with 14 total undiscovered branches:
//           cmsMLUgetTranslation, cmsMLUtranslationsCodes, cmsMLUtranslationsCount, and cmsMLUsetUTF8
// Required helpers: cmsMLUalloc, cmsMLUfree, cmsMLUsetWide for comparison, cmsCreateContext for THR operations
// Semantic diversity from existing harnesses:
// - harness_006: Uses cmsMLUsetWide and cmsMLUalloc for profile descriptions
// - harness_010: Creates random MLU for dictionary entries using cmsMLUsetWide
// - This harness (027): First harness to specifically target MLU query/translation functions (getTranslation, translationsCodes, translationsCount)
//                      and UTF8-specific API (cmsMLUsetUTF8), focusing on comprehensive MLU lifecycle testing
// Invocation sequence:
//   1. Create MLU object with initial capacity
//   2. Populate with translations using both UTF8 and wide string APIs
//   3. Test cmsMLUtranslationsCount to get total translation count
//   4. Iterate through translations using cmsMLUtranslationsCodes
//   5. Test cmsMLUgetTranslation for language/country code mapping
//   6. Retrieve translations in different formats (ASCII, Wide, UTF8)
//   7. Test edge cases: NULL pointers, empty MLU, invalid indices
//   8. Clean up MLU object


// Helper function to convert ASCII string to wide string for cmsMLUsetWide
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

// Helper to generate valid 2-char language/country codes from fuzzer input
static std::string get_valid_code(FuzzedDataProvider& fdp) {
    std::string code = fdp.ConsumeBytesAsString(2);
    if (code.length() < 2) {
        // Pad with 'a' if we don't have enough bytes
        code = "aa";
    }
    return code;
}

// Helper to generate UTF8 string (can include multibyte characters)
static std::string get_utf8_string(FuzzedDataProvider& fdp) {
    // Consume random length string for UTF8 testing
    // UTF8 can include various byte patterns that cmsMLUsetUTF8 should handle
    size_t max_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    return fdp.ConsumeRandomLengthString(max_len);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for MLU operations
    // We need: operation selection, language/country codes, string data, etc.
    const size_t MIN_SIZE = sizeof(uint8_t) + 4 * 2 + 16; // op + 4 codes + minimal strings
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine test mode from fuzzer input
    // 0: Basic MLU lifecycle with mixed UTF8/Wide strings
    // 1: Focus on translation query functions (getTranslation, translationsCodes)
    // 2: Edge case testing (NULL pointers, empty strings, invalid indices)
    // 3: Comprehensive mixed testing
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 2: Create context and MLU object
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        // Use default context if context creation fails
        context = NULL;
    }
    
    // Determine MLU initial capacity
    uint8_t initial_capacity = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    cmsMLU* mlu = cmsMLUalloc(context, initial_capacity);
    if (mlu == NULL) {
        if (context) cmsDeleteContext(context);
        return 0; // Cannot proceed without MLU
    }
    
    // Step 3: Determine number of translations to add
    uint8_t num_translations = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    
    // Arrays to store added language/country codes for later querying
    std::vector<std::string> added_langs;
    std::vector<std::string> added_countries;
    
    // Step 4: Populate MLU with translations using both UTF8 and Wide APIs
    for (uint8_t i = 0; i < num_translations; i++) {
        // Generate language and country codes
        std::string lang = get_valid_code(fdp);
        std::string country = get_valid_code(fdp);
        
        // Store for later querying
        added_langs.push_back(lang);
        added_countries.push_back(country);
        
        // Decide which API to use for this translation
        bool use_utf8 = fdp.ConsumeBool();
        
        if (use_utf8) {
            // Target: cmsMLUsetUTF8 (uncovered function)
            std::string utf8_str = get_utf8_string(fdp);
            cmsMLUsetUTF8(mlu, lang.c_str(), country.c_str(), utf8_str.c_str());
        } else {
            // Use cmsMLUsetWide for comparison (already covered in other harnesses)
            std::string ascii_str = fdp.ConsumeRandomLengthString(64);
            wchar_t* wstr = ascii_to_wstr(ascii_str.c_str());
            if (wstr) {
                cmsMLUsetWide(mlu, lang.c_str(), country.c_str(), wstr);
                free_wstr(wstr);
            }
        }
        
        // Check if we have enough input left
        if (fdp.remaining_bytes() < 10) break;
    }
    
    // Step 5: Test cmsMLUtranslationsCount (uncovered function)
    // This should return the number of translations we added
    cmsUInt32Number translation_count = cmsMLUtranslationsCount(mlu);
    
    // Step 6: Test cmsMLUtranslationsCodes (uncovered function)
    // Iterate through all translations and get their language/country codes
    for (cmsUInt32Number idx = 0; idx < translation_count; idx++) {
        char lang_code[3] = {0};
        char country_code[3] = {0};
        
        cmsBool success = cmsMLUtranslationsCodes(mlu, idx, lang_code, country_code);
        // success should be TRUE for valid indices
        
        // Test edge case: index beyond valid range
        if (idx == 0 && translation_count > 0) {
            char invalid_lang[3] = {0};
            char invalid_country[3] = {0};
            cmsBool should_fail = cmsMLUtranslationsCodes(mlu, translation_count + 10, invalid_lang, invalid_country);
            // should_fail should be FALSE
        }
    }
    
    // Step 7: Test cmsMLUgetTranslation (uncovered function)
    // Try to find translations for some of the languages we added
    if (!added_langs.empty() && !added_countries.empty()) {
        for (size_t i = 0; i < std::min(added_langs.size(), added_countries.size()); i++) {
            char obtained_lang[3] = {0};
            char obtained_country[3] = {0};
            
            cmsBool found = cmsMLUgetTranslation(mlu, 
                                                 added_langs[i].c_str(), 
                                                 added_countries[i].c_str(),
                                                 obtained_lang, 
                                                 obtained_country);
            // found should be TRUE if the translation exists
            
            // Also test with random language/country codes that might not exist
            if (i == 0) {
                char random_lang[3] = {0};
                char random_country[3] = {0};
                std::string test_lang = get_valid_code(fdp);
                std::string test_country = get_valid_code(fdp);
                
                cmsBool random_found = cmsMLUgetTranslation(mlu,
                                                           test_lang.c_str(),
                                                           test_country.c_str(),
                                                           random_lang,
                                                           random_country);
                // random_found may be FALSE if no translation exists
            }
        }
    }
    
    // Step 8: Test retrieval functions for different formats
    // This exercises code paths that may call the uncovered functions internally
    if (translation_count > 0 && !added_langs.empty() && !added_countries.empty()) {
        // Test ASCII retrieval
        char ascii_buffer[256];
        cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, 
                                                   added_langs[0].c_str(),
                                                   added_countries[0].c_str(),
                                                   ascii_buffer, 
                                                   sizeof(ascii_buffer));
        
        // Test Wide retrieval
        wchar_t wide_buffer[128];
        cmsUInt32Number wide_len = cmsMLUgetWide(mlu,
                                                added_langs[0].c_str(),
                                                added_countries[0].c_str(),
                                                wide_buffer,
                                                sizeof(wide_buffer) / sizeof(wchar_t));
        
        // Test UTF8 retrieval
        char utf8_buffer[256];
        cmsUInt32Number utf8_len = cmsMLUgetUTF8(mlu,
                                                added_langs[0].c_str(),
                                                added_countries[0].c_str(),
                                                utf8_buffer,
                                                sizeof(utf8_buffer));
    }
    
    // Step 9: Test edge cases based on test_mode
    if (test_mode == 2 || test_mode == 3) {
        // Test with NULL MLU pointer
        cmsUInt32Number null_count = cmsMLUtranslationsCount(NULL);
        // null_count should be 0
        
        char null_lang[3] = {0};
        char null_country[3] = {0};
        cmsBool null_success = cmsMLUtranslationsCodes(NULL, 0, null_lang, null_country);
        // null_success should be FALSE
        
        cmsBool null_translation = cmsMLUgetTranslation(NULL, "en", "US", null_lang, null_country);
        // null_translation should be FALSE
        
        // Test cmsMLUsetUTF8 with empty string (special case in implementation)
        cmsMLU* empty_mlu = cmsMLUalloc(context, 1);
        if (empty_mlu) {
            cmsMLUsetUTF8(empty_mlu, "en", "US", ""); // Empty string special case
            cmsMLUfree(empty_mlu);
        }
    }
    
    // Step 10: Clean up
    cmsMLUfree(mlu);
    if (context) cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_027
