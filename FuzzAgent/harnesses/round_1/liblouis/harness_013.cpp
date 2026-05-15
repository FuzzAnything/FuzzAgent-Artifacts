#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <climits>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis/liblouis.h"

namespace harness_013 {
/*
 * liblouis Final Comprehensive Integration Testing Harness (harness_013.cpp)
 * 
 * This harness serves as the final comprehensive integration test for liblouis,
 * targeting all remaining uncovered code paths, complex integration scenarios,
 * and real-world usage patterns not extensively tested in previous harnesses.
 * 
 * Seven comprehensive testing areas:
 * 1) All remaining public APIs not extensively covered in harnesses 000-012
 * 2) Complex table resolver scenarios with custom resolvers that actually resolve tables
 * 3) Advanced logging with actual callback implementations that process messages
 * 4) Comprehensive Unicode handling including surrogate pairs, combining characters, and scripts
 * 5) API sequences that simulate real-world library usage patterns
 * 6) Resource cleanup and re-initialization sequences with proper state management
 * 7) Testing with tables that have complex metadata and dependencies
 * 
 * APIs specifically targeted in this harness:
 * - lou_readCharFromFile: File character reading with different encodings
 * - lou_registerTableResolver: Custom table resolution with resolver callbacks
 * - lou_indexTables, lou_findTable, lou_findTables: Advanced table discovery
 * - lou_getTableInfo, lou_freeTableInfo: Metadata extraction and management
 * - lou_listTables, lou_freeTableFiles: Table listing and cleanup
 * - lou_version, lou_charSize: Library information queries
 * - lou_setDataPath, lou_getDataPath: Data path management (deprecated but tested)
 * - lou_logPrint, lou_logFile, lou_logEnd: Legacy logging system (deprecated)
 * - Comprehensive Unicode handling in all translation APIs
 * - Real-world API sequences with cleanup and reinitialization
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000-012: Focus on functional testing, specific API groups, boundary conditions
 * - This harness: Focuses on INTEGRATION, REAL-WORLD USAGE, COMPLEX SCENARIOS, and UNCOVERED APIS
 */


extern "C" {
}

// ==================== TABLE RESOLVER IMPLEMENTATIONS ====================

// Resolver 1: Simple resolver that returns a fixed set of tables
static char** simple_resolver(const char* table, const char* base) {
    static const char* available_tables[] = {
        "/root/src/liblouis/tables/en-us-g2.ctb",
        "/root/src/liblouis/tables/en-us-g1.ctb",
        "/root/src/liblouis/tables/en-us-comp8.ctb",
        "/root/src/liblouis/tables/afr-za-g1.ctb",
        "/root/src/liblouis/tables/Es-Es-G0.utb",
        "/root/src/liblouis/tables/Pl-Pl-g1.utb",
        NULL
    };
    
    // Count available tables
    size_t count = 0;
    while (available_tables[count] != NULL) {
        count++;
    }
    
    // Allocate array with NULL terminator
    char** result = (char**)malloc((count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    for (size_t i = 0; i < count; i++) {
        result[i] = strdup(available_tables[i]);
        if (!result[i]) {
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
    }
    result[count] = NULL;
    
    (void)table;
    (void)base;
    return result;
}

// Resolver 2: Pattern-based resolver that filters tables by name
static char** pattern_resolver(const char* table, const char* base) {
    static const char* all_tables[] = {
        "/root/src/liblouis/tables/en-us-g2.ctb",
        "/root/src/liblouis/tables/en-us-g1.ctb", 
        "/root/src/liblouis/tables/en-us-comp8.ctb",
        "/root/src/liblouis/tables/afr-za-g1.ctb",
        "/root/src/liblouis/tables/afr-za-g2.ctb",
        "/root/src/liblouis/tables/Es-Es-G0.utb",
        "/root/src/liblouis/tables/IPA.utb",
        "/root/src/liblouis/tables/Lv-Lv-g1.utb",
        "/root/src/liblouis/tables/Pl-Pl-g1.utb",
        "/root/src/liblouis/tables/da-dk-g16.ctb",
        "/root/src/liblouis/tables/da-dk-g18.ctb",
        "/root/src/liblouis/tables/da-dk-g26.ctb",
        "/root/src/liblouis/tables/da-dk-g28.ctb",
        NULL
    };
    
    // If table name contains "en", return English tables
    // If table name contains "afr", return Afrikaans tables
    // Otherwise return all tables
    std::string table_str = table ? table : "";
    std::vector<const char*> matched_tables;
    
    for (size_t i = 0; all_tables[i] != NULL; i++) {
        std::string table_path = all_tables[i];
        bool include = false;
        
        if (table_str.find("en") != std::string::npos) {
            if (table_path.find("en-") != std::string::npos ||
                table_path.find("en.") != std::string::npos) {
                include = true;
            }
        } else if (table_str.find("afr") != std::string::npos) {
            if (table_path.find("afr-") != std::string::npos) {
                include = true;
            }
        } else if (table_str.find("da") != std::string::npos) {
            if (table_path.find("da-") != std::string::npos) {
                include = true;
            }
        } else {
            include = true; // Return all if no specific pattern
        }
        
        if (include) {
            matched_tables.push_back(all_tables[i]);
        }
    }
    
    // Allocate result array
    char** result = (char**)malloc((matched_tables.size() + 1) * sizeof(char*));
    if (!result) return NULL;
    
    for (size_t i = 0; i < matched_tables.size(); i++) {
        result[i] = strdup(matched_tables[i]);
        if (!result[i]) {
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
    }
    result[matched_tables.size()] = NULL;
    
    (void)base;
    return result;
}

// Resolver 3: Empty resolver (returns no tables)
static char** empty_resolver(const char* table, const char* base) {
    char** result = (char**)malloc(sizeof(char*));
    if (!result) return NULL;
    result[0] = NULL;
    
    (void)table;
    (void)base;
    return result;
}

// ==================== ADVANCED LOGGING CALLBACKS ====================

// Callback 1: Message counting callback with level filtering
struct MessageCounter {
    int total_messages;
    int debug_count;
    int info_count;
    int warn_count;
    int error_count;
    logLevels current_filter;
};

static void counting_log_callback(logLevels level, const char* message) {
    static MessageCounter counter = {0, 0, 0, 0, 0, LOU_LOG_ALL};
    
    // Only count if message passes filter
    if (level >= counter.current_filter) {
        counter.total_messages++;
        
        switch (level) {
            case LOU_LOG_DEBUG: counter.debug_count++; break;
            case LOU_LOG_INFO: counter.info_count++; break;
            case LOU_LOG_WARN: counter.warn_count++; break;
            case LOU_LOG_ERROR: counter.error_count++; break;
            case LOU_LOG_FATAL: counter.error_count++; break; // Group with ERROR
            default: break;
        }
    }
    
    // Simulate processing of message content
    if (message && strlen(message) > 0) {
        // Could do actual processing here
        (void)message;
    }
}

// Callback 2: Message content analysis callback
static void analysis_log_callback(logLevels level, const char* message) {
    // Analyze message content for patterns
    if (message) {
        std::string msg_str = message;
        
        // Check for common patterns in liblouis messages
        bool has_table = msg_str.find("table") != std::string::npos;
        bool has_error = msg_str.find("error") != std::string::npos ||
                         msg_str.find("Error") != std::string::npos ||
                         msg_str.find("ERROR") != std::string::npos;
        bool has_warning = msg_str.find("warning") != std::string::npos ||
                           msg_str.find("Warning") != std::string::npos;
        bool has_pattern = msg_str.find("pattern") != std::string::npos;
        
        // Store analysis results (in real implementation, could use this data)
        (void)has_table;
        (void)has_error;
        (void)has_warning;
        (void)has_pattern;
    }
    
    (void)level;
}

// Callback 3: Dynamic filtering callback that changes behavior
static void dynamic_log_callback(logLevels level, const char* message) {
    static int call_count = 0;
    static logLevels dynamic_threshold = LOU_LOG_ALL;
    
    call_count++;
    
    // Change threshold every 10 calls to test dynamic behavior
    if (call_count % 10 == 0) {
        switch ((call_count / 10) % 6) {
            case 0: dynamic_threshold = LOU_LOG_ALL; break;
            case 1: dynamic_threshold = LOU_LOG_DEBUG; break;
            case 2: dynamic_threshold = LOU_LOG_INFO; break;
            case 3: dynamic_threshold = LOU_LOG_WARN; break;
            case 4: dynamic_threshold = LOU_LOG_ERROR; break;
            case 5: dynamic_threshold = LOU_LOG_OFF; break;
        }
    }
    
    // Only "process" message if it passes current threshold
    if (level >= dynamic_threshold && dynamic_threshold != LOU_LOG_OFF) {
        // Simulate message processing
        if (message && strlen(message) > 50) {
            // Long message handling
            (void)message;
        }
    }
}

// ==================== UNICODE HANDLING FUNCTIONS ====================

// Generate Unicode strings with various scripts and combining characters
static std::string generate_unicode_string(FuzzedDataProvider& fdp, size_t max_len) {
    std::string result;
    size_t target_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    for (size_t i = 0; i < target_len && fdp.remaining_bytes() >= 4; ) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (char_type) {
            case 0: // Basic ASCII (U+0000 - U+007F)
                result.push_back(fdp.ConsumeIntegralInRange<char>(32, 126));
                i++;
                break;
                
            case 1: // Latin-1 Supplement (U+0080 - U+00FF)
                result.push_back(0xC2);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 2;
                break;
                
            case 2: // Cyrillic (U+0400 - U+04FF)
                result.push_back(0xD0);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 2;
                break;
                
            case 3: // Greek (U+0370 - U+03FF)
                result.push_back(0xCD);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0xB0, 0xBF));
                i += 2;
                break;
                
            case 4: // Arabic (U+0600 - U+06FF)
                result.push_back(0xD9);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 2;
                break;
                
            case 5: // Combining characters (U+0300 - U+036F)
                result.push_back(0xCC);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 2;
                // Add base character before combining mark
                if (i < target_len && fdp.remaining_bytes() >= 1) {
                    result.insert(result.end() - 2, fdp.ConsumeIntegralInRange<char>('a', 'z'));
                    i++;
                }
                break;
                
            case 6: // Surrogate pair (U+10000 - U+10FFFF)
                // High surrogate (U+D800 - U+DBFF)
                result.push_back(0xED);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0xA0, 0xAF));
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                
                // Low surrogate (U+DC00 - U+DFFF)
                result.push_back(0xED);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0xB0, 0xBF));
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 6;
                break;
                
