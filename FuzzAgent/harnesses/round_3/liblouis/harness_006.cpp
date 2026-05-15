#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <algorithm>

namespace harness_006 {
// liblouis fuzzing harness - Custom table resolution and advanced compilation
// Targets: lou_registerTableResolver, advanced table compilation opcodes, maketable.c helper functions
// Focus on custom table resolution, advanced table compilation logic, and maketable.c utilities
// FIXED VERSION: Improved determinism, state isolation, and crash reproducibility


// Custom table resolver function for testing lou_registerTableResolver
// Completely deterministic based ONLY on fuzzer input, no dynamic allocations
// Uses fixed static buffers to avoid memory management issues across iterations
static char **EXPORT_CALL fuzzer_table_resolver(const char *tableList, const char *base) {
    // Use tableList and base to generate deterministic behavior
    // Return different behaviors based on hash of inputs
    if (!tableList) {
        return NULL;
    }
    
    // Simple hash for deterministic behavior
    unsigned int hash = 0;
    const char *p = tableList;
    while (*p) {
        hash = (hash * 31) + (unsigned char)(*p);
        p++;
    }
    if (base) {
        p = base;
        while (*p) {
            hash = (hash * 37) + (unsigned char)(*p);
            p++;
        }
    }
    
    int behavior = hash % 8;
    
    // Static buffers for resolver results - no dynamic allocation
    // Using multiple buffers to avoid conflicts if called multiple times
    static char *result_buffers[8][4];  // 8 behaviors, up to 4 strings each
    static char string_storage[8][4][64];  // Actual string storage
    
    // Initialize on first call
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 4; j++) {
                result_buffers[i][j] = string_storage[i][j];
            }
        }
        initialized = 1;
    }
    
    // Select which buffer set to use based on behavior
    int buffer_set = behavior;
    
    switch (behavior) {
        case 0:
            // Return NULL to test error handling
            return NULL;
            
        case 1:
            // Return empty list (just NULL terminator)
            result_buffers[buffer_set][0] = NULL;
            return result_buffers[buffer_set];
            
        case 2:
        case 3:
            // Return a valid path (deterministic but unique per call)
            snprintf(string_storage[buffer_set][0], 64, "/tmp/liblouis-fuzz-%u.ctb", hash);
            result_buffers[buffer_set][0] = string_storage[buffer_set][0];
            result_buffers[buffer_set][1] = NULL;
            return result_buffers[buffer_set];
            
        case 4:
        case 5:
            // Return multiple paths (deterministic)
            snprintf(string_storage[buffer_set][0], 64, "/tmp/liblouis-fuzz-%u-1.ctb", hash);
            snprintf(string_storage[buffer_set][1], 64, "/tmp/liblouis-fuzz-%u-2.ctb", hash);
            snprintf(string_storage[buffer_set][2], 64, "/tmp/liblouis-fuzz-%u-3.ctb", hash);
            
            result_buffers[buffer_set][0] = string_storage[buffer_set][0];
            result_buffers[buffer_set][1] = string_storage[buffer_set][1];
            result_buffers[buffer_set][2] = string_storage[buffer_set][2];
            result_buffers[buffer_set][3] = NULL;
            return result_buffers[buffer_set];
            
        case 6:
            // Return invalid/nonexistent path (deterministic)
            snprintf(string_storage[buffer_set][0], 64, "/nonexistent/path/table-%u.ctb", hash);
            result_buffers[buffer_set][0] = string_storage[buffer_set][0];
            result_buffers[buffer_set][1] = NULL;
            return result_buffers[buffer_set];
            
        case 7:
        default:
            // Return single deterministic path
            snprintf(string_storage[buffer_set][0], 64, "/tmp/fallback-%u.ctb", hash);
            result_buffers[buffer_set][0] = string_storage[buffer_set][0];
            result_buffers[buffer_set][1] = NULL;
            return result_buffers[buffer_set];
    }
}

// Log callback to suppress output during fuzzing
void fuzzer_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Suppress logging for cleaner fuzzing
}


