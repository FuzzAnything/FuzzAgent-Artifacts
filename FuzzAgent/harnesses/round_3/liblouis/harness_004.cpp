#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_004 {
// liblouis fuzzing harness - Low-level table compilation functions
// Targets: Table compilation and pattern processing functions from maketable.c and pattern.c
// Functions: lou_checkTable, lou_compileString, lou_getTable, lou_translateString with fuzzer-generated tables
// Uses FuzzedDataProvider for structured input processing
// Focus: Lower-level table compilation/parsing logic at 0% coverage


// Global initialization flag
static int initialized = 0;
static int file_counter = 0;

// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Create a temporary file with fuzzer data for table compilation testing
static char* create_temp_table_file(const uint8_t* data, size_t size, size_t* consumed_size) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/liblouis-fuzz-table-%d-%d.ctb", getpid(), file_counter++);
    
    // Determine how much data to write to the file (min 1 byte, max 4096 bytes)
    size_t file_size = (size > 0) ? (size % 4096) + 1 : 1;
    if (file_size > size) file_size = size;
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return NULL;
    }
    
    fwrite(data, 1, file_size, fp);
    fclose(fp);
    
    *consumed_size = file_size;
    
    // Duplicate filename for caller to free
    return strdup(filename);
}

// Test table compilation with fuzzer-generated table files
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create a temporary table file for compilation testing
    char* temp_filename = NULL;
    size_t file_data_consumed = 0;
    
    // For operations that need table data, create temporary table file
    if (operation == 0 || operation == 1 || operation == 2 || operation == 3) {
        std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (file_data.empty()) {
            return 0;
        }
        
        temp_filename = create_temp_table_file(file_data.data(), file_data.size(), &file_data_consumed);
        if (!temp_filename) {
            return 0;
        }
    }
    
    int result = 0;
    const void* table_ptr = NULL;
    
    switch (operation) {
        case 0: {
            // Test lou_checkTable with fuzzer-generated table file
            if (temp_filename) {
                result = lou_checkTable(temp_filename);
                (void)result;
                
                // If table is valid, also try to get it
                if (result != 0) {
                    table_ptr = lou_getTable(temp_filename);
                    (void)table_ptr;
                }
            }
            break;
        }
        
        case 1: {
            // Test lou_compileString with fuzzer-generated table content
            if (temp_filename) {
                // Read the file content to use as compilation string
                FILE* fp = fopen(temp_filename, "rb");
                if (fp) {
                    fseek(fp, 0, SEEK_END);
                    long file_len = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    
                    if (file_len > 0 && file_len < 8192) {
                        char* file_content = (char*)malloc(file_len + 1);
                        if (file_content) {
                            fread(file_content, 1, file_len, fp);
                            file_content[file_len] = '\0';
                            
                            // Try to compile the table content
                            result = lou_compileString("fuzzer-compiled-table", file_content);
                            (void)result;
                            
                            free(file_content);
                        }
                    }
                    fclose(fp);
                }
            }
            break;
        }
        
        case 2: {
            // Test lou_getTable directly with fuzzer-generated table file
            if (temp_filename) {
                table_ptr = lou_getTable(temp_filename);
                (void)table_ptr;
            }
            break;
        }
        
        case 3: {
            // Test translation with fuzzer-generated table
            if (temp_filename) {
                // First check if the table is valid
                result = lou_checkTable(temp_filename);
                if (result != 0) {
                    // Load the table
                    table_ptr = lou_getTable(temp_filename);
                    
                    if (table_ptr) {
                        // Consume text to translate from fuzzer input
                        // We need to consume some data for translation - use remaining input
                        // First save current position
                        size_t remaining_before = fdp.remaining_bytes();
                        
                        if (remaining_before > 0) {
                            std::string text_to_translate = fdp.ConsumeRandomLengthString(128);
                            
                            if (!text_to_translate.empty()) {
                                // Convert to widechar
                                size_t text_len = text_to_translate.length();
                                widechar *wide_input = (widechar *)malloc((text_len + 1) * sizeof(widechar));
                                if (wide_input) {
                                    for (size_t i = 0; i < text_len; i++) {
                                        wide_input[i] = (widechar)text_to_translate[i];
                                    }
                                    wide_input[text_len] = 0;
                                    
                                    int inlen = (int)text_len;
                                    int outlen = inlen * 16;
                                    widechar *output = (widechar *)malloc((outlen + 1) * sizeof(widechar));
                                    
                                    if (output) {
                                        // Try translation with the fuzzer-generated table
                                        int translate_result = lou_translateString(temp_filename, wide_input, &inlen,
                                                                                  output, &outlen, NULL, NULL, ucBrl);
                                        (void)translate_result;
                                        
                                        free(output);
                                    }
                                    free(wide_input);
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test lou_compileString with random string data
            std::string random_table_data = fdp.ConsumeRandomLengthString(1024);
            if (!random_table_data.empty()) {
                result = lou_compileString("random-compiled-table", random_table_data.c_str());
                (void)result;
            }
            break;
        }
        
        case 5: {
            // Test lou_checkTable with random table names and patterns
            std::string table_pattern = fdp.ConsumeRandomLengthString(128);
            if (!table_pattern.empty()) {
                // Create various table name patterns
                std::string table_names[] = {
                    table_pattern + ".ctb",
                    table_pattern + ".utb", 
                    "tables/" + table_pattern,
                    table_pattern + "/test.ctb"
                };
                
                for (int i = 0; i < 4; i++) {
                    result = lou_checkTable(table_names[i].c_str());
                    (void)result;
                }
            }
            break;
        }
    }
    
    // Clean up temporary file
    if (temp_filename) {
        unlink(temp_filename);
        free(temp_filename);
    }
    
    return 0;
}

} // namespace harness_004