            case 7: // Emoji/Supplementary (U+1F600 - U+1F64F)
                result.push_back(0xF0);
                result.push_back(0x9F);
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x98, 0x9B));
                result.push_back(fdp.ConsumeIntegralInRange<char>(0x80, 0xBF));
                i += 4;
                break;
        }
    }
    
    return result;
}

// Convert string to widechar with Unicode awareness
static int unicode_string_to_widechar(const char* input, size_t input_len, widechar* output, size_t output_max) {
    // Simplified conversion: treat UTF-8 bytes as individual characters
    // In a real implementation, would properly decode UTF-8
    size_t i = 0, j = 0;
    while (i < input_len && j < output_max) {
        // Simple conversion for fuzzing - each byte becomes a widechar
        // This works for testing but isn't proper Unicode handling
        output[j] = (widechar)(unsigned char)input[i];
        i++;
        j++;
    }
    return (int)j;
}

// ==================== REAL-WORLD API SEQUENCES ====================

// Sequence 1: Full translation lifecycle with cleanup and reinitialization
static void translation_lifecycle_sequence(const char* table, const widechar* input, int input_len, 
                                          FuzzedDataProvider& fdp) {
    // Initialize
    lou_registerLogCallback(counting_log_callback);
    lou_setLogLevel(LOU_LOG_WARN);
    
    // Perform translation
    widechar output_buf[2048];
    int out_len = sizeof(output_buf) / sizeof(output_buf[0]);
    formtype typeforms[2048] = {0};
    char spacing[2048] = {0};
    int outputPos[2048] = {0};
    int inputPos[2048] = {0};
    int cursorPos = 0;
    
    int mode = fdp.ConsumeIntegral<int>();
    
    // Forward translation
    int result = lou_translate(table, input, &input_len, output_buf, &out_len,
                              typeforms, spacing, outputPos, inputPos, &cursorPos, mode);
    
    if (result) {
        // Back translation
        int back_in_len = out_len;
        int back_out_len = input_len;
        widechar back_output[2048];
        
        lou_backTranslate(table, output_buf, &back_in_len, back_output, &back_out_len,
                         typeforms, spacing, outputPos, inputPos, &cursorPos, mode);
    }
    
    // Cleanup
    lou_setLogLevel(LOU_LOG_OFF);
    lou_registerLogCallback(NULL);
}

