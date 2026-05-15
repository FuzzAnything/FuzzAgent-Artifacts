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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_listTables","lou_freeTableFiles","lou_registerLogCallback","lou_registerTableResolver","lou_checkTable","lou_findTable","lou_freeTableFile","lou_getTypeformForEmphClass","lou_translateString","lou_hyphenate","lou_charToDots","lou_logEnd"],"critical_calls":["lou_listTables","lou_freeTableFiles","lou_registerLogCallback","lou_registerTableResolver","lou_checkTable","lou_findTable","lou_freeTableFile","lou_getTypeformForEmphClass","lou_translateString","lou_hyphenate","lou_charToDots","lou_logEnd"],"visited":0}
/**/


// liblouis headers
extern "C" {
}

// Custom log callback
static void  id_000014_log_callback(logLevels level, const char *message) {
    // Simple callback that does nothing
    (void)level;
    (void)message;
}

// Custom table resolver
static char ** id_000014_table_resolver(const char *table_name, const char *base) {
    // Always return NULL - let liblouis handle table resolution
    (void)table_name;
    (void)base;
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Early exit for too small inputs
    if (size < sizeof(widechar) + 1) {
        return 0;
    }

    // Use a file handle to read input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse data structure:
    // 1. First byte: mode parameter
    // 2. Next bytes: table list string (null-terminated)
    // 3. Remaining: widechar buffer for input text
    
    int mode = 0;
    if (fread(&mode, 1, 1, in_file) != 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    mode = mode % 3; // Limit mode to valid values

    // Read table list string
    std::vector<char> table_list;
    int c;
    while ((c = fgetc(in_file)) != EOF && c != 0) {
        table_list.push_back(static_cast<char>(c));
    }
    table_list.push_back('\0');
    
    if (table_list.size() <= 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read remaining data as widechar buffer
    std::vector<widechar> inbuf;
    while (true) {
        widechar wc = 0;
        if (fread(&wc, sizeof(widechar), 1, in_file) != 1) {
            break;
        }
        inbuf.push_back(wc);
    }
    assert_file_closed(&in_file);;
    
    if (inbuf.empty()) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Determine input length
    int inlen = inbuf.size();
    if (inlen <= 0) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Event: Perform various braille translation operations
    
    // 1. lou_listTables - list available tables
    char **tables = lou_listTables();
    if (tables) {
        lou_freeTableFiles(tables);
    }

    // 2. lou_registerLogCallback - register log callback
    lou_registerLogCallback( id_000014_log_callback);
    
    // 3. lou_registerTableResolver - register table resolver
    lou_registerTableResolver( id_000014_table_resolver);
    
    // 4. lou_checkTable - check if table is valid
    lou_checkTable(table_list.data());
    
    // 5. lou_findTable - find a table
    char *found_table = lou_findTable(fuzz_str_1);
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // 6. lou_getTypeformForEmphClass - get typeform for emphasis class
    // Try with a common emphasis class
    formtype typeform = lou_getTypeformForEmphClass(table_list.data(), fuzz_str_2);
    (void)typeform; // Use variable to avoid warning
    
    // 7. lou_translateString - translate string
    int outlen = inlen * 2; // Allocate enough space
    std::vector<widechar> outbuf(outlen);
    std::vector<char> spacing(outlen);
    formtype typeform_arr = 0;
    
    int trans_result = lou_translateString(
        table_list.data(),
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform_arr,
        spacing.data(),
        fuzz_int32_t_3
    );
    
    // If translation succeeded, use the result for other operations
    if (trans_result > 0) {
        // 8. lou_hyphenate - hyphenate the input
        std::vector<char> hyphens(inlen);
        int hyphen_result = lou_hyphenate(
            table_list.data(),
            inbuf.data(),
            inlen,
            hyphens.data(),
            fuzz_int32_t_4
        );
        (void)hyphen_result; // Use variable to avoid warning
        
        // 9. lou_charToDots - convert characters to dots
        std::vector<widechar> dotsbuf(inlen);
        int dots_result = lou_charToDots(
            table_list.data(),
            inbuf.data(),
            dotsbuf.data(),
            inlen,
            mode
        );
        (void)dots_result; // Use variable to avoid warning
    }
    
    // Clean up
    lou_logEnd();
    
    assert_file_closed(&in_file);
	return 0;
}