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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.6666665, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"freeTranslationTable":[[4932,18,4932,38,0,0,4,1]]},"library_calls":["lou_registerLogCallback","lou_listTables","lou_free","lou_checkTable","lou_freeTableFiles","lou_free","lou_logPrint","lou_freeTableFiles","lou_free","lou_translateString","lou_logPrint","lou_backTranslate","lou_logPrint","lou_freeTableFiles","lou_free"],"critical_calls":["lou_registerLogCallback","lou_listTables","lou_checkTable","lou_logPrint","lou_translateString","lou_logPrint","lou_backTranslate","lou_logPrint","lou_freeTableFiles","lou_free"],"visited":2}
/**/


// Include liblouis headers
extern "C" {
}

// Custom log callback function
static void  id_000012_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing - just to demonstrate registration
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 1: Register log callback
    lou_registerLogCallback( id_000012_log_callback);
    
    // Step 2: List available tables
    char **tables = lou_listTables();
    if (!tables) {
        lou_free();
        return 0;
    }
    
    // Step 3: Find a valid table to use
    const char *table_to_use = nullptr;
    for (int i = 0; tables[i] != nullptr; i++) {
        if (lou_checkTable(tables[i]) == 0) {
            table_to_use = tables[i];
            break;
        }
    }
    
    // If no valid table found, clean up and return
    if (!table_to_use) {
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    // Step 4: Log the table we're using
    char log_message[256];
    snprintf(log_message, sizeof(log_message), "Using table: %s", table_to_use);
    lou_logPrint(log_message);
    
    // Step 5: Prepare input buffer from fuzzer data
    // We need to convert byte data to widechar format
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        lou_freeTableFiles(tables);
        lou_free();
        return 0;
    }
    
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));
    
    // Step 6: Prepare variables for translation
    int inlen = static_cast<int>(widechar_count);
    int outlen = inlen * 2; // Allocate enough space for output
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(outlen);
    formtype typeform = 0;
    int mode = 0;
    
    // Step 7: Perform forward translation
    int trans_result = lou_translateString(
        table_to_use,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        fuzz_int32_t_1
    );
    
    // Log translation result
    snprintf(log_message, sizeof(log_message), 
             "Translation result: %d, inlen: %d, outlen: %d", 
             trans_result, inlen, outlen);
    lou_logPrint(log_message);
    
    // Step 8: Prepare for back translation
    if (trans_result == 0 && outlen > 0) {
        int back_inlen = outlen;
        int back_outlen = inlen * 2;
        std::vector<widechar> back_outbuf(back_outlen);
        std::vector<char> back_spacing(back_outlen);
        int outputPos = 0, inputPos = 0, cursorPos = 0;
        
        // Step 9: Perform back translation
        int back_result = lou_backTranslate(
            table_to_use,
            outbuf.data(),
            &back_inlen,
            back_outbuf.data(),
            &back_outlen,
            &typeform,
            back_spacing.data(),
            &outputPos,
            &inputPos,
            &cursorPos,
            fuzz_int32_t_2
        );
        
        // Log back translation result
        snprintf(log_message, sizeof(log_message),
                 "Back translation result: %d, inlen: %d, outlen: %d",
                 back_result, back_inlen, back_outlen);
        lou_logPrint(log_message);
    }
    
    // Step 10: Clean up
    lou_freeTableFiles(tables);
    lou_free();
    
    return 0;
}