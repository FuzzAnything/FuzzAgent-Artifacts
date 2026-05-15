#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_007 {
// Targeted liblouis fuzzing harness for remaining uncovered utility APIs and high-value hyphenation
// Harness_007: Focuses on lou_version, lou_getDataPath, lou_freeEmphClasses, lou_charSize, lou_freeTableFile (all 0% coverage)
//              Also targets lou_hyphenate (11.54% coverage, 47 undiscovered branches), 
//              lou_dotsToChar (66.67% coverage), lou_charToDots (71.43% coverage)
// Differentiation from harness_006: Completes API coverage (91.18% → 100%), implements exact invocation sequence 
//                                   from coverage guidance, tests utility/cleanup functions with proper lifecycle
// Strategy: Follow prescribed 13-step invocation sequence with fuzzer-driven variations for maximum coverage




// Log callback to suppress output during fuzzing
static void fuzz_log_callback(logLevels level, const char* message) {
    (void)level;
    (void)message;
    // Suppress all logging during fuzzing
}

// Convert std::string to widechar buffer (simple ASCII/Latin-1 conversion)
static widechar* string_to_widechar(const std::string& str, int* out_len) {
    if (str.empty()) {
        *out_len = 0;
        return nullptr;
    }
    
    widechar* wbuf = (widechar*)malloc((str.size() + 1) * sizeof(widechar));
    if (!wbuf) {
        *out_len = 0;
        return nullptr;
    }
    
    for (size_t i = 0; i < str.size(); i++) {
        wbuf[i] = (widechar)(unsigned char)str[i];
    }
    wbuf[str.size()] = 0;
    *out_len = (int)str.size();
    return wbuf;
}

// Generate random table queries for dynamic table discovery
static std::string generate_table_query(FuzzedDataProvider& fdp) {
    std::string query;
    
    // Choose query type based on fuzzer input
    uint8_t query_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (query_type) {
        case 0:
            query = "en-us-g2.ctb";  // Specific table from guidance
            break;
        case 1:
            query = "en";  // English tables
            break;
        case 2:
            query = "g2";  // Grade 2 tables
            break;
        case 3:
            query = "us";  // US tables
            break;
        case 4:
            query = "comp"; // Computer braille
            break;
        case 5:
            // Random string query
            if (fdp.remaining_bytes() > 1) {
                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                query = fdp.ConsumeBytesAsString(len);
            } else {
                query = "en-us-g2.ctb";
            }
            break;
    }
    
    return query;
}

// Generate random text for testing hyphenation and conversions
static std::string generate_test_text(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 4) {
        return "test";
    }
    
    uint8_t text_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (text_type) {
        case 0:
            // Random ASCII text
            return fdp.ConsumeRandomLengthString(256);
        case 1:
            // Short text for edge cases
            return fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(0, 10));
        case 2:
            // Braille dot patterns (simplified)
            return fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
        case 3:
            // Mixed content
            return fdp.ConsumeRemainingBytesAsString();
    }
    
    return "testing";
}

// Generate hyphen array for hyphenation testing
static char* generate_hyphen_array(FuzzedDataProvider& fdp, int length) {
    if (length <= 0) return nullptr;
    
    char* hyphens = (char*)malloc(length * sizeof(char));
    if (!hyphens) return nullptr;
    
    for (int i = 0; i < length; i++) {
        // Generate valid hyphen characters: '0', '1', or other valid hyphen markers
        uint8_t hyphen_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (hyphen_type) {
            case 0:
                hyphens[i] = '0';  // No hyphen
                break;
            case 1:
                hyphens[i] = '1';  // Hyphen
                break;
            case 2:
                hyphens[i] = fdp.ConsumeIntegralInRange<char>('0', '1');
                break;
        }
    }
    
    return hyphens;
}

