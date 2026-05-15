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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"_lou_translate":[[1186,62,1186,76,0,0,4,0]]},"library_calls":["lou_getDataPath","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_freeTableFiles","lou_freeEmphClasses","lou_freeTableFiles","lou_translateString","lou_freeEmphClasses","lou_freeTableFiles","lou_freeEmphClasses","lou_freeTableFiles","lou_translateString","lou_freeEmphClasses","lou_freeTableFiles"],"critical_calls":["lou_getDataPath","lou_listTables","lou_checkTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_translateString","lou_translateString","lou_freeEmphClasses","lou_freeTableFiles"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 1: Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could log or use data_path if needed
    }
    
    // Step 2: List available tables
    char **tables = lou_listTables();
    if (!tables) {
        return 0;  // No tables available
    }
    
    // Find a valid table to use
    const char *selected_table = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {  // Table is valid
            selected_table = tables[i];
            break;
        }
    }
    
    if (!selected_table) {
        lou_freeTableFiles(tables);
        return 0;  // No valid table found
    }
    
    // Step 3: Get emphasis classes for the selected table
    const char **emph_classes = lou_getEmphClasses(selected_table);
    
    // Step 4: Get typeform for the first emphasis class (if available)
    formtype typeform_value = 0;
    if (emph_classes && emph_classes[0]) {
        typeform_value = lou_getTypeformForEmphClass(selected_table, emph_classes[0]);
    }
    
    // Step 5: Prepare input buffer from fuzzing data
    // We need to convert uint8_t to widechar for liblouis
    if (size < 2) {
        lou_freeEmphClasses(emph_classes);
        lou_freeTableFiles(tables);
        return 0;  // Need at least 2 bytes for one widechar
    }
    
    // Convert data to widechar buffer
    // Each widechar is 2 bytes, but we'll use the data as-is
    int inlen = size / sizeof(widechar);
    if (inlen == 0) {
        lou_freeEmphClasses(emph_classes);
        lou_freeTableFiles(tables);
        return 0;
    }
    
    widechar *inbuf = (widechar *)data;  // Cast directly - data is already bytes
    widechar *outbuf = nullptr;
    int outlen = 0;
    
    // Step 6: First translation pass to get output length
    int actual_inlen = inlen;
    int actual_outlen = 0;
    int mode = 0;
    
    // First call to get required output buffer size
    int result = lou_translateString(selected_table, inbuf, &actual_inlen, 
                                     nullptr, &actual_outlen, nullptr, nullptr, fuzz_int32_t_1);
    
    if (result == 0 || actual_outlen <= 0) {
        // Translation failed or no output needed
        lou_freeEmphClasses(emph_classes);
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // Step 7: Allocate output buffer and perform actual translation
    outbuf = (widechar *)malloc(actual_outlen * sizeof(widechar));
    if (!outbuf) {
        lou_freeEmphClasses(emph_classes);
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // Reset lengths for actual translation
    actual_inlen = inlen;
    actual_outlen = actual_outlen;  // Keep the same size
    
    // Create typeform array if we have a typeform value
    formtype *typeform_array = nullptr;
    if (typeform_value != 0) {
        typeform_array = (formtype *)malloc(actual_inlen * sizeof(formtype));
        if (typeform_array) {
            for (int i = 0; i < actual_inlen; i++) {
                typeform_array[i] = typeform_value;
            }
        }
    }
    
    // Create spacing array
    char *spacing_array = (char *)malloc(actual_inlen * sizeof(char));
    if (spacing_array) {
        memset(spacing_array, 0, actual_inlen * sizeof(char));
    }
    
    // Perform translation
    result = lou_translateString(selected_table, inbuf, &actual_inlen, 
                                 outbuf, &actual_outlen, 
                                 typeform_array, spacing_array, fuzz_int32_t_2);
    
    // Step 8: Clean up allocated resources
    free(outbuf);
    free(typeform_array);
    free(spacing_array);
    lou_freeEmphClasses(emph_classes);
    lou_freeTableFiles(tables);
    
    return 0;
}