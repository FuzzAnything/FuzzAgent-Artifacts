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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["lou_checkTable","lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_logPrint","lou_translateString","lou_backTranslate","lou_free"],"critical_calls":["lou_checkTable","lou_registerLogCallback","lou_listTables","lou_freeTableFiles","lou_logPrint","lou_translateString","lou_backTranslate","lou_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to achieve a round-trip translation event with logging:

*/


// Include liblouis headers (assuming they're available)
extern "C" {
}

// Log callback function
static void  id_000013_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing to avoid output spam during fuzzing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Split input into two parts: table list and translation data
    size_t split_point = size / 2;
    if (split_point == 0) split_point = 1;
    
    // First part: table list (null-terminated string)
    std::string table_list(reinterpret_cast<const char*>(data), split_point);
    // Ensure null termination
    if (table_list.empty()) {
        table_list = "en-us-g2.ctb";
    } else {
        // Replace any null bytes in the middle to ensure single null termination
        for (size_t i = 0; i < table_list.size() - 1; ++i) {
            if (table_list[i] == '\0') {
                table_list[i] = ' ';
            }
        }
        // Ensure the string is null-terminated
        table_list += '\0';
        table_list = table_list.substr(0, table_list.find('\0') + 1);
    }

    // Second part: translation input (wide character data)
    size_t translation_data_size = size - split_point;
    const uint8_t* translation_data = data + split_point;
    
    // Convert to wide characters (assuming UTF-16LE)
    std::vector<widechar> inbuf;
    if (translation_data_size >= sizeof(widechar)) {
        size_t widechar_count = translation_data_size / sizeof(widechar);
        inbuf.resize(widechar_count);
        memcpy(inbuf.data(), translation_data, widechar_count * sizeof(widechar));
    } else {
        // If not enough data for widechars, use some default
        inbuf = {L'h', L'e', L'l', L'l', L'o'};
    }
    
    // 1. Check the table first
    int table_check = lou_checkTable(table_list.c_str());
    if (table_check == 0) {
        // Table is valid, proceed with translation
        
        // 2. Register log callback
        lou_registerLogCallback( id_000013_log_callback);
        
        // 3. List available tables (and free them)
        char **tables = lou_listTables();
        if (tables) {
            lou_freeTableFiles(tables);
        }
        
        // 4. Log a message
        lou_logPrint("Starting translation test");
        
        // Prepare buffers for translation
        int inlen = static_cast<int>(inbuf.size());
        int outlen = inlen * 2;  // Conservative estimate
        std::vector<widechar> outbuf(outlen);
        std::vector<char> spacing(inlen);
        formtype typeform = 0;
        
        // 5. Translate string
        int trans_result = lou_translateString(
            table_list.c_str(),
            inbuf.data(),
            &inlen,
            outbuf.data(),
            &outlen,
            &typeform,
            spacing.data(),
            fuzz_int32_t_1
        );
        
        if (trans_result != 0 && outlen > 0) {
            // 6. Back-translate
            int back_inlen = outlen;
            int back_outlen = inlen * 2;
            std::vector<widechar> back_outbuf(back_outlen);
            std::vector<char> back_spacing(back_inlen);
            int outputPos[2] = {0, 0};
            int inputPos[2] = {0, 0};
            int cursorPos = 0;
            
            int back_result = lou_backTranslate(
                table_list.c_str(),
                outbuf.data(),
                &back_inlen,
                back_outbuf.data(),
                &back_outlen,
                &typeform,
                back_spacing.data(),
                outputPos,
                inputPos,
                &cursorPos,
                fuzz_int32_t_2
            );
            
            (void)back_result;  // Use result to avoid unused variable warning
        }
    }
    
    // 7. Clean up liblouis resources
    lou_free();
    
    return 0;
}