// Generate random data path for lou_setDataPath
static std::string generate_data_path(FuzzedDataProvider& fdp) {
    uint8_t path_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (path_type) {
        case 0:
            return "/usr/share/liblouis";  // Default from guidance
        case 1:
            return "/tmp";
        case 2:
            return "/var/lib/liblouis";
        case 3:
            // Random path
            if (fdp.remaining_bytes() > 4) {
                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                return fdp.ConsumeBytesAsString(len);
            }
            break;
    }
    
    return "/usr/share/liblouis";
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Register log callback to suppress output
    lou_registerLogCallback(fuzz_log_callback);
    lou_setLogLevel(LOU_LOG_ERROR);  // Only log errors
    
    // === STEP 1: lou_version() ===
    const char* version = lou_version();
    (void)version;  // Use version string to avoid unused variable warning
    
    // === STEP 2: lou_setDataPath() ===
    std::string data_path = generate_data_path(fdp);
    char* set_path_result = lou_setDataPath(data_path.c_str());
    (void)set_path_result;  // Result may be NULL for invalid paths
    
    // === STEP 3: lou_getDataPath() ===
    char* retrieved_path = lou_getDataPath();
    (void)retrieved_path;  // Use retrieved path
    
    // === STEP 4: lou_findTable() or lou_getTable() ===
    char* found_table = nullptr;
    const void* loaded_table = nullptr;
    
    // Choose between findTable and getTable based on fuzzer input
    bool use_find_table = fdp.ConsumeBool();
    std::string table_query = generate_table_query(fdp);
    
    if (use_find_table) {
        // Use lou_findTable for dynamic table discovery
        found_table = lou_findTable(table_query.c_str());
    } else {
        // Use lou_getTable for direct table loading
        loaded_table = lou_getTable(table_query.c_str());
    }
    
    // Determine which table to use for subsequent operations
    const char* table_to_use = nullptr;
    if (found_table) {
        table_to_use = found_table;
    } else if (loaded_table) {
        // For lou_getTable, we need the table list string
        table_to_use = table_query.c_str();
    } else {
        // If both failed, use a default table
        table_to_use = "en-us-g2.ctb";
    }
    
    // === STEP 5: lou_charSize() ===
    int char_size = lou_charSize();
    (void)char_size;  // Use character size
    
    // === STEP 6: lou_getEmphClasses() ===
    char const** emph_classes = lou_getEmphClasses(table_to_use);
    
    // === STEP 7: lou_freeEmphClasses() ===
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // === STEP 8: lou_getTableInfo() ===
    char* table_info = nullptr;
    if (fdp.remaining_bytes() > 4) {
        // Choose metadata key to query
        uint8_t key_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        const char* key = nullptr;
        
        switch (key_choice) {
            case 0:
                key = "language";
                break;
            case 1:
                key = "locale";
                break;
            case 2:
                key = "type";
                break;
            case 3:
                // Random key
                if (fdp.remaining_bytes() > 4) {
                    size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    std::string random_key = fdp.ConsumeBytesAsString(key_len);
                    // Use a static buffer since we need const char*
                    static char static_key[32];
                    strncpy(static_key, random_key.c_str(), sizeof(static_key) - 1);
                    static_key[sizeof(static_key) - 1] = '\0';
                    key = static_key;
                } else {
                    key = "language";
                }
                break;
        }
        
        if (key) {
            table_info = lou_getTableInfo(table_to_use, key);
        }
    }
    
    // === STEP 9: lou_freeTableFile() ===
    if (found_table) {
        lou_freeTableFile(found_table);
        found_table = nullptr;
    }
    
    // Also free table info if we got it
    if (table_info) {
        lou_freeTableInfo(table_info);
        table_info = nullptr;
    }
    
    // === STEP 10: lou_hyphenate() ===
    // Perform hyphenation testing with variations
    if (fdp.remaining_bytes() > 8) {
        int hyphenation_ops = fdp.ConsumeIntegralInRange<int>(1, 5);
        
        for (int i = 0; i < hyphenation_ops && fdp.remaining_bytes() > 4; i++) {
            std::string text_str = generate_test_text(fdp);
            if (text_str.empty()) {
                continue;
            }
            
            int text_len = 0;
            widechar* text_buf = string_to_widechar(text_str, &text_len);
            if (!text_buf || text_len == 0) {
                free(text_buf);
                continue;
            }
            
            // Choose hyphenation mode
            int mode = fdp.ConsumeIntegral<int>();
            
            // Apply common mode flags based on fuzzer input
            if (fdp.ConsumeBool()) mode |= compbrlAtCursor;
            if (fdp.ConsumeBool()) mode |= dotsIO;
            if (fdp.ConsumeBool()) mode |= noUndefined;
            if (fdp.ConsumeBool()) mode |= partialTrans;
            
            // Create hyphen array
            char* hyphens = (char*)malloc((text_len + 1) * sizeof(char));
            if (hyphens) {
                // Test hyphenation
                int result = lou_hyphenate(table_to_use, text_buf, text_len, hyphens, mode);
                (void)result;  // Use result
                
                free(hyphens);
            }
            
            free(text_buf);
        }
    }
    
    // === STEP 11: lou_dotsToChar() ===
    // Test dots to character conversion
    if (fdp.remaining_bytes() > 16) {
        size_t conv_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::vector<widechar> inbuf(conv_len);
        std::vector<widechar> outbuf(conv_len);
        
        // Fill input buffer with random dot patterns
        for (size_t i = 0; i < conv_len && fdp.remaining_bytes() > 0; i++) {
            // Generate dot patterns (simplified - real dots would be bit patterns)
            inbuf[i] = fdp.ConsumeIntegral<widechar>();
        }
        
        int length = (int)conv_len;
        int mode = fdp.ConsumeIntegral<int>();
        
        // Test dots to char conversion
        int dots_to_char_result = lou_dotsToChar(table_to_use, inbuf.data(), outbuf.data(), length, mode);
        (void)dots_to_char_result;
    }
    
    // === STEP 12: lou_charToDots() ===
    // Test character to dots conversion  
    if (fdp.remaining_bytes() > 16) {
        size_t conv_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::vector<widechar> inbuf(conv_len);
        std::vector<widechar> outbuf(conv_len);
        
        // Fill input buffer with random characters
        for (size_t i = 0; i < conv_len && fdp.remaining_bytes() > 0; i++) {
            inbuf[i] = fdp.ConsumeIntegral<widechar>() % 256;  // ASCII range
        }
        
        int length = (int)conv_len;
        int mode = fdp.ConsumeIntegral<int>();
        
        // Test char to dots conversion
        int char_to_dots_result = lou_charToDots(table_to_use, inbuf.data(), outbuf.data(), length, mode);
        (void)char_to_dots_result;
    }
    
    // === STEP 13: lou_free() ===
    lou_free();
    
    return 0;
}

} // namespace harness_007
