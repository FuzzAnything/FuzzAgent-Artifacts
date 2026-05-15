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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 64
//<Quality> {"density":8,"unique_branches":{"back_selectRule":[[782,11,782,21,0,0,4,0],[837,11,838,53,0,0,4,1],[885,29,885,62,0,0,4,1]],"backTranslateString":[[1304,3,1304,22,0,0,4,0],[1305,3,1305,23,0,0,4,0],[1306,3,1306,23,0,0,4,0],[1309,3,1309,25,0,0,4,0],[1310,3,1310,25,0,0,4,0],[1316,5,1316,24,0,0,4,0],[1320,5,1320,25,0,0,4,0],[1324,5,1324,25,0,0,4,0],[1330,10,1330,42,0,0,4,1],[1332,17,1332,51,0,0,4,0],[1332,17,1332,51,0,0,4,1],[1336,5,1336,27,0,0,4,0],[1340,5,1340,27,0,0,4,0],[1344,5,1344,21,0,0,4,1],[1447,7,1447,22,0,0,4,0],[1447,26,1447,59,0,0,4,0],[1447,26,1447,59,0,0,4,1],[1452,31,1452,49,0,0,4,0],[1452,31,1452,49,0,0,4,1]],"findEmphasisClass":[[529,7,529,54,0,0,4,0],[533,7,533,55,0,0,4,0],[537,7,537,55,0,0,4,0],[541,7,541,57,0,0,4,0],[545,7,545,63,0,0,4,0],[549,7,549,62,0,0,4,0],[553,7,553,51,0,0,4,1],[557,7,557,51,0,0,4,1]],"_lou_backTranslate":[[250,6,250,23,0,0,4,0],[251,15,251,31,0,0,4,0],[251,15,251,31,0,0,4,1],[252,6,252,23,0,0,4,0],[257,6,257,21,0,0,4,0],[258,6,258,22,0,0,4,0],[353,6,353,22,0,0,4,0],[356,15,356,25,0,0,4,0],[356,15,356,25,0,0,4,1],[357,8,357,30,0,0,4,0],[357,8,357,30,0,0,4,1],[358,12,358,34,0,0,4,0],[358,12,358,34,0,0,4,1],[359,10,359,21,0,0,4,0],[359,10,359,21,0,0,4,1],[359,25,359,41,0,0,4,0],[360,26,360,35,0,0,4,1],[365,7,365,17,0,0,4,0],[365,7,365,17,0,0,4,1],[366,10,366,26,0,0,4,0],[366,10,366,26,0,0,4,1],[368,6,368,23,0,0,4,0],[369,15,369,25,0,0,4,0],[369,15,369,25,0,0,4,1],[370,8,370,25,0,0,4,1],[372,13,372,40,0,0,4,0],[372,13,372,40,0,0,4,1],[377,6,377,23,0,0,4,0],[377,27,377,43,0,0,4,0],[378,7,378,24,0,0,4,0]],"back_updatePositions":[[1012,6,1012,20,0,0,4,1],[1012,24,1012,46,0,0,4,0],[1012,50,1012,84,0,0,4,0]],"insertSpace":[[1072,6,1072,14,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_findTable","lou_getEmphClasses","lou_freeEmphClasses","lou_freeTableFile","lou_freeTableFile","lou_backTranslate","lou_charToDots","lou_freeTableFile"],"critical_calls":["lou_registerLogCallback","lou_findTable","lou_getEmphClasses","lou_freeEmphClasses","lou_backTranslate","lou_charToDots","lou_freeTableFile"],"visited":1}
/**/


// Include liblouis headers

// Dummy log callback function
void  id_000050_dummy_log_callback(logLevels level, const char* message) {
    // Do nothing - just a placeholder for logging
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small
    if (size < 4) {
        return 0;
    }

    // 1. Register log callback first
    lou_registerLogCallback( id_000050_dummy_log_callback);

    // 2. Use part of the input to create a table query
    // We'll use first 100 bytes or less if size is smaller
    size_t query_size = std::min(size, (size_t)100);
    char* table_query = static_cast<char*>(malloc(query_size + 1));
    if (!table_query) {
	return 0;
	}
	
    
    memcpy(table_query, data, query_size);
    table_query[query_size] = '\0';
    
    // Consume the used bytes
    data += query_size;
    size -= query_size;
    
    if (size < 2) {
        free(table_query);
        return 0;
    }

    // 3. Find a braille table using the query
    char* found_table = lou_findTable(table_query);
    free(table_query);
    
    if (!found_table) {
        // If no table found, use a default table
        found_table = strdup("en-us-g2.ctb");
        if (!found_table) return 0;
    }

    // 4. Get emphasis classes for the found table
    const char** emph_classes = lou_getEmphClasses(found_table);
    if (emph_classes) {
        // Just use the first emphasis class if available
        for (int i = 0; emph_classes[i] != nullptr; i++) {
            // We could use the emphasis class, but for fuzzing we just consume
            break;
        }
        lou_freeEmphClasses(emph_classes);
    }

    // 5. Prepare input for back translation
    // We need at least some data for translation
    if (size < 4) {
        lou_freeTableFile(found_table);
        return 0;
    }

    // Convert the remaining data to widechars (assuming it's already in widechar format)
    // We'll interpret the data as widechar array
    int inlen = static_cast<int>(size / sizeof(widechar));
    if (inlen < 1) {
        lou_freeTableFile(found_table);
        return 0;
    }

    const widechar* inbuf = reinterpret_cast<const widechar*>(data);
    
    // Allocate output buffers
    int outlen = inlen * 2;  // Allow some extra space
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform(outlen);
    std::vector<char> spacing(outlen);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(outlen);
    int cursorPos = 0;
    
    // 6. Perform back translation
    int back_translate_result = lou_backTranslate(
        found_table,
        inbuf,
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        &cursorPos,
        0
    );

    // 7. If back translation succeeded, convert the result to dots
    if (back_translate_result > 0 && outlen > 0) {
        // Allocate buffer for dots conversion
        std::vector<widechar> dotsbuf(outlen);
        
        // Perform char to dots conversion
        int dots_result = lou_charToDots(
            found_table,
            outbuf.data(),
            dotsbuf.data(),
            outlen,
            0
        );
        
        // The result is not used, but the API was called
        (void)dots_result;
    }

    // 8. Clean up
    lou_freeTableFile(found_table);
    
    // Note: lou_registerLogCallback doesn't need cleanup
    // The  id_000050_dummy_log_callback will persist, but that's OK for fuzzing
    
    return 0;
}