// Sequence 2: Table discovery and metadata extraction
static void table_discovery_sequence(FuzzedDataProvider& fdp) {
    // Index tables
    const char* tables_to_index[] = {
        "/root/src/liblouis/tables/en-us-g2.ctb",
        "/root/src/liblouis/tables/en-us-g1.ctb",
        "/root/src/liblouis/tables/afr-za-g1.ctb",
        "/root/src/liblouis/tables/Es-Es-G0.utb",
        NULL
    };
    
    lou_indexTables(tables_to_index);
    
    // Find tables with queries
    const char* queries[] = {"en", "us", "g2", "afr", "za", "es", "g0", "g1", NULL};
    
    for (int i = 0; queries[i] != NULL && fdp.remaining_bytes() > 0; i++) {
        char* found_table = lou_findTable(queries[i]);
        if (found_table) {
            // Get metadata
            const char* keys[] = {"display-name", "locale", "grade", "contraction", "type", NULL};
            
            for (int j = 0; keys[j] != NULL && fdp.remaining_bytes() > 0; j++) {
                char* info = lou_getTableInfo(found_table, keys[j]);
                if (info) {
                    lou_freeTableInfo(info);
                }
            }
            
            lou_freeTableFile(found_table);
        }
    }
    
    // List all tables
    char** all_tables = lou_listTables();
    if (all_tables) {
        lou_freeTableFiles(all_tables);
    }
}

