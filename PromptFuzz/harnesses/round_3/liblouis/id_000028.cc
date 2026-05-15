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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"analyzeTable":[[728,10,728,30,0,0,4,1]]},"library_calls":["lou_registerTableResolver","lou_getTypeformForEmphClass","lou_charToDots","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_free","lou_logEnd"],"critical_calls":["lou_registerTableResolver","lou_getTypeformForEmphClass","lou_charToDots","lou_getTableInfo","lou_freeTableInfo","lou_logFile","lou_free","lou_logEnd"],"visited":0}
/*Looking at the liblouis APIs and the requirements, I'll create a fuzz driver that processes braille output data. The event I'll implement is: converting braille output data back to text while using table resolution and logging.

Here's the complete fuzzer implementation:

*/


// Include the liblouis headers

// Custom table resolver function
static char ** id_000028_my_table_resolver(const char *table_list, const char *base) {
    // Simple resolver that returns the table list as-is
    static char *result[2];
    static char table_copy[256];
    
    if (table_list) {
        strncpy(table_copy, table_list, sizeof(table_copy) - 1);
        table_copy[sizeof(table_copy) - 1] = '\0';
        result[0] = table_copy;
        result[1] = nullptr;
        return result;
    }
    
    return nullptr;
}

// Global variable for table list
static std::string  id_000028_g_table_list;

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
        // Not enough data for meaningful processing
        return 0;
    }

    // Step 1: Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Read table list from the beginning of data
    // The first part of data contains the table list string
    char table_buffer[256] = {0};
    size_t table_len = size > 255 ? 255 : size;
    fread(table_buffer, 1, table_len, in_file);
    
    // Ensure null termination
    table_buffer[table_len] = '\0';
    
    // Find the first null terminator in the buffer
    char *table_end = strchr(table_buffer, '\0');
    if (table_end) {
        *table_end = '\0';
         id_000028_g_table_list = table_buffer;
    } else {
         id_000028_g_table_list = "en-us-comp8.ctb"; // Default table
    }

    // Step 3: Register table resolver
    lou_registerTableResolver( id_000028_my_table_resolver);

    // Step 4: Get remaining data for braille processing
    fseek(in_file, 0, SEEK_SET);
    fseek(in_file, strlen(table_buffer) + 1, SEEK_SET);
    
    // Read emphasis class from data
    char emph_buffer[256] = {0};
    size_t emph_len = size - ftell(in_file) > 255 ? 255 : size - ftell(in_file);
    if (emph_len > 0) {
        fread(emph_buffer, 1, emph_len, in_file);
        emph_buffer[emph_len] = '\0';
    }

    // Step 5: Get typeform for emphasis class
    formtype typeform = 0;
    if (strlen(emph_buffer) > 0) {
        typeform = lou_getTypeformForEmphClass( id_000028_g_table_list.c_str(), emph_buffer);
    }

    // Step 6: Read braille dots data (remaining bytes)
    long remaining = size - ftell(in_file);
    if (remaining > 0) {
        // Allocate buffer for braille dots
        std::vector<widechar> dots_buffer(remaining / sizeof(widechar) + 1);
        size_t dots_count = fread(dots_buffer.data(), sizeof(widechar), 
                                 remaining / sizeof(widechar), in_file);
        
        if (dots_count > 0) {
            // Allocate output buffer for charToDots conversion
            std::vector<widechar> output_buffer(dots_count);
            
            // Step 7: Convert braille dots to characters
            int result = lou_charToDots(
                 id_000028_g_table_list.c_str(),
                dots_buffer.data(),
                output_buffer.data(),
                dots_count,
                fuzz_int32_t_1  // mode
            );
            
            // Get table info (key "locale" is commonly available)
            char *table_info = lou_getTableInfo( id_000028_g_table_list.c_str(), fuzz_str_2);
            if (table_info) {
                lou_freeTableInfo(table_info);
            }
        }
    }

    // Step 8: Set up logging
    lou_logFile(fuzz_str_3);
    
    // Optional: Perform additional logging operations
    // lou_logPrint("Fuzzing completed with table: %s",  id_000028_g_table_list.c_str());

    // Step 9: Clean up
    assert_file_closed(&in_file);;
    
    // Clean up liblouis resources
    lou_free();
    lou_logEnd();
    
    // Remove the log file we created
    remove("fuzz_output.log");

    assert_file_closed(&in_file);
	return 0;
}