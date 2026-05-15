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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_registerTableResolver","lou_getTypeformForEmphClass","lou_setDataPath","lou_translate","lou_charToDots","lou_setDataPath","lou_free"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_listTables","lou_freeTableFiles","lou_registerTableResolver","lou_getTypeformForEmphClass","lou_translate","lou_charToDots","lou_setDataPath","lou_free"],"visited":0}
/**/


// Include liblouis headers

// Custom table resolver function
static char ** id_000009_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL - liblouis will use its default resolver
    (void)table;
    (void)base;
    return NULL;
}

// Helper function to convert bytes to widechar
static std::vector<widechar>  id_000009_bytes_to_widechars(const uint8_t *data, size_t size) {
    std::vector<widechar> result;
    // Convert bytes to widechars (2 bytes per widechar)
    for (size_t i = 0; i + 1 < size; i += 2) {
        widechar wc = (data[i] << 8) | data[i + 1];
        result.push_back(wc);
    }
    return result;
}

// Helper function to extract null-terminated string from data
static std::string  id_000009_extract_string(const uint8_t *&data, size_t &size) {
    std::string result;
    while (size > 0 && *data != 0) {
        result.push_back(static_cast<char>(*data));
        ++data;
        --size;
    }
    if (size > 0 && *data == 0) {
        ++data; // Skip null terminator
        --size;
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Create a copy of pointers since we'll be consuming the data
    const uint8_t *data_ptr = data;
    size_t remaining_size = size;
    
    // 1. Get and set data path
    char *original_path = lou_getDataPath();
    if (original_path) {
        // Extract a path from input data
        std::string new_path =  id_000009_extract_string(data_ptr, remaining_size);
        if (!new_path.empty()) {
            char *set_path_result = lou_setDataPath(new_path.c_str());
            if (set_path_result) {
                // Path was set successfully
            }
        }
    }
    
    // 2. List available tables
    char **tables = lou_listTables();
    if (tables) {
        // Free the table list when done
        lou_freeTableFiles(tables);
    }
    
    // 3. Register a table resolver
    lou_registerTableResolver( id_000009_custom_table_resolver);
    
    // 4. Get typeform for emphasis class
    // Extract table list and emphasis class from input data
    std::string table_list =  id_000009_extract_string(data_ptr, remaining_size);
    std::string emph_class =  id_000009_extract_string(data_ptr, remaining_size);
    
    if (!table_list.empty() && !emph_class.empty()) {
        formtype typeform = lou_getTypeformForEmphClass(table_list.c_str(), emph_class.c_str());
        (void)typeform; // Use typeform to avoid unused variable warning
    }
    
    // 5. Perform translation
    // Convert remaining data to widechars for translation
    std::vector<widechar> inbuf =  id_000009_bytes_to_widechars(data_ptr, remaining_size);
    if (inbuf.empty()) {
        // Restore original data path if we changed it
        if (original_path) {
            lou_setDataPath(original_path);
        }
        return 0;
    }
    
    int inlen = static_cast<int>(inbuf.size());
    int outlen = inlen * 4; // Allocate enough space for output
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(inlen);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(inlen);
    
    // Use a default table list if none was extracted
    if (table_list.empty()) {
        table_list = "en-us-g2.ctb";
    }
    
    formtype typeform = 0;
    int mode = 0;
    
    int translate_result = lou_translate(
        table_list.c_str(),
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_1
    );
    (void)translate_result; // Use result to avoid unused variable warning
    
    // 6. Perform charToDots conversion
    // Use the same input buffer for charToDots
    std::vector<widechar> dots_outbuf(inbuf.size());
    int dots_result = lou_charToDots(
        table_list.c_str(),
        inbuf.data(),
        dots_outbuf.data(),
        static_cast<int>(inbuf.size()),
        fuzz_int32_t_2
    );
    (void)dots_result; // Use result to avoid unused variable warning
    
    // 7. Restore original data path
    if (original_path) {
        lou_setDataPath(original_path);
    }
    
    // 8. Free liblouis resources
    lou_free();
    
    return 0;
}