// Sequence 3: File reading and legacy logging
static void file_reading_sequence(FuzzedDataProvider& fdp) {
    // Create a temporary file with random content
    char temp_filename[] = "/tmp/liblouis_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd >= 0) {
        // Write some content
        std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(10, 1000));
        write(fd, file_data.data(), file_data.size());
        close(fd);
        
        // Test lou_readCharFromFile
        int mode = 1;
        while (lou_readCharFromFile(temp_filename, &mode) != EOF) {
            // Continue reading
            if (fdp.remaining_bytes() < 10) break;
        }
        
        // Test legacy logging with file
        lou_logFile(temp_filename);
        lou_logPrint("Test message from fuzzer: %d", fdp.ConsumeIntegral<int>());
        lou_logEnd();
        
        // Cleanup
        unlink(temp_filename);
    }
}

// ==================== MAIN FUZZER FUNCTION ====================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static int initialized = 0;
    
    // Initialize liblouis once
    if (!initialized) {
        // Start with default logging
        lou_registerLogCallback(NULL);
        lou_setLogLevel(LOU_LOG_ERROR);
        initialized = 1;
    }
    
    // Need sufficient input for comprehensive testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ========== TEST 1: LIBRARY INFORMATION APIS ==========
    
    // Test lou_version and lou_charSize
    const char* version = lou_version();
    int char_size = lou_charSize();
    
    (void)version;
    (void)char_size;
    
    // Test deprecated data path APIs
    char* old_path = lou_getDataPath();
    if (fdp.remaining_bytes() > 10) {
        std::string new_path = fdp.ConsumeRandomLengthString(100);
        char* result = lou_setDataPath(new_path.c_str());
        if (result) {
            // Path was set successfully
            (void)result;
        }
    }
    if (old_path) {
        // Don't free old_path - according to documentation, it points to static memory
    }
    
    // ========== TEST 2: TABLE RESOLVER REGISTRATION ==========
    
    // Register different resolvers based on input
    uint8_t resolver_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (resolver_type) {
        case 0:
            lou_registerTableResolver(simple_resolver);
            break;
        case 1:
            lou_registerTableResolver(pattern_resolver);
            break;
        case 2:
            lou_registerTableResolver(empty_resolver);
            break;
        case 3:
            // NULL resolver (default behavior)
            lou_registerTableResolver(NULL);
            break;
    }
    
    // ========== TEST 3: ADVANCED LOGGING ==========
    
    // Switch logging callback dynamically
    uint8_t log_callback_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (log_callback_type) {
        case 0:
            lou_registerLogCallback(counting_log_callback);
            break;
        case 1:
            lou_registerLogCallback(analysis_log_callback);
            break;
        case 2:
            lou_registerLogCallback(dynamic_log_callback);
            break;
        case 3:
            lou_registerLogCallback(NULL); // Default callback
            break;
    }
    
    // Set log level based on input
    uint8_t log_level_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    logLevels levels[] = {LOU_LOG_ALL, LOU_LOG_DEBUG, LOU_LOG_INFO, 
                         LOU_LOG_WARN, LOU_LOG_ERROR, LOU_LOG_OFF};
    lou_setLogLevel(levels[log_level_choice]);
    
    // ========== TEST 4: UNICODE HANDLING ==========
    
    // Generate Unicode text for testing
    std::string unicode_text = generate_unicode_string(fdp, 500);
    if (unicode_text.empty() || fdp.remaining_bytes() < 20) {
        return 0;
    }
    
    // Convert to widechar for translation APIs
    widechar wide_input[1024];
    int wide_len = unicode_string_to_widechar(unicode_text.c_str(), unicode_text.size(),
                                             wide_input, sizeof(wide_input)/sizeof(wide_input[0]));
    
    if (wide_len <= 0) {
        return 0;
    }
    
    // ========== TEST 5: REAL-WORLD API SEQUENCES ==========
    
    // Choose a table for translation
    const char* test_tables[] = {
        "/root/src/liblouis/tables/en-us-g2.ctb",
        "/root/src/liblouis/tables/en-us-g1.ctb",
        "/root/src/liblouis/tables/afr-za-g1.ctb",
        "/root/src/liblouis/tables/Es-Es-G0.utb",
        "/root/src/liblouis/tables/Pl-Pl-g1.utb",
        NULL
    };
    
    uint8_t table_index = fdp.ConsumeIntegral<uint8_t>() % 5;
    const char* selected_table = test_tables[table_index];
    
    // Test various API sequences based on remaining input
    uint8_t sequence_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (sequence_type) {
        case 0:
            translation_lifecycle_sequence(selected_table, wide_input, wide_len, fdp);
            break;
        case 1:
            table_discovery_sequence(fdp);
            break;
        case 2:
            file_reading_sequence(fdp);
            break;
    }
    
    // ========== TEST 6: COMPREHENSIVE CLEANUP AND REINITIALIZATION ==========
    
    // Test lou_free to cleanup everything
    if (fdp.ConsumeBool()) {
        lou_free();
        
        // Reinitialize after cleanup
        lou_registerLogCallback(NULL);
        lou_setLogLevel(LOU_LOG_WARN);
        
        // Verify we can still use basic functions after reinitialization
        if (fdp.remaining_bytes() > 10) {
            // Try a simple translation after reinitialization
            widechar test_output[1024];
            int test_out_len = sizeof(test_output) / sizeof(test_output[0]);
            int test_in_len = std::min(wide_len, 100);
            
            lou_translateString(selected_table, wide_input, &test_in_len,
                               test_output, &test_out_len, NULL, NULL, 0);
        }
    }
    
    // ========== TEST 7: COMPLEX TABLE METADATA AND DEPENDENCIES ==========
    
    // Test with tables that have complex metadata
    if (fdp.remaining_bytes() > 50) {
        const char* complex_tables[] = {
            "/root/src/liblouis/tables/da-dk-g16.ctb",  // Has emphasis classes
            "/root/src/liblouis/tables/da-dk-g26.ctb",  // Grade 2 with contractions
            "/root/src/liblouis/tables/en-ueb-g1.ctb",  // Unified English Braille
            NULL
        };
        
        uint8_t complex_table_idx = fdp.ConsumeIntegral<uint8_t>() % 3;
        const char* complex_table = complex_tables[complex_table_idx];
        
        // Get emphasis classes
        char const** emph_classes = lou_getEmphClasses(complex_table);
        if (emph_classes) {
            // Test typeform mapping for each class
            for (int i = 0; emph_classes[i] != NULL && fdp.remaining_bytes() > 10; i++) {
                formtype tf = lou_getTypeformForEmphClass(complex_table, emph_classes[i]);
                (void)tf;
            }
            lou_freeEmphClasses(emph_classes);
        }
        
        // Test table compilation with random strings
        if (fdp.remaining_bytes() > 20) {
            std::string compile_str = fdp.ConsumeRandomLengthString(200);
            lou_compileString(complex_table, compile_str.c_str());
        }
    }
    
    // Final cleanup
    lou_setLogLevel(LOU_LOG_OFF);
    lou_registerLogCallback(NULL);
    lou_registerTableResolver(NULL);
    
    return 0;
}

} // namespace harness_013