// Helper to create a temporary table file with fuzzer data - deterministic and cleaned up
static char* create_temp_table_file(const uint8_t* data, size_t size, unsigned int unique_id) {
    char filename[256];
    // Use unique_id for deterministic naming
    snprintf(filename, sizeof(filename), "/tmp/liblouis-fuzz-temp-%u.ctb", unique_id);
    
    // Clean up any existing file with this name
    unlink(filename);
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return NULL;
    }
    
    // Write fuzzer data to file (limit to reasonable size)
    size_t write_size = size > 4096 ? 4096 : size;
    fwrite(data, 1, write_size, fp);
    fclose(fp);
    
    return strdup(filename);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Register log callback for this iteration only
    lou_registerLogCallback(fuzzer_log_callback);
    
    // Consume operation type to decide which functionality to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    // Consume table name/string from input
    std::string table_input = fdp.ConsumeRandomLengthString(512);
    if (table_input.empty()) {
        // If empty, use a minimal table name derived from fuzzer data
        table_input = "t";
    }
    
    // Consume compilation rule string
    std::string rule_string = fdp.ConsumeRandomLengthString(1024);
    
    // Consume text for helper function testing
    std::string test_text = fdp.ConsumeRandomLengthString(256);
    
    // Generate unique ID for this iteration from fuzzer data
    uint32_t unique_id = fdp.ConsumeIntegral<uint32_t>();
    
    // Track resources to clean up
    std::vector<char*> temp_files_to_clean;
    std::vector<char**> resolver_results_to_clean;
    
    switch (operation) {
        case 0: {
            // Test lou_registerTableResolver - custom table resolution
            // Register custom resolver for this iteration
            lou_registerTableResolver(fuzzer_table_resolver);
            
            // Test that resolver is now active by attempting to use it
            int check_result = lou_checkTable(table_input.c_str());
            (void)check_result;
            
            // IMPORTANT: Reset to default resolver BEFORE calling lou_free()
            lou_registerTableResolver(NULL);
            break;
        }
        
        case 1: {
            // Test lou_compileString with advanced compilation rules
            // This exercises the compileRule function internally
            
            // Register custom resolver for this iteration
            lou_registerTableResolver(fuzzer_table_resolver);
            
            // Try to compile various rule types based on fuzzer input
            if (!rule_string.empty()) {
                // Use fuzzer input to determine which type of advanced rule to create
                uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                std::string advanced_rule;
                
                // Generate rules using fuzzer data instead of hardcoded values
                switch (rule_type) {
                    case 0:
                        // Test swap rules
                        advanced_rule = rule_string.substr(0, std::min(rule_string.size(), size_t(100)));
                        break;
                    case 1:
                        // Test class rules  
                        advanced_rule = "class " + rule_string.substr(0, std::min(rule_string.size(), size_t(50)));
                        break;
                    case 2:
                        // Test context rules
                        advanced_rule = "context " + rule_string.substr(0, std::min(rule_string.size(), size_t(50)));
                        break;
                    case 3:
                        // Test correct rules
                        advanced_rule = "correct " + rule_string.substr(0, std::min(rule_string.size(), size_t(50)));
                        break;
                }
                
                if (!advanced_rule.empty()) {
                    int compile_result = lou_compileString(table_input.c_str(), advanced_rule.c_str());
                    (void)compile_result;
                }
            }
            
            // Reset to default resolver after test
            lou_registerTableResolver(NULL);
            break;
        }
        
        case 2: {
            // Test table loading and compilation with complex scenarios
            // Create a temporary table file with fuzzer data
            char* temp_table = create_temp_table_file(data, size, unique_id);
            if (temp_table) {
                temp_files_to_clean.push_back(temp_table);
                
                // Check the table (exercises compilation)
                int check_result = lou_checkTable(temp_table);
                (void)check_result;
                
                // Test translation with the custom table if valid
                if (check_result != 0 && !test_text.empty()) {
                    size_t text_len = test_text.length();
                    widechar* wide_input = (widechar*)malloc((text_len + 1) * sizeof(widechar));
                    if (wide_input) {
                        for (size_t i = 0; i < text_len; i++) {
                            wide_input[i] = (widechar)test_text[i];
                        }
                        wide_input[text_len] = 0;
                        
                        int inlen = (int)text_len;
                        int outlen = inlen * 16;
                        widechar* output = (widechar*)malloc((outlen + 1) * sizeof(widechar));
                        if (output) {
                            int trans_result = lou_translateString(
                                temp_table, wide_input, &inlen, output, &outlen, 
                                NULL, NULL, ucBrl);
                            (void)trans_result;
                            free(output);
                        }
                        free(wide_input);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test advanced table features that might exercise maketable.c helpers
            
            // Try hyphenation (may use suggestChunks internally)
            if (!test_text.empty()) {
                size_t text_len = test_text.length();
                widechar* wide_input = (widechar*)malloc((text_len + 1) * sizeof(widechar));
                if (wide_input) {
                    for (size_t i = 0; i < text_len; i++) {
                        wide_input[i] = (widechar)test_text[i];
                    }
                    wide_input[text_len] = 0;
                    
                    char* hyphens = (char*)malloc(text_len + 1);
                    if (hyphens) {
                        int hyphen_result = lou_hyphenate(
                            table_input.c_str(), wide_input, (int)text_len, hyphens, 0);
                        (void)hyphen_result;
                        free(hyphens);
                    }
                    free(wide_input);
                }
            }
            break;
        }
        
        case 4: {
            // Test emphasis and mode features (advanced compilation opcodes)
            
            // Try to get emphasis classes (tests table compilation for emphasis)
            char const** emph_classes = lou_getEmphClasses(table_input.c_str());
            if (emph_classes) {
                // Process emphasis classes
                for (int i = 0; emph_classes[i]; i++) {
                    // Try to get typeform for each class
                    formtype typeform = lou_getTypeformForEmphClass(
                        table_input.c_str(), emph_classes[i]);
                    (void)typeform;
                }
                lou_freeEmphClasses(emph_classes);
            }
            break;
        }
        
        case 5: {
            // Test error scenarios and edge cases in table resolution
            
            // Test with NULL/empty inputs
            lou_checkTable("");
            lou_getTable("");
            
            // Test with very long table names
            std::string long_name = fdp.ConsumeRandomLengthString(2048);
            lou_checkTable(long_name.c_str());
            
            // Test with special characters in table names
            std::string special_name;
            size_t special_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            for (size_t i = 0; i < special_len; i++) {
                special_name.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            lou_checkTable(special_name.c_str());
            
            break;
        }
        
        case 6: {
            // Test complex resolver interaction with state management
            // Register custom resolver
            lou_registerTableResolver(fuzzer_table_resolver);
            
            // Test multiple resolver calls
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string test_table = fdp.ConsumeRandomLengthString(100);
                int result = lou_checkTable(test_table.c_str());
                (void)result;
            }
            
            // IMPORTANT: Always reset resolver before lou_free()
            lou_registerTableResolver(NULL);
            break;
        }
    }
    
    // Clean up temporary files
    for (char* temp_file : temp_files_to_clean) {
        unlink(temp_file);
        free(temp_file);
    }
    
    // Clean up liblouis state for this iteration
    // CRITICAL: lou_free() must be called AFTER any custom resolver is unregistered
    lou_free();
    
    return 0;
}

} // namespace harness_006
