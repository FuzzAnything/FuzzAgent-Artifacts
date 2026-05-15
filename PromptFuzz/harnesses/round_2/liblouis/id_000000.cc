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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 73.333336, nr_unique_branch: 230
//<Quality> {"density":5,"unique_branches":{"_lou_getALine":[[346,7,346,15,0,0,4,0]],"isAlpha":[[268,10,268,18,0,0,4,0],[268,10,268,18,0,0,4,1],[268,22,268,30,0,0,4,0],[268,22,268,30,0,0,4,1],[268,36,268,44,0,0,4,0],[268,36,268,44,0,0,4,1],[268,48,268,56,0,0,4,0]],"lou_getTypeformForEmphClass":[[5262,6,5262,12,0,0,4,0]],"copyStringArray":[[4847,6,4847,12,0,0,4,0]],"list_sort":[[138,17,138,18,0,0,4,0],[138,17,138,18,0,0,4,1]],"list_toArray":[[167,17,167,18,0,0,4,0],[167,17,167,18,0,0,4,1],[168,16,168,24,0,0,4,1]],"feat_free":[[219,6,219,7,0,0,4,0],[221,7,221,14,0,0,4,0]],"_lou_translate":[[1198,6,1198,19,0,0,4,0]],"list_free":[[107,6,107,10,0,0,4,0],[108,7,108,17,0,0,4,0],[108,7,108,17,0,0,4,1]],"lou_hyphenate":[[4079,6,4079,19,0,0,4,0]],"list_dup":[[119,6,119,11,0,0,4,0],[119,6,119,11,0,0,4,1],[121,12,121,21,0,0,4,0]],"cmpFeatures":[[350,6,350,12,0,0,4,0],[350,6,350,12,0,0,4,1],[351,6,351,39,0,0,4,0],[351,6,351,39,0,0,4,1],[354,10,354,12,0,0,4,0],[354,16,354,18,0,0,4,0],[354,16,354,18,0,0,4,1],[356,8,356,14,0,0,4,0],[356,8,356,14,0,0,4,1],[360,10,360,12,0,0,4,0]],"_lou_getTablePath":[[4725,6,4725,18,0,0,4,1]],"listFiles":[[1032,15,1032,42,0,0,4,0],[1032,15,1032,42,0,0,4,1],[1032,46,1032,72,0,0,4,0],[1039,7,1039,30,0,0,4,0]],"analyzeTable":[[702,7,702,23,0,0,4,1],[710,15,710,26,0,0,4,0],[710,15,710,26,0,0,4,1],[713,7,713,12,0,0,4,1],[724,6,724,44,0,0,4,0],[728,10,728,30,0,0,4,0],[729,8,729,25,0,0,4,0],[729,8,729,25,0,0,4,1],[731,13,731,32,0,0,4,0],[731,13,731,32,0,0,4,1],[732,9,732,26,0,0,4,0],[732,9,732,26,0,0,4,1],[733,8,733,27,0,0,4,0],[733,8,733,27,0,0,4,1],[734,10,734,21,0,0,4,1],[742,10,742,37,0,0,4,0],[743,8,743,50,0,0,4,0],[747,14,747,41,0,0,4,0],[748,9,748,51,0,0,4,0],[748,9,748,51,0,0,4,1],[754,11,754,38,0,0,4,0],[755,9,755,39,0,0,4,0],[757,15,757,42,0,0,4,0],[758,10,758,48,0,0,4,0],[758,10,758,48,0,0,4,1],[760,12,760,39,0,0,4,0],[761,11,761,18,0,0,4,1],[762,12,762,54,0,0,4,0],[762,12,762,54,0,0,4,1],[763,13,763,22,0,0,4,0],[764,14,764,44,0,0,4,0],[768,16,768,43,0,0,4,0],[768,16,768,43,0,0,4,1],[769,12,769,19,0,0,4,1],[770,13,771,45,0,0,4,0],[780,11,780,39,0,0,4,0],[781,18,781,21,0,0,4,0],[782,12,782,14,0,0,4,1],[786,12,786,19,0,0,4,1],[806,12,806,21,0,0,4,0],[806,12,806,21,0,0,4,1],[808,13,808,17,0,0,4,1],[815,13,815,41,0,0,4,0],[815,13,815,41,0,0,4,1],[845,14,845,23,0,0,4,0],[846,14,846,21,0,0,4,0],[861,14,861,44,0,0,4,0],[861,14,861,44,0,0,4,1],[862,15,862,24,0,0,4,0],[862,15,862,24,0,0,4,1],[863,21,863,49,0,0,4,0],[864,15,864,22,0,0,4,0],[864,15,864,22,0,0,4,1],[881,13,881,48,0,0,4,0],[881,13,881,48,0,0,4,1],[897,7,897,14,0,0,4,0],[897,7,897,14,0,0,4,1],[897,18,897,26,0,0,4,0],[897,18,897,26,0,0,4,1],[912,7,912,15,0,0,4,0],[912,7,912,15,0,0,4,1],[912,19,912,32,0,0,4,0],[912,19,912,32,0,0,4,1]],"lou_getTable":[[5138,6,5138,12,0,0,4,0]],"meta_free":[[247,6,247,7,0,0,4,0]],"lou_registerLogCallback":[[87,6,87,22,0,0,4,1]],"lou_free":[[5381,6,5381,35,0,0,4,1],[5392,6,5392,31,0,0,4,1],[5421,6,5421,25,0,0,4,1],[5424,6,5424,25,0,0,4,1],[5427,6,5427,25,0,0,4,1]],"lou_indexTables":[[951,23,951,29,0,0,4,0],[951,23,951,29,0,0,4,1],[954,7,954,15,0,0,4,0],[954,7,954,15,0,0,4,1],[960,6,960,17,0,0,4,1]],"lou_freeTableFiles":[[4950,6,4950,13,0,0,4,0]],"lou_listTables":[[1181,6,1181,17,0,0,4,0],[1182,23,1182,24,0,0,4,0],[1182,23,1182,24,0,0,4,1]],"lou_logEnd":[[230,6,230,21,0,0,4,1]],"widestrToStr":[[684,14,684,19,0,0,4,0],[684,14,684,19,0,0,4,1]],"_lou_getTable":[[5126,6,5126,14,0,0,4,1]],"isValidChar":[[523,9,523,22,0,0,4,0],[523,9,523,22,0,0,4,1],[523,26,523,34,0,0,4,0],[523,26,523,34,0,0,4,1],[523,38,523,46,0,0,4,0],[523,38,523,46,0,0,4,1],[523,50,523,58,0,0,4,1]],"isSpace":[[531,9,531,17,0,0,4,0],[531,9,531,17,0,0,4,1],[531,21,531,30,0,0,4,1]],"isLanguageTag":[[262,9,262,47,0,0,4,0],[262,9,262,47,0,0,4,1],[263,4,263,40,0,0,4,0],[263,4,263,40,0,0,4,1],[263,44,263,80,0,0,4,0],[263,44,263,80,0,0,4,1]],"list_conj":[[60,6,60,11,0,0,4,0],[60,6,60,11,0,0,4,1],[67,13,67,17,0,0,4,0],[67,13,67,17,0,0,4,1],[77,10,77,12,0,0,4,0],[77,10,77,12,0,0,4,1],[79,8,79,13,0,0,4,0],[79,8,79,13,0,0,4,1],[81,13,81,18,0,0,4,0],[81,13,81,18,0,0,4,1],[85,9,85,22,0,0,4,0],[85,26,85,30,0,0,4,0],[85,34,85,38,0,0,4,0],[94,7,94,10,0,0,4,0],[94,7,94,10,0,0,4,1]],"_lou_defaultTableResolver":[[4809,7,4809,17,0,0,4,0],[4817,38,4817,48,0,0,4,1],[4820,7,4820,71,0,0,4,0],[4824,8,4824,20,0,0,4,1]],"resolveSubtable":[[4655,6,4655,33,0,0,4,0],[4655,37,4655,62,0,0,4,0],[4683,8,4683,12,0,0,4,0]],"isAlphaNum":[[273,10,273,18,0,0,4,0],[273,10,273,18,0,0,4,1],[273,22,273,30,0,0,4,0],[273,22,273,30,0,0,4,1],[273,35,273,45,0,0,4,0],[273,35,273,45,0,0,4,1]],"indexTablePath":[[1055,6,1055,24,0,0,4,1]],"lou_getEmphClasses":[[5090,6,5090,12,0,0,4,0]],"parseLanguageTag":[[286,6,286,11,0,0,4,1],[287,6,287,19,0,0,4,0],[287,6,287,19,0,0,4,1],[288,7,288,13,0,0,4,0],[288,17,288,30,0,0,4,1],[293,7,293,14,0,0,4,1],[298,10,298,18,0,0,4,0],[299,8,299,17,0,0,4,0],[299,8,299,17,0,0,4,1],[299,21,299,42,0,0,4,0],[299,21,299,42,0,0,4,1],[299,47,299,52,0,0,4,0],[299,47,299,52,0,0,4,1],[299,56,299,74,0,0,4,1],[301,7,301,14,0,0,4,1],[301,18,301,25,0,0,4,1],[301,30,301,38,0,0,4,0],[301,30,301,38,0,0,4,1],[301,42,301,57,0,0,4,1],[309,7,309,16,0,0,4,0],[309,7,309,16,0,0,4,1]],"listDir":[[1005,6,1005,30,0,0,4,0],[1006,10,1006,31,0,0,4,0],[1006,10,1006,31,0,0,4,1],[1008,8,1008,34,0,0,4,0],[1008,38,1008,63,0,0,4,0],[1008,38,1008,63,0,0,4,1]],"compileTable":[[5002,6,5002,18,0,0,4,1],[5005,6,5005,18,0,0,4,1],[5007,6,5007,23,0,0,4,1],[5013,6,5013,18,0,0,4,1],[5029,6,5029,18,0,0,4,1],[5033,7,5033,57,0,0,4,0],[5042,7,5042,19,0,0,4,1],[5052,7,5052,23,0,0,4,0],[5053,8,5053,58,0,0,4,0],[5067,6,5067,17,0,0,4,1],[5072,7,5072,23,0,0,4,0],[5073,8,5073,25,0,0,4,0],[5076,7,5076,19,0,0,4,0],[5076,7,5076,19,0,0,4,1],[5077,8,5077,21,0,0,4,0]],"freeTranslationTable":[[4935,6,4935,25,0,0,4,1]],"list_size":[[153,17,153,18,0,0,4,0],[153,17,153,18,0,0,4,1]],"serializeLanguageTag":[[324,16,324,17,0,0,4,0],[324,16,324,17,0,0,4,1],[327,16,327,17,0,0,4,0],[327,16,327,17,0,0,4,1],[328,7,328,15,0,0,4,0],[328,7,328,15,0,0,4,1]],"getTable":[[5163,38,5163,64,0,0,4,0],[5165,6,5165,30,0,0,4,0],[5165,34,5165,56,0,0,4,0],[5190,6,5190,18,0,0,4,1],[5213,5,5213,17,0,0,4,1],[5216,7,5219,72,0,0,4,1],[5219,7,5219,19,0,0,4,1]],"compileCharDef":[[2675,6,2675,18,0,0,4,1]],"_lou_getTranslationTable":[[5146,6,5146,11,0,0,4,1]]},"library_calls":["lou_readCharFromFile","lou_registerLogCallback","lou_listTables","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_hyphenate","lou_translate","lou_logEnd","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"critical_calls":["lou_readCharFromFile","lou_registerLogCallback","lou_listTables","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_hyphenate","lou_translate","lou_logEnd","lou_freeEmphClasses","lou_freeTableFiles","lou_free"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs to process braille translation and hyphenation:

