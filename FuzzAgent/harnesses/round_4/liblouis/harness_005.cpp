#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "liblouis.h"

namespace harness_005 {
// liblouis table compilation deep fuzzing harness
// Target APIs: lou_compileString (Primary), lou_checkTable (Secondary)
// Required helpers: lou_freeTableFile, lou_setDataPath (optional), lou_getDataPath (optional)
// Based on coverage analysis guidance targeting compileRule function (478 blocked branches)
// Invocation sequence: setDataPath (optional) -> compileString -> checkTable -> freeTableFile -> getDataPath (optional)
// Semantic differentiation from harness_003: Focuses more deeply on compileRule function with diverse table string inputs


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate table names from fuzzer input
static std::string generate_table_name(FuzzedDataProvider& fdp) {
    // Generate table names that might trigger different compilation paths
    const char* table_prefixes[] = {
        "en-", "fr-", "de-", "es-", "ar-", "zh-", "ja-", "ko-", "test-", "example-", "invalid-"
    };
    const char* table_suffixes[] = {
        "g1", "g2", "g3", "comp", "comp8", "compbrl", "ascii", "unicode", "utb", "ctb", "dis", "tbl"
    };
    
    std::string table_name;
    
    // Choose a prefix
    uint8_t prefix_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(table_prefixes)/sizeof(table_prefixes[0]) - 1);
    table_name += table_prefixes[prefix_idx];
    
    // Add middle part based on fuzzer input
    size_t middle_len = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    std::string middle = fdp.ConsumeBytesAsString(middle_len);
    
    // Clean middle part to be alphanumeric or dashes
    for (char c : middle) {
        if (isalnum(c) || c == '-') {
            table_name += c;
        } else {
            table_name += '-';
        }
    }
    
    // Choose a suffix
    uint8_t suffix_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(table_suffixes)/sizeof(table_suffixes[0]) - 1);
    table_name += table_suffixes[suffix_idx];
    
    // Add extension
    if (fdp.ConsumeBool()) {
        table_name += ".ctb";
    } else {
        table_name += ".utb";
    }
    
    return table_name;
}

// Helper to generate table content that might exercise compileRule function
static std::string generate_table_content(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Start with optional table metadata
    if (fdp.ConsumeBool()) {
        content += "table ";
        content += fdp.ConsumeRandomLengthString(32);
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "locale ";
        content += fdp.ConsumeRandomLengthString(16);
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "author ";
        content += fdp.ConsumeRandomLengthString(32);
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "dots ";
        content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 255));
        content += "\n";
    }
    
    // Add rules to exercise compileRule function
    size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(0, 20);
    for (size_t i = 0; i < num_rules; i++) {
        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (rule_type) {
            case 0: // Character mapping rule
                content += fdp.ConsumeRandomLengthString(1);
                content += " ";
                content += fdp.ConsumeRandomLengthString(1);
                content += "\n";
                break;
                
            case 1: // Multicharacter rule
                content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 4));
                content += " ";
                content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 4));
                content += "\n";
                break;
                
            case 2: // Pattern rule
                content += "pat ";
                content += fdp.ConsumeRandomLengthString(4);
                content += " ";
                content += fdp.ConsumeRandomLengthString(4);
                content += "\n";
                break;
                
            case 3: // Context rule
                content += "ctx ";
                content += fdp.ConsumeRandomLengthString(2);
                content += " ";
                content += fdp.ConsumeRandomLengthString(2);
                content += "\n";
                break;
                
            case 4: // Class definition
                content += "class ";
                content += fdp.ConsumeRandomLengthString(8);
                content += " ";
                content += fdp.ConsumeRandomLengthString(16);
                content += "\n";
                break;
                
            case 5: // Emphasis rule
                content += "emph ";
                content += fdp.ConsumeRandomLengthString(8);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(0, 65535));
                content += "\n";
                break;
        }
    }
    
    // Add optional include directive
    if (fdp.ConsumeBool()) {
        content += "include ";
        content += fdp.ConsumeRandomLengthString(16);
        content += "\n";
    }
    
    // Add optional finish directive
    if (fdp.ConsumeBool()) {
        content += "finish\n";
    }
    
    return content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters and table data
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Track if we've successfully compiled a table
    char* compiled_table_name = nullptr;
    
    // 1. Optionally call lou_setDataPath to configure table search path
    // Use fuzzer input to generate a path
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        size_t path_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(256, fdp.remaining_bytes()));
        std::string data_path = fdp.ConsumeBytesAsString(path_len);
        char* path_result = lou_setDataPath(data_path.c_str());
        (void)path_result; // May be NULL if path is invalid
    }
    
    // Generate multiple table compilation attempts to increase coverage
    size_t num_attempts = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    for (size_t attempt = 0; attempt < num_attempts && fdp.remaining_bytes() > 10; attempt++) {
        // 2. Call lou_compileString with various table string inputs
        // Generate table name and content from fuzzer input
        std::string table_name = generate_table_name(fdp);
        std::string table_content = generate_table_content(fdp);
        
        // Try to compile the table string
        int compile_result = lou_compileString(table_name.c_str(), table_content.c_str());
        
        // If compilation succeeded, store the table name for later checks
        if (compile_result == 1) {
            // Make a copy of the table name for lou_freeTableFile
            compiled_table_name = strdup(table_name.c_str());
            
            // 3. Call lou_checkTable to validate compiled tables
            // Check the specific table we just compiled
            int check_result = lou_checkTable(table_name.c_str());
            (void)check_result; // Result can be 0 or non-zero
            
            // Also check with variations of the table name
            if (fdp.ConsumeBool()) {
                std::string alt_name = table_name + ",en-us-g2.ctb";
                int alt_check = lou_checkTable(alt_name.c_str());
                (void)alt_check;
            }
        }
        
        // Try compilation with different variations
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            // Compile with comma-separated table list
            std::string table_list = table_name + ",test.ctb,example.utb";
            std::string alt_content = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 512));
            int list_compile = lou_compileString(table_list.c_str(), alt_content.c_str());
            (void)list_compile;
        }
        
        // Try with empty or minimal table content
        if (fdp.ConsumeBool()) {
            int empty_compile = lou_compileString("empty-test.ctb", "");
            (void)empty_compile;
        }
        
        // Try with just whitespace
        if (fdp.ConsumeBool()) {
            int whitespace_compile = lou_compileString("whitespace-test.ctb", "   \n\t  \n");
            (void)whitespace_compile;
        }
        
        // Try with malformed table content (edge cases)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            std::string malformed = fdp.ConsumeRemainingBytesAsString();
            int malformed_compile = lou_compileString("malformed-test.ctb", malformed.c_str());
            (void)malformed_compile;
            break; // Used all remaining bytes
        }
    }
    
    // 4. Call lou_freeTableFile to clean up resources
    // Free any table file that was successfully compiled
    if (compiled_table_name != nullptr) {
        // First try to find the table
        char* found_table = lou_findTable(compiled_table_name);
        if (found_table != nullptr) {
            lou_freeTableFile(found_table);
        }
        
        // Free the table compilation itself
        lou_freeTableFile(compiled_table_name);
        free(compiled_table_name);
    }
    
    // 5. Optionally call lou_getDataPath to verify configuration
    if (fdp.ConsumeBool()) {
        char* current_path = lou_getDataPath();
        (void)current_path; // May be NULL
    }
    
    // Additional cleanup: call lou_free to free all memory
    lou_free();
    
    return 0;
}

} // namespace harness_005
