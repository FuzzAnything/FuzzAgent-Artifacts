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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"lou_logFile":[[201,26,201,42,0,0,4,0]]},"library_calls":["lou_getDataPath","lou_setDataPath","lou_logFile","lou_findTables","lou_logEnd","lou_setDataPath","lou_freeTableFiles","lou_logEnd","lou_setDataPath","lou_getTable","lou_getEmphClasses","lou_freeEmphClasses","lou_freeTableFile","lou_freeTableFiles","lou_logEnd","lou_setDataPath","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_freeTableFiles","lou_logEnd","lou_setDataPath"],"critical_calls":["lou_getDataPath","lou_setDataPath","lou_logFile","lou_findTables","lou_getTable","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_dotsToChar","lou_hyphenate","lou_backTranslate","lou_freeTableFile","lou_freeTableFiles","lou_logEnd","lou_setDataPath"],"visited":1}
/**/


// Include necessary headers for liblouis
extern "C" {
}

// Custom types from liblouis
typedef unsigned short widechar;
typedef unsigned short formtype;

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 2) {
	return 0;
	}
	

    // 1. Setup: Initialize paths and logging
    char *original_path = lou_getDataPath();  // Store original path
    lou_setDataPath(".");  // Set current directory as data path
    
    // Use input data as a log file name (truncate if too long)
    char log_filename[256] = "fuzz.log";
    if (size < 256) {
        memcpy(log_filename, data, size);
        log_filename[size] = '\0';
    }
    lou_logFile(log_filename);

    // 2. Find and load braille tables
    char **tables = lou_findTables("*.ctb,*.utb,*.tbl");  // Find braille tables
    if (!tables) {
        lou_logEnd();
        lou_setDataPath(original_path);
        return 0;
    }

    // Use first available table
    const char *table_list = tables[0];
    if (!table_list) {
        lou_freeTableFiles(tables);
        lou_logEnd();
        lou_setDataPath(original_path);
        return 0;
    }

    // 3. Get table and emphasis classes
    const void *table = lou_getTable(table_list);
    const char **emph_classes = lou_getEmphClasses(table_list);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }

    // 4. Process input data as wide characters
    // Convert byte stream to wide characters (UTF-16 like)
    size_t wide_len = size / sizeof(widechar);
    if (wide_len < 1) {
        lou_freeTableFile(const_cast<char*>(table_list));
        lou_freeTableFiles(tables);
        lou_logEnd();
        lou_setDataPath(original_path);
        return 0;
    }

    std::vector<widechar> inbuf(wide_len);
    memcpy(inbuf.data(), data, wide_len * sizeof(widechar));

    // 5. Perform char-to-dots conversion (text to braille)
    std::vector<widechar> dot_buf(wide_len * 2);  // Braille can be longer
    int dots_result = lou_charToDots(table_list, inbuf.data(), dot_buf.data(), 
                                     static_cast<int>(wide_len), 0);

    // 6. Perform dots-to-char conversion (braille to text)
    std::vector<widechar> char_buf(wide_len * 2);
    if (dots_result > 0) {
        lou_dotsToChar(table_list, dot_buf.data(), char_buf.data(), 
                      dots_result, 0);
    }

    // 7. Hyphenation
    std::vector<char> hyphens(wide_len + 1);
    int hyphen_result = lou_hyphenate(table_list, inbuf.data(), 
                                     static_cast<int>(wide_len), 
                                     hyphens.data(), 0);

    // 8. Back translation (braille back to text)
    int inlen = static_cast<int>(wide_len);
    int outlen = wide_len * 2;
    std::vector<widechar> back_buf(outlen);
    std::vector<int> outputPos(outlen), inputPos(outlen), cursorPos(outlen);
    std::vector<formtype> typeform(wide_len);
    std::vector<char> spacing(wide_len);
    
    lou_backTranslate(table_list, inbuf.data(), &inlen, back_buf.data(), &outlen,
                      typeform.data(), spacing.data(), 
                      outputPos.data(), inputPos.data(), cursorPos.data(), 0);

    // 9. Cleanup
    lou_freeTableFile(const_cast<char*>(table_list));
    lou_freeTableFiles(tables);
    lou_logEnd();
    lou_setDataPath(original_path);  // Restore original path

    return 0;
}