*/


// Include liblouis headers (adjust based on actual paths)
extern "C" {
}

// Custom log callback
void  id_000000_log_callback(logLevels level, const char *msg) {
    // Empty callback for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
        return 0; // Too small for meaningful input
    }

    // Initialize variables
    const char *tableList = nullptr;
    const char *emphClass = nullptr;
    widechar *inbuf = nullptr;
    char *hyphens = nullptr;
    FILE *temp_file = nullptr;
    char **tables = nullptr;
    const char **emphClasses = nullptr;
    
    // Step 1: Create a temporary file with input data
    temp_file = fmemopen((void *)data, size, "rb");
    if (!temp_file) {
        assert_file_closed(&temp_file);
	return 0;
    }
    
    // Step 2: Read first character from file
    int mode = 0;
    lou_readCharFromFile("input_file", &mode);
    
    // Step 3: Create test strings from input data
    // Use first 256 bytes for table list (null-terminated)
    size_t table_len = std::min(size / 4, (size_t)255);
    char *table_buf = new char[table_len + 1];
    memcpy(table_buf, data, table_len);
    table_buf[table_len] = '\0';
    tableList = table_buf;
    
    // Use next portion for emphasis class
    size_t emph_offset = table_len + 1;
    if (emph_offset < size - 1) {
        size_t emph_len = std::min((size - emph_offset) / 4, (size_t)100);
        char *emph_buf = new char[emph_len + 1];
        memcpy(emph_buf, data + emph_offset, emph_len);
        emph_buf[emph_len] = '\0';
        emphClass = emph_buf;
    } else {
        emphClass = "emphasis";
    }
    
    // Step 4: Register log callback
    lou_registerLogCallback( id_000000_log_callback);
    
    // Step 5: Get list of tables
    tables = lou_listTables();
    
    // Step 6: Get emphasis classes for table list
    emphClasses = lou_getEmphClasses(tableList);
    
    // Step 7: Get typeform for emphasis class
    formtype typeform_val = lou_getTypeformForEmphClass(tableList, emphClass);
    
    // Step 8: Prepare input buffer for translation/hyphenation
    // Use remaining data for wide character buffer
    size_t wide_offset = emph_offset + (emphClass == std::string("emphasis") ? 0 : 100);
    if (wide_offset < size) {
        size_t wide_len = (size - wide_offset) / sizeof(widechar);
        if (wide_len > 0) {
            inbuf = new widechar[wide_len + 1];
            memcpy(inbuf, data + wide_offset, wide_len * sizeof(widechar));
            inbuf[wide_len] = 0; // Null terminate
            
            // Prepare hyphenation array
            hyphens = new char[wide_len + 1];
            
            // Step 9: Call hyphenate function
            int hyphen_result = lou_hyphenate(tableList, inbuf, wide_len, hyphens, 0);
            
            // Step 10: Prepare for translation
            int inlen = wide_len;
            int outlen = wide_len * 2; // Conservative estimate
            widechar *outbuf = new widechar[outlen + 1];
            formtype *typeform = new formtype[wide_len];
            char *spacing = new char[wide_len];
            int *outputPos = new int[outlen];
            int *inputPos = new int[wide_len];
            int *cursorPos = new int[1];
            
            // Initialize arrays
            for (size_t i = 0; i < wide_len; i++) {
                typeform[i] = typeform_val;
                spacing[i] = 0;
                inputPos[i] = i;
            }
            cursorPos[0] = 0;
            
            // Step 11: Call translate function
            int translate_result = lou_translate(
                tableList,
                inbuf,
                &inlen,
                outbuf,
                &outlen,
                typeform,
                spacing,
                outputPos,
                inputPos,
                cursorPos,
                0
            );
            
            // Clean up translation resources
            delete[] outbuf;
            delete[] typeform;
            delete[] spacing;
            delete[] outputPos;
            delete[] inputPos;
            delete[] cursorPos;
            
            delete[] hyphens;
        }
    }
    
    // Step 12: End logging
    lou_logEnd();
    
    // Clean up
    if (temp_file) {
        assert_file_closed(&temp_file);;
    }
    
    // Free allocated strings
    delete[] table_buf;
    if (emphClass != std::string("emphasis")) {
        delete[] const_cast<char*>(emphClass);
    }
    
    // Free liblouis allocated resources
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }
    
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    if (inbuf) {
        delete[] inbuf;
    }
    
    // Free internal liblouis resources
    lou_free();
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
}