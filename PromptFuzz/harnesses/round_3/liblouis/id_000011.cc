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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 198
//<Quality> {"density":12,"unique_branches":{"pattern_compile_2":[[1048,7,1048,38,0,0,4,0],[1048,42,1048,71,0,0,4,0],[1049,8,1049,79,0,0,4,1],[1053,7,1053,42,0,0,4,0],[1055,8,1055,33,0,0,4,1],[1060,8,1060,40,0,0,4,1]],"getTable":[[5174,6,5175,38,0,0,4,1],[5197,6,5198,34,0,0,4,1]],"lou_checkTable":[[5255,6,5255,29,0,0,4,1]],"pattern_compile_1":[[1013,9,1013,31,0,0,4,0],[1015,7,1016,41,0,0,4,1],[1020,7,1020,32,0,0,4,1]],"compilePassOpcode":[[1905,8,1905,27,0,0,4,1],[2066,8,2066,27,0,0,4,1],[2071,8,2071,79,0,0,4,1],[2073,8,2073,50,0,0,4,1],[2081,8,2081,27,0,0,4,1],[2085,8,2085,79,0,0,4,1]],"isMatchPatternEscape":[[1259,9,1259,23,0,0,4,0],[1259,28,1259,37,0,0,4,1],[1259,41,1259,50,0,0,4,1],[1259,54,1259,63,0,0,4,0]],"addRule":[[1038,8,1038,14,0,0,4,1],[1040,8,1040,15,0,0,4,0],[1041,9,1041,48,0,0,4,1]],"getMatchPatternCharacters":[[1544,6,1544,43,0,0,4,1]],"pattern_compile_expression":[[607,6,607,29,0,0,4,1],[609,2,609,10,0,0,4,0],[609,2,609,10,0,0,4,1],[611,7,611,33,0,0,4,1],[614,7,614,30,0,0,4,1],[618,32,618,53,0,0,4,0],[619,8,619,32,0,0,4,1],[624,8,624,31,0,0,4,1],[626,13,626,36,0,0,4,0],[626,13,626,36,0,0,4,1],[626,40,626,44,0,0,4,0],[627,9,627,13,0,0,4,1],[635,7,635,29,0,0,4,1],[647,7,648,31,0,0,4,1],[658,2,658,10,0,0,4,0],[658,2,658,10,0,0,4,1],[660,7,660,33,0,0,4,1],[681,7,682,41,0,0,4,1],[685,7,685,32,0,0,4,1],[697,2,697,10,0,0,4,1],[705,2,705,10,0,0,4,0],[705,2,705,10,0,0,4,1],[707,7,707,32,0,0,4,1],[713,2,713,10,0,0,4,1],[720,2,720,10,0,0,4,0],[720,2,720,10,0,0,4,1],[722,7,722,32,0,0,4,1],[727,2,727,10,0,0,4,1],[734,2,734,10,0,0,4,0],[734,2,734,10,0,0,4,1],[736,7,736,32,0,0,4,1],[739,7,739,30,0,0,4,1],[742,7,742,31,0,0,4,0],[742,7,742,31,0,0,4,1],[745,33,745,54,0,0,4,0],[746,9,746,32,0,0,4,0],[746,9,746,32,0,0,4,1],[747,8,747,30,0,0,4,1],[756,10,756,34,0,0,4,0],[756,10,756,34,0,0,4,1],[758,4,758,12,0,0,4,0],[758,4,758,12,0,0,4,1],[761,4,761,12,0,0,4,0],[761,4,761,12,0,0,4,1],[764,4,764,12,0,0,4,0],[764,4,764,12,0,0,4,1],[767,4,767,12,0,0,4,1],[770,4,770,12,0,0,4,0],[770,4,770,12,0,0,4,1],[773,4,773,12,0,0,4,0],[773,4,773,12,0,0,4,1],[776,4,776,12,0,0,4,0],[776,4,776,12,0,0,4,1],[779,4,779,12,0,0,4,1],[782,4,782,12,0,0,4,0],[782,4,782,12,0,0,4,1],[785,4,785,12,0,0,4,0],[785,4,785,12,0,0,4,1],[788,4,788,12,0,0,4,0],[788,4,788,12,0,0,4,1],[792,4,792,12,0,0,4,1],[793,4,793,12,0,0,4,0],[793,4,793,12,0,0,4,1],[794,4,794,12,0,0,4,0],[794,4,794,12,0,0,4,1],[795,4,795,12,0,0,4,0],[795,4,795,12,0,0,4,1],[796,4,796,12,0,0,4,1],[797,4,797,12,0,0,4,1],[798,4,798,12,0,0,4,1],[799,4,799,12,0,0,4,1],[802,9,802,11,0,0,4,1],[817,4,817,12,0,0,4,0],[817,4,817,12,0,0,4,1],[821,4,821,11,0,0,4,1],[828,7,828,10,0,0,4,0],[828,7,828,10,0,0,4,1],[831,2,831,10,0,0,4,0],[831,2,831,10,0,0,4,1],[834,7,834,30,0,0,4,1],[838,32,838,53,0,0,4,0],[839,8,839,32,0,0,4,0],[839,8,839,32,0,0,4,1],[839,36,839,40,0,0,4,0],[844,8,844,31,0,0,4,0],[844,8,844,31,0,0,4,1],[844,35,844,39,0,0,4,0],[844,35,844,39,0,0,4,1],[847,7,847,29,0,0,4,1],[849,7,849,59,0,0,4,1],[855,15,855,37,0,0,4,0],[855,15,855,37,0,0,4,1],[856,8,856,33,0,0,4,0],[856,8,856,33,0,0,4,1],[856,37,856,41,0,0,4,0],[868,2,868,10,0,0,4,1],[904,2,904,10,0,0,4,1],[905,2,905,10,0,0,4,1],[912,2,912,11,0,0,4,1],[917,2,917,9,0,0,4,0],[917,2,917,9,0,0,4,1],[919,7,919,32,0,0,4,1]],"passGetAttributes":[[1705,3,1705,21,0,0,4,0],[1723,3,1723,22,0,0,4,0]],"pattern_reverse_expression":[[1244,6,1244,36,0,0,4,1],[1247,9,1247,39,0,0,4,0],[1247,9,1247,39,0,0,4,1],[1260,9,1260,31,0,0,4,0],[1260,9,1260,31,0,0,4,1]],"addBackwardPassRule":[[972,2,972,18,0,0,4,0],[972,2,972,18,0,0,4,1],[975,2,975,18,0,0,4,1],[978,2,978,16,0,0,4,0],[978,2,978,16,0,0,4,1],[981,2,981,16,0,0,4,1],[984,2,984,16,0,0,4,1],[987,2,987,9,0,0,4,1],[990,9,990,22,0,0,4,0],[990,9,990,22,0,0,4,1],[992,7,992,35,0,0,4,0],[992,7,992,35,0,0,4,1]],"compileRule":[[3103,8,3103,34,0,0,4,0],[3103,38,3103,60,0,0,4,0],[3112,8,3112,33,0,0,4,0],[3112,37,3112,58,0,0,4,0],[3782,8,3783,41,0,0,4,1],[3801,3,3801,26,0,0,4,0],[3802,8,3802,43,0,0,4,1],[3803,20,3803,40,0,0,4,0],[3803,20,3803,40,0,0,4,1],[3805,9,3805,11,0,0,4,1],[4124,13,4124,27,0,0,4,1],[4126,5,4126,13,0,0,4,0],[4127,5,4127,13,0,0,4,0],[4128,5,4128,13,0,0,4,0],[4138,9,4138,24,0,0,4,0],[4139,10,4139,29,0,0,4,1],[4145,10,4145,26,0,0,4,1],[4145,30,4145,44,0,0,4,1],[4151,10,4151,51,0,0,4,0],[4181,9,4181,28,0,0,4,0]],"passGetDots":[[1765,8,1765,51,0,0,4,0]],"parseCharsInternal":[[1272,6,1273,41,0,0,4,1]],"pattern_compile_3":[[1101,7,1101,38,0,0,4,0],[1101,42,1101,71,0,0,4,0],[1103,5,1103,40,0,0,4,0],[1105,8,1105,79,0,0,4,1],[1109,7,1109,42,0,0,4,0],[1110,8,1110,34,0,0,4,1],[1115,8,1115,40,0,0,4,1],[1117,11,1117,43,0,0,4,0],[1117,11,1117,43,0,0,4,1],[1145,8,1145,38,0,0,4,1],[1145,42,1145,78,0,0,4,1],[1148,11,1148,41,0,0,4,0],[1148,11,1148,41,0,0,4,1],[1148,45,1148,81,0,0,4,0],[1148,45,1148,81,0,0,4,1],[1177,8,1177,79,0,0,4,1]],"pattern_reverse_branch":[[1220,2,1220,20,0,0,4,0],[1220,2,1220,20,0,0,4,1],[1227,2,1227,16,0,0,4,0],[1227,2,1227,16,0,0,4,1],[1228,2,1228,14,0,0,4,0],[1228,2,1228,14,0,0,4,1],[1229,2,1229,19,0,0,4,1],[1230,2,1230,20,0,0,4,0],[1230,2,1230,20,0,0,4,1],[1231,2,1231,19,0,0,4,1],[98,34,98,52,8,0,4,0],[98,34,98,52,8,0,4,1]],"getNextNumberedAttribute":[[1087,6,1087,29,0,0,4,1]]},"library_calls":["lou_listTables","lou_checkTable","lou_freeTableFiles","lou_registerLogCallback","lou_freeTableFiles","lou_translateString","lou_logPrint","lou_freeTableFiles","lou_backTranslate","lou_logPrint","lou_checkTable","lou_logPrint","lou_freeTableFiles"],"critical_calls":["lou_listTables","lou_checkTable","lou_registerLogCallback","lou_translateString","lou_logPrint","lou_backTranslate","lou_logPrint","lou_checkTable","lou_logPrint","lou_freeTableFiles"],"visited":2}
/**/


