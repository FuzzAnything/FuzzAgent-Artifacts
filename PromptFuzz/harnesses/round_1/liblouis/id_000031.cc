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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"_lou_translate":[[1186,62,1186,76,0,0,4,0]]},"library_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_readCharFromFile","lou_getTableInfo","lou_freeTableInfo","lou_findTables","lou_freeTableFiles","lou_getTypeformForEmphClass","lou_translate","lou_translateString","lou_backTranslate","lou_free"],"critical_calls":["lou_registerLogCallback","lou_registerTableResolver","lou_readCharFromFile","lou_getTableInfo","lou_freeTableInfo","lou_findTables","lou_freeTableFiles","lou_getTypeformForEmphClass","lou_translate","lou_translateString","lou_backTranslate","lou_free"],"visited":1}
/**/


// Include necessary liblouis headers

// Simple log callback function
static void  id_000031_fuzz_log_callback(logLevels level, const char *message) {
    // Do nothing in fuzzing - just satisfy the API requirement
    (void)level;
    (void)message;
}

// Simple table resolver function
static char ** id_000031_fuzz_table_resolver(const char *table, const char *base) {
    // Return NULL to let liblouis use default resolver
    (void)table;
    (void)base;
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        // Not enough data to process
        return 0;
    }

    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register log callback
    lou_registerLogCallback( id_000031_fuzz_log_callback);

    // Register table resolver
    lou_registerTableResolver( id_000031_fuzz_table_resolver);

    // Read a character using lou_readCharFromFile
    // First, write the input data to a temporary file
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        int mode = 0;
        int result = lou_readCharFromFile("input_file", &mode);
        // Result is ignored as we're fuzzing
        (void)result;
    }

    // Parse the input data to get table name and input text
    // First byte: table name length (1-127)
    size_t offset = 0;
    uint8_t table_len = data[offset++];
    
    if (table_len == 0 || offset + table_len >= size) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Extract table name
    std::string table_name(reinterpret_cast<const char*>(data + offset), 
                          std::min<size_t>(table_len, size - offset));
    offset += table_len;

    // Get table info using the extracted table name
    char *table_info = lou_getTableInfo(table_name.c_str(), fuzz_str_1);
    if (table_info) {
        lou_freeTableInfo(table_info);
    }

    // Find tables using a query based on first few bytes
    std::string query = "table";
    char **found_tables = lou_findTables(query.c_str());
    if (found_tables) {
        lou_freeTableFiles(found_tables);
    }

    // Get typeform for emphasis class
    formtype emph_typeform = lou_getTypeformForEmphClass(table_name.c_str(), fuzz_str_2);
    (void)emph_typeform;

    // Prepare input buffer (remaining data)
    if (offset >= size) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    size_t remaining_size = size - offset;
    std::vector<widechar> inbuf(remaining_size / sizeof(widechar) + 1);
    
    // Convert bytes to widechars
    size_t widechars_to_copy = std::min(remaining_size / sizeof(widechar), 
                                       inbuf.size() - 1);
    memcpy(inbuf.data(), data + offset, widechars_to_copy * sizeof(widechar));
    
    int inlen = static_cast<int>(widechars_to_copy);
    int outlen = inlen * 2;  // Allocate enough space for output
    
    std::vector<widechar> outbuf(outlen);
    std::vector<widechar> back_outbuf(inlen);
    
    // Variables for translation functions
    formtype typeform = 0;
    std::vector<char> spacing(inlen, 0);
    std::vector<int> outputPos(outlen);
    std::vector<int> inputPos(outlen);
    std::vector<int> cursorPos(1, 0);
    int outputPos2 = 0, inputPos2 = 0, cursorPos2 = 0;

    // Perform translation using lou_translate
    int trans_result = lou_translate(
        table_name.c_str(),
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        &typeform,
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_3  // mode
    );

    // Also try lou_translateString
    int inlen2 = static_cast<int>(widechars_to_copy);
    int outlen2 = inlen2 * 2;
    std::vector<widechar> outbuf2(outlen2);
    
    int trans_string_result = lou_translateString(
        table_name.c_str(),
        inbuf.data(),
        &inlen2,
        outbuf2.data(),
        &outlen2,
        &typeform,
        spacing.data(),
        fuzz_int32_t_4  // mode
    );

    // If translation was successful, try back-translation
    if (trans_result > 0 && outlen > 0) {
        int back_inlen = outlen;
        int back_outlen = inlen;
        
        lou_backTranslate(
            table_name.c_str(),
            outbuf.data(),
            &back_inlen,
            back_outbuf.data(),
            &back_outlen,
            &typeform,
            spacing.data(),
            &outputPos2,
            &inputPos2,
            &cursorPos2,
            0  // mode
        );
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    // Free liblouis resources
    lou_free();

    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}