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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["lou_registerLogCallback","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_getTableInfo","lou_freeTableInfo","lou_freeTableFiles","lou_charToDots","lou_backTranslateString","lou_checkTable","lou_freeTableFiles"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_charToDots","lou_backTranslateString","lou_checkTable","lou_freeTableFiles"],"visited":1}
/**/


extern "C" {
}

// Define log callback function
static void  id_000041_logCallback(logLevels level, const char *message) {
    // Simple callback that does nothing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 2) {
        return 0; // Need at least some data
    }

    // 1. Register log callback
    lou_registerLogCallback( id_000041_logCallback);

    // 2. List available tables
    char **tables = lou_listTables();
    if (!tables) {
        return 0;
    }

    // Find first valid table
    const char *selected_table = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {
            selected_table = tables[i];
            break;
        }
    }

    if (!selected_table) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // 3. Get table info
    char *table_info = lou_getTableInfo(selected_table, fuzz_str_1);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // 4. Prepare input buffer from fuzzer data
    // Convert data to widechars (liblouis uses 16-bit widechars)
    size_t widechar_count = size / 2;
    if (widechar_count == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }

    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // 5. Perform charToDots conversion
    std::vector<widechar> dotsbuf(widechar_count * 2); // Dots might be larger
    int dots_result = lou_charToDots(selected_table, inbuf.data(), 
                                     dotsbuf.data(), widechar_count, 0);
    
    // 6. Prepare for back translation
    if (dots_result > 0) {
        int inlen = dots_result; // Number of dots characters produced
        int outlen = widechar_count * 2; // Allocate enough space
        
        std::vector<widechar> outbuf(outlen);
        std::vector<formtype> typeform(inlen);
        std::vector<char> spacing(inlen);
        
        // Clear typeform and spacing (not used in this simple test)
        memset(typeform.data(), 0, typeform.size() * sizeof(formtype));
        memset(spacing.data(), 0, spacing.size());
        
        // 7. Perform back translation
        lou_backTranslateString(selected_table, dotsbuf.data(), &inlen,
                               outbuf.data(), &outlen,
                               typeform.data(), spacing.data(), 0);
    }

    // 8. Check the table one more time
    lou_checkTable(selected_table);

    // 9. Cleanup
    lou_freeTableFiles(tables);
    
    return 0;
}