// Include liblouis headers

// Custom log callback function
static void  id_000011_logCallback(logLevels level, const char* message) {
    // Simple callback that does nothing for fuzzing
    (void)level;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. List available tables
    char** tables = lou_listTables();
    if (!tables) {
        return 0;
    }

    // Find a valid table to use
    const char* tableList = nullptr;
    for (int i = 0; tables[i] != nullptr; ++i) {
        if (lou_checkTable(tables[i]) == 0) {
            tableList = tables[i];
            break;
        }
    }

    if (!tableList) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // 2. Register log callback
    lou_registerLogCallback( id_000011_logCallback);

    // 3. Prepare input data for translation
    // Convert byte data to widechars (liblouis expects UTF-16 like encoding)
    // We'll use half the size for widechars since widechar is 2 bytes
    size_t widecharCount = size / 2;
    if (widecharCount == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }

    std::vector<widechar> inbuf(widecharCount);
    memcpy(inbuf.data(), data, widecharCount * sizeof(widechar));

    int inlen = static_cast<int>(widecharCount);
    std::vector<widechar> outbuf(inlen * 2); // Allocate enough space for output
    int outlen = static_cast<int>(outbuf.size());

    // Allocate auxiliary arrays
    std::vector<formtype> typeform(inlen, 0);
    std::vector<char> spacing(inlen, 0);
    std::vector<int> outputPos(inlen, 0);
    std::vector<int> inputPos(inlen, 0);
    std::vector<int> cursorPos(1, 0);

    // 4. Perform forward translation
    int translateResult = lou_translateString(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        fuzz_int32_t_1  // mode
    );

    // 5. Log the result
    lou_logPrint("Forward translation result: %d", translateResult);

    if (translateResult == 0) {
        lou_freeTableFiles(tables);
        return 0;
    }

    // 6. Prepare for back translation using the output from forward translation
    // Reset lengths for back translation
    int backInlen = outlen;
    int backOutlen = inlen * 2;
    std::vector<widechar> backOutbuf(backOutlen);

    // Reset auxiliary arrays for back translation
    std::fill(typeform.begin(), typeform.end(), 0);
    std::fill(spacing.begin(), spacing.end(), 0);
    std::fill(outputPos.begin(), outputPos.end(), 0);
    std::fill(inputPos.begin(), inputPos.end(), 0);
    std::fill(cursorPos.begin(), cursorPos.end(), 0);

    // 7. Perform back translation
    int backTranslateResult = lou_backTranslate(
        tableList,
        outbuf.data(),
        &backInlen,
        backOutbuf.data(),
        &backOutlen,
        typeform.data(),
        spacing.data(),
        outputPos.data(),
        inputPos.data(),
        cursorPos.data(),
        fuzz_int32_t_2  // mode
    );

    // 8. Log the back translation result
    lou_logPrint("Back translation result: %d", backTranslateResult);

    // 9. Check the table again (as required by instructions)
    int tableCheckResult = lou_checkTable(tableList);
    lou_logPrint("Table check result: %d", tableCheckResult);

    // 10. Clean up
    lou_freeTableFiles(tables);
    
    // Note: lou_free() is not called because it would unload all tables
    // and break subsequent fuzzing iterations
    
    return 0;
}