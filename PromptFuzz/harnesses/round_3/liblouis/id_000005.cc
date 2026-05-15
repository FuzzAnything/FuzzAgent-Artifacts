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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.6666665, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"lou_findTables":[[1121,23,1121,24,0,0,4,0],[1124,7,1124,19,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_getDataPath","lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_freeTableFiles","lou_backTranslateString","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_getDataPath","lou_findTables","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_backTranslateString","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"visited":2}
/**/


// Include liblouis headers

// Custom log callback function
static void  id_000005_log_callback(logLevels level, const char *message) {
    // Simple log callback that does nothing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // 1. Register log callback
    lou_registerLogCallback( id_000005_log_callback);
    
    // 2. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Could use data_path if needed
        free(data_path);
    }
    
    // 3. Find available tables
    char **tables = lou_findTables("*");
    if (!tables) {
        return 0;
    }
    
    // Use the first table found
    const char *table = tables[0];
    if (!table) {
        lou_freeTableFiles(tables);
        return 0;
    }
    
    // 4. Get emphasis classes for the table
    const char **emph_classes = lou_getEmphClasses(table);
    
    // 5. Get typeform for first emphasis class (if available)
    formtype typeform_value = 0;
    if (emph_classes && emph_classes[0]) {
        typeform_value = lou_getTypeformForEmphClass(table, emph_classes[0]);
    }
    
    // 6. Prepare input buffer for back-translation
    // Convert byte data to widechar (assuming UTF-16LE encoding)
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        if (emph_classes) lou_freeEmphClasses(emph_classes);
        lou_freeTableFiles(tables);
        return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // 7. Prepare output buffer
    std::vector<widechar> outbuf(widechar_count * 2); // Allocate extra space
    int inlen = static_cast<int>(widechar_count);
    int outlen = static_cast<int>(outbuf.size());
    
    // 8. Prepare typeform array (all elements set to the same typeform)
    std::vector<formtype> typeform_array(widechar_count, typeform_value);
    
    // 9. Prepare spacing array
    std::vector<char> spacing_array(widechar_count, 0);
    
    // 10. Perform back-translation
    int result = lou_backTranslateString(
        table,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform_array.data(),
        spacing_array.data(),
        fuzz_int32_t_1  // mode
    );
    
    // Clean up
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    lou_freeTableFiles(tables);
    lou_free();
    
    return 0;
}