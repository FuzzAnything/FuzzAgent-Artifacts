#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <liblouis/liblouis.h>
}
//<ID> 657
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 3
//<Quality> {"density":2,"unique_branches":{"compileRule":[[2884,7,2884,12,0,0,4,1],[2888,11,2888,30,0,0,4,0],[2888,11,2888,30,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_logPrint","lou_findTables","lou_getTypeformForEmphClass","lou_charToDots","lou_findTables","lou_charToDots","lou_findTables","lou_charToDots","lou_logPrint"],"critical_calls":["lou_registerLogCallback","lou_logPrint","lou_findTables","lou_getTypeformForEmphClass","lou_charToDots","lou_findTables","lou_charToDots","lou_findTables","lou_charToDots","lou_logPrint"],"visited":0}
/*Here's a fuzz driver that uses the specified liblouis APIs to achieve an event where braille translation output data is processed:

*/


// Include liblouis headers

// Custom log callback function
static void  id_000035_logCallback(logLevels level, const char *message) {
    // Simple callback that does nothing - required by lou_registerLogCallback
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<5) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data for basic operations
    if (size < 10) {
        return 0;
    }

    // 1. First, register a log callback
    lou_registerLogCallback( id_000035_logCallback);
    
    // 2. Use lou_logPrint to log a message about fuzzing
    lou_logPrint("Starting liblouis fuzzing session");

    // 3. Find available braille tables using a query from input data
    // Extract query string from input (first 64 bytes or until null terminator)
    size_t query_len = 0;
    while (query_len < size && query_len < 64 && data[query_len] != 0) {
        query_len++;
    }
    
    // Create null-terminated query string
    std::vector<char> query(query_len + 1);
    for (size_t i = 0; i < query_len; i++) {
        query[i] = data[i];
    }
    query[query_len] = '\0';
    
    char **tables = lou_findTables(query.data());
    
    // 4. If tables were found, process them
    if (tables) {
        // Use the first table found (if any) for subsequent operations
        const char *table_to_use = nullptr;
        for (int i = 0; tables[i] != nullptr; i++) {
            table_to_use = tables[i];
            break; // Just use the first table
        }
        
        if (table_to_use) {
            // 5. Get typeform for an emphasis class
            // Extract emphasis class from input data (after query)
            size_t emph_offset = query_len + 1;
            if (emph_offset < size) {
                size_t emph_len = 0;
                while (emph_offset + emph_len < size && emph_len < 32 && 
                       data[emph_offset + emph_len] != 0) {
                    emph_len++;
                }
                
                if (emph_len > 0) {
                    std::vector<char> emph_class(emph_len + 1);
                    for (size_t i = 0; i < emph_len; i++) {
                        emph_class[i] = data[emph_offset + i];
                    }
                    emph_class[emph_len] = '\0';
                    
                    formtype typeform = lou_getTypeformForEmphClass(table_to_use, emph_class.data());
                    (void)typeform; // Use result to avoid unused variable warning
                }
            }
            
            // 6. Convert characters to braille dots using lou_charToDots
            // Use remaining input data as widechar input
            size_t data_offset = query_len + 1;
            if (data_offset < size) {
                size_t remaining = size - data_offset;
                
                // Ensure we have at least some data and it's properly aligned for widechars
                if (remaining >= sizeof(widechar)) {
                    size_t widechar_count = remaining / sizeof(widechar);
                    if (widechar_count > 0) {
                        // Create input buffer
                        std::vector<widechar> inbuf(widechar_count);
                        std::vector<widechar> outbuf(widechar_count);
                        
                        // Copy data to input buffer
                        memcpy(inbuf.data(), data + data_offset, widechar_count * sizeof(widechar));
                        
                        // Call lou_charToDots with a reasonable mode
                        int mode = 0; // Default mode
                        int result = lou_charToDots(table_to_use, inbuf.data(), outbuf.data(), 
                                                  static_cast<int>(widechar_count), mode);
                        (void)result; // Use result to avoid unused variable warning
                    }
                }
            }
        }
        
        // 7. Clean up tables
        // First free individual table strings, then free the array
        if (tables) {
            for (int i = 0; tables[i] != nullptr; i++) {
                // Free the table string
                free(tables[i]);
            }
            // Free the array itself
            free(tables);
        }
    }
    
    // 8. Additional testing with different modes for lou_charToDots
    // Try with another table query using part of input data
    if (size > 5) {
        size_t query2_len = 5;
        std::vector<char> query2(query2_len + 1);
        for (size_t i = 0; i < query2_len; i++) {
            query2[i] = data[i % size];
        }
        query2[query2_len] = '\0';
        
        char **tables2 = lou_findTables(query2.data());
        if (tables2) {
            // Process found tables
            for (int i = 0; tables2[i] != nullptr; i++) {
                // Try different modes for charToDots
                for (int mode = 0; mode <= 1; mode++) {
                    if (size > 20) {
                        size_t test_len = 10;
                        std::vector<widechar> test_in(test_len);
                        std::vector<widechar> test_out(test_len);
                        
                        // Fill test data
                        for (size_t j = 0; j < test_len; j++) {
                            test_in[j] = static_cast<widechar>(data[j % size]);
                        }
                        
                        lou_charToDots(tables2[i], test_in.data(), test_out.data(), 
                                     static_cast<int>(test_len), mode);
                    }
                }
                free(tables2[i]);
            }
            free(tables2);
        }
    }

    // 9. Test with a FILE* input using fmemopen (as per requirement #4)
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // We could read from the file, but for this event we just demonstrate usage
            // Read a small amount to exercise the file descriptor
            uint8_t buffer[16];
            if (fread(buffer, 1, sizeof(buffer), in_file) > 0) {
                // Use the data read from file
                if (sizeof(buffer) >= sizeof(widechar)) {
                    std::vector<widechar> inbuf(2);
                    std::vector<widechar> outbuf(2);
                    inbuf[0] = static_cast<widechar>(buffer[0]);
                    inbuf[1] = static_cast<widechar>(buffer[1]);
                    
                    // Try to find a simple table
                    char **simple_tables = lou_findTables(fuzz_str_1);
                    if (simple_tables && simple_tables[0]) {
                        lou_charToDots(simple_tables[0], inbuf.data(), outbuf.data(), 2, 0);
                        
                        // Free the table list
                        for (int i = 0; simple_tables[i] != nullptr; i++) {
                            free(simple_tables[i]);
                        }
                        free(simple_tables);
                    }
                }
            }
            assert_file_closed(&in_file);;
        }
    }

    // 10. Create an output file (as per requirement #4)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some test data to the output file
        const char *test_output = "Liblouis fuzz test output";
        fwrite(test_output, 1, strlen(test_output), out_file);
        assert_file_closed(&out_file);;
        
        // Remove the output file to avoid accumulating files
        remove("output_file");
    }

    // 11. Final cleanup
    lou_logPrint("Ending liblouis fuzzing session");
    
    assert_file_closed(&out_file);
	return 0;
}