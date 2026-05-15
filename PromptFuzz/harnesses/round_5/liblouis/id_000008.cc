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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 2.5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["lou_logFile","lou_registerTableResolver","lou_indexTables","lou_readCharFromFile","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_dotsToChar","lou_backTranslate","lou_free"],"critical_calls":["lou_logFile","lou_registerTableResolver","lou_indexTables","lou_readCharFromFile","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_dotsToChar","lou_backTranslate","lou_free"],"visited":1}
/**/


// Include liblouis headers
extern "C" {
}

// Custom table resolver function
extern "C" char **custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL - can be fuzzed
    (void)table;
    (void)base;
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        return 0;
    }

    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Write the data to a file for lou_readCharFromFile
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;

    // 3. Set up logging
    lou_logFile("fuzz_log.txt");

    // 4. Register table resolver
    lou_registerTableResolver(custom_table_resolver);

    // 5. Index tables - using a simple table list
    const char *tables[] = {"en-us-g2.ctb", NULL};
    lou_indexTables(tables);

    // 6. Read character from file
    int mode = 0;
    int read_char_result = lou_readCharFromFile("input_file", &mode);
    (void)read_char_result; // Result may be -1 for invalid files

    // 7. Prepare table list for other operations
    const char *tableList = "en-us-g2.ctb";

    // 8. Get emphasis classes
    const char **emph_classes = lou_getEmphClasses(tableList);
    if (emph_classes) {
        // Free the emphasis classes
        lou_freeEmphClasses(emph_classes);
    }

    // 9. Prepare buffers for conversion operations
    // Use a portion of the input data for conversion
    size_t usable_size = size > 1024 ? 1024 : size;
    std::vector<widechar> inbuf(usable_size / 2 + 1);
    std::vector<widechar> outbuf(usable_size / 2 + 1);
    
    // Convert input bytes to widechars
    for (size_t i = 0; i < usable_size / 2 && i * 2 + 1 < usable_size; i++) {
        inbuf[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    int length = static_cast<int>(usable_size / 2);

    // 10. Test dotsToChar and charToDots conversions
    if (length > 0) {
        // First convert characters to dots
        int dots_result = lou_charToDots(tableList, inbuf.data(), outbuf.data(), length, 0);
        
        if (dots_result > 0) {
            // Then convert dots back to characters
            std::vector<widechar> roundtrip_buf(length);
            int chars_result = lou_dotsToChar(tableList, outbuf.data(), roundtrip_buf.data(), dots_result, 0);
            (void)chars_result;
        }
    }

    // 11. Test backTranslate with the input data
    if (length > 0) {
        int inlen = length;
        int outlen = length * 2; // Allow space for expansion
        std::vector<widechar> back_outbuf(outlen);
        std::vector<int> outputPos(outlen);
        std::vector<int> inputPos(outlen);
        std::vector<int> cursorPos(1);
        std::vector<char> spacing(outlen);
        std::vector<formtype> typeform(outlen, 0);

        int back_result = lou_backTranslate(
            tableList,
            inbuf.data(),
            &inlen,
            back_outbuf.data(),
            &outlen,
            typeform.data(),
            spacing.data(),
            outputPos.data(),
            inputPos.data(),
            cursorPos.data(),
            0
        );
        (void)back_result;
    }

    // 12. Clean up
    assert_file_closed(&in_file);;
    remove("input_file");
    remove("fuzz_log.txt");
    remove("output_file");

    // Free liblouis resources
    lou_free();

    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}