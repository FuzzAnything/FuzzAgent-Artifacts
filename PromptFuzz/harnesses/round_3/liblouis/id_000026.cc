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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_getDataPath","lou_listTables","lou_checkTable","lou_freeTableFiles","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_freeTableFiles","lou_translateString","lou_translateString","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"critical_calls":["lou_getDataPath","lou_listTables","lou_checkTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_translateString","lou_translateString","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"visited":0}
/*Looking at the requirements, I need to create a fuzz driver that uses the specified liblouis APIs to process input data. The event I'll simulate is: **Loading braille translation tables and performing text translation with emphasis/typeform handling**.

Here's the step-by-step implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 4) {
        return 0;
    }
    
    // API 1: Get the data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Data path is available, but we don't need to do anything with it
        // Just ensuring the API is called
    }
    
    // API 2: List available tables
    char **table_list = lou_listTables();
    if (!table_list) {
        return 0;  // No tables available
    }
    
    // Find a valid table to use
    const char *selected_table = nullptr;
    for (int i = 0; table_list[i] != nullptr; i++) {
        // API 3: Check if table is valid
        if (lou_checkTable(table_list[i]) == 0) {  // 0 means table is valid
            selected_table = table_list[i];
            break;
        }
    }
    
    if (!selected_table) {
        lou_freeTableFiles(table_list);
        return 0;  // No valid tables found
    }
    
    // API 4: Get emphasis classes for the selected table
    const char **emph_classes = lou_getEmphClasses(selected_table);
    
    // API 5: Get typeform for the first emphasis class (if available)
    formtype typeform_value = 0;
    if (emph_classes && emph_classes[0]) {
        typeform_value = lou_getTypeformForEmphClass(selected_table, emph_classes[0]);
    }
    
    // Prepare input buffer from fuzzer data
    // We'll use the input data as widechar text to translate
    size_t input_len = size / sizeof(widechar);
    if (input_len == 0) {
        if (emph_classes) {
            lou_freeEmphClasses(emph_classes);
        }
        lou_freeTableFiles(table_list);
        return 0;
    }
    
    // Create input buffer (convert bytes to widechars)
    std::vector<widechar> inbuf(input_len);
    for (size_t i = 0; i < input_len; i++) {
        inbuf[i] = (widechar)(data[i * sizeof(widechar)] << 8) | 
                   (widechar)(data[i * sizeof(widechar) + 1]);
    }
    
    // Prepare output buffer (allocate 4x input size for safety)
    int outbuf_size = input_len * 4;
    std::vector<widechar> outbuf(outbuf_size);
    
    // Prepare typeform array (all characters get the same typeform)
    std::vector<formtype> typeform(input_len, typeform_value);
    
    // Prepare spacing array
    std::vector<char> spacing(input_len, 0);
    
    // Set input/output lengths
    int inlen = static_cast<int>(input_len);
    int outlen = outbuf_size;
    
    // API 6: Translate the string
    int translation_result = lou_translateString(
        selected_table,           // tableList
        inbuf.data(),             // inbuf
        &inlen,                   // inlen (in/out)
        outbuf.data(),            // outbuf
        &outlen,                  // outlen (in/out)
        typeform.data(),          // typeform
        spacing.data(),           // spacing
        fuzz_int32_t_1                         // mode (0 = no contractions)
    );
    
    // Also test with a different mode if we have enough data
    if (size > 100) {
        // Reset lengths for second translation attempt
        inlen = static_cast<int>(input_len);
        outlen = outbuf_size;
        
        // Try translation with a different mode
        lou_translateString(
            selected_table,
            inbuf.data(),
            &inlen,
            outbuf.data(),
            &outlen,
            typeform.data(),
            spacing.data(),
            fuzz_int32_t_2  // mode 1 = partial translation
        );
    }
    
    // Test dotsToChar and charToDots if we have enough data
    if (input_len > 10) {
        std::vector<widechar> dots_buffer(input_len);
        std::vector<widechar> char_buffer(input_len);
        
        // Convert characters to dots
        lou_charToDots(
            selected_table,
            inbuf.data(),
            dots_buffer.data(),
            static_cast<int>(std::min<size_t>(input_len, 10)),
            fuzz_int32_t_3
        );
        
        // Convert dots back to characters
        lou_dotsToChar(
            selected_table,
            dots_buffer.data(),
            char_buffer.data(),
            static_cast<int>(std::min<size_t>(input_len, 10)),
            fuzz_int32_t_4
        );
    }
    
    // Test hyphenation if we have enough data
    if (input_len > 5) {
        std::vector<char> hyphens(input_len);
        lou_hyphenate(
            selected_table,
            inbuf.data(),
            static_cast<int>(std::min<size_t>(input_len, 20)),
            hyphens.data(),
            fuzz_int32_t_5
        );
    }
    
    // Clean up
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    lou_freeTableFiles(table_list);
    
    // Call lou_free to clean up internal resources
    lou_free();
    
    return 0;
}