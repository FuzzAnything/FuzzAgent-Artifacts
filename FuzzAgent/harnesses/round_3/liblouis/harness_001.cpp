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

namespace harness_001 {
// liblouis fuzzing harness - Table compilation and checking focus
// Targets: lou_checkTable, lou_compileString, lou_getTable, lou_getEmphClasses, lou_getTypeformForEmphClass
// Uses FuzzedDataProvider for structured input processing
// Based on existing liblouis fuzzing harness patterns (fuzz_translate_generic.c, table_fuzzer.cc)


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

// Create a temporary file with fuzzer data
static char* create_temp_table_file(const uint8_t* data, size_t size, size_t* consumed_size) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/liblouis-fuzz-%d-%d.ctb", getpid(), file_counter++);
    
    // Determine how much data to write to the file (min 1 byte, max 1024 bytes)
    size_t file_size = (size > 0) ? (size % 1024) + 1 : 1;
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
    
    // Consume table name from input (limited to 256 chars)
    std::string table_name = fdp.ConsumeRandomLengthString(256);
    if (table_name.empty()) {
        table_name = "en-us-g1.ctb"; // Default table if no name provided
    }
    
    // For operations that need file data, create temporary table file
    char* temp_filename = NULL;
    size_t file_data_consumed = 0;
    
    if (operation == 0 || operation == 1 || operation == 2) {
        // These operations work with file-based tables
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
                    
                    if (file_len > 0 && file_len < 4096) {
                        char* file_content = (char*)malloc(file_len + 1);
                        if (file_content) {
                            fread(file_content, 1, file_len, fp);
                            file_content[file_len] = '\0';
                            
                            // Try to compile the table content
                            result = lou_compileString("temp-compiled-table", file_content);
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
            // Test lou_getEmphClasses
            char const** emph_classes = lou_getEmphClasses(table_name.c_str());
            if (emph_classes) {
                // Count how many emphasis classes we got
                int count = 0;
                while (emph_classes[count] != NULL) {
                    count++;
                }
                (void)count;
                
                // Free the emphasis classes array
                lou_freeEmphClasses(emph_classes);
            }
            break;
        }
        
        case 4: {
            // Test lou_getTypeformForEmphClass
            // Consume emphasis class name
            std::string emph_class = fdp.ConsumeRandomLengthString(64);
            if (emph_class.empty()) {
                emph_class = "bold"; // Default emphasis class
            }
            
            formtype typeform = lou_getTypeformForEmphClass(table_name.c_str(), emph_class.c_str());
            (void)typeform;
            break;
        }
        
        case 5: {
            // Test lou_setDataPath and lou_getDataPath
            std::string data_path = fdp.ConsumeRandomLengthString(128);
            if (!data_path.empty()) {
                char* old_path = lou_setDataPath(data_path.c_str());
                if (old_path) {
                    // Get the current path
                    char* current_path = lou_getDataPath();
                    (void)current_path;
                }
            }
            break;
        }
    }
    
    // Clean up temporary file if created
    if (temp_filename) {
        unlink(temp_filename);
        free(temp_filename);
    }
    
    return 0;
}

} // namespace harness_001
