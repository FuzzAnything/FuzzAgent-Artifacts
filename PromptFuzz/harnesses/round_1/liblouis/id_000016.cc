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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"includeFile":[[4986,6,4986,9,0,0,4,0]]},"library_calls":["lou_registerTableResolver","lou_logFile","lou_checkTable","lou_indexTables","lou_findTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_translateString","lou_translatePrehyphenated","lou_freeEmphClasses","lou_freeTableFile","lou_free"],"critical_calls":["lou_registerTableResolver","lou_logFile","lou_checkTable","lou_indexTables","lou_findTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_translateString","lou_translatePrehyphenated","lou_freeEmphClasses","lou_freeTableFile","lou_free"],"visited":0}
/**/


// Custom table resolver function
static char ** id_000016_custom_table_resolver(const char *table_name, const char *base) {
    // Simplified resolver - just return the table name as-is
    static char *result[2];
    static char *table_copy;
    
    if (table_copy) free(table_copy);
    
    table_copy = strdup(table_name);
    result[0] = table_copy;
    result[1] = nullptr;
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early exit if not enough data
    if (size < 4) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Register custom table resolver
    lou_registerTableResolver( id_000016_custom_table_resolver);
    
    // Set log file
    lou_logFile(fuzz_str_1);
    
    // Extract table name from input data
    // Use first 50 bytes or less as table name
    size_t table_name_len = size < 50 ? size : 50;
    char *table_list = (char *)malloc(table_name_len + 1);
    if (!table_list) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read table name from input
    fseek(in_file, 0, SEEK_SET);
    size_t read_len = fread(table_list, 1, table_name_len, in_file);
    table_list[read_len] = '\0';
    
    // Check if table is valid
    int table_check = lou_checkTable(table_list);
    
    // Index tables
    const char *tables[] = {"en-us-g2.ctb", "de-de-g2.ctb", table_list, nullptr};
    lou_indexTables(tables);
    
    // Find table using the input as query
    char *found_table = lou_findTable(table_list);
    
    // Get emphasis classes
    const char **emph_classes = lou_getEmphClasses(found_table ? found_table : "en-us-g2.ctb");
    
    // If we have emphasis classes, get typeform for first class
    formtype typeform_value = 0;
    if (emph_classes && emph_classes[0]) {
        typeform_value = lou_getTypeformForEmphClass(found_table ? found_table : "en-us-g2.ctb", emph_classes[0]);
    }
    
    // Process the remaining data as input for translation
    // Reset file pointer to after table name
    fseek(in_file, read_len, SEEK_SET);
    long remaining = size - read_len;
    
    if (remaining > 0) {
        // Read remaining data as wide characters
        std::vector<widechar> inbuf(remaining / sizeof(widechar) + 1);
        size_t widechars_read = fread(inbuf.data(), sizeof(widechar), 
                                     remaining / sizeof(widechar), in_file);
        
        if (widechars_read > 0) {
            int inlen = static_cast<int>(widechars_read);
            int outlen = inlen * 2;  // Allocate enough space for output
            
            std::vector<widechar> outbuf(outlen);
            std::vector<char> spacing(inlen);
            std::vector<formtype> typeform(inlen);
            
            // First translation
            int actual_outlen = outlen;
            int actual_inlen = inlen;
            int result1 = lou_translateString(
                found_table ? found_table : "en-us-g2.ctb",
                inbuf.data(),
                &actual_inlen,
                outbuf.data(),
                &actual_outlen,
                typeform.data(),
                spacing.data(),
                fuzz_int32_t_2
            );
            
            // Prepare for pre-hyphenated translation
            if (widechars_read > 2) {
                std::vector<int> outputPos(actual_outlen);
                std::vector<int> inputPos(actual_inlen);
                std::vector<int> cursorPos(1);
                std::vector<char> inputHyphens(actual_inlen);
                std::vector<char> outputHyphens(actual_outlen);
                
                // Fill hyphen arrays with dummy data
                for (int i = 0; i < actual_inlen; i++) {
                    inputHyphens[i] = (i % 3 == 0) ? 1 : 0;
                    if (typeform_value && i < static_cast<int>(typeform.size())) {
                        typeform[i] = typeform_value;
                    }
                }
                
                // Pre-hyphenated translation
                int result2 = lou_translatePrehyphenated(
                    found_table ? found_table : "en-us-g2.ctb",
                    inbuf.data(),
                    &actual_inlen,
                    outbuf.data(),
                    &actual_outlen,
                    typeform.data(),
                    spacing.data(),
                    outputPos.data(),
                    inputPos.data(),
                    cursorPos.data(),
                    inputHyphens.data(),
                    outputHyphens.data(),
                    0
                );
            }
        }
    }
    
    // Cleanup
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    free(table_list);
    assert_file_closed(&in_file);;
    
    // Free liblouis resources
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}