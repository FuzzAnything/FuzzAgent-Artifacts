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
#include <cre2.h>
}
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 506, nr_unique_branch: 222
//<Quality> {"density":11,"unique_branches":{"_ZN3re28Compiler9PostVisitEPNS_6RegexpENS_4FragES3_PS3_i":[[869,5,869,23,0,0,4,0],[871,23,871,39,0,0,4,0],[871,23,871,39,0,0,4,1],[886,5,886,21,0,0,4,0],[919,5,919,26,0,0,4,0],[921,11,921,22,0,0,4,1],[939,49,939,63,0,0,4,0],[939,49,939,63,0,0,4,1],[941,13,941,22,0,0,4,0],[941,26,941,38,0,0,4,0],[941,26,941,38,0,0,4,1],[941,42,941,54,0,0,4,0],[941,42,941,54,0,0,4,1],[947,14,947,26,0,0,4,0],[947,14,947,26,0,0,4,1],[947,30,947,42,0,0,4,1],[947,47,947,58,0,0,4,0],[947,47,947,58,0,0,4,1],[947,62,947,73,0,0,4,1],[948,14,948,25,0,0,4,0],[948,29,948,40,0,0,4,0],[948,29,948,40,0,0,4,1],[956,5,956,24,0,0,4,0],[959,11,959,24,0,0,4,1]],"_ZN3re214CoalesceWalker9PostVisitEPNS_6RegexpES2_S2_PS2_i":[[238,7,238,22,0,0,4,1],[241,7,241,32,0,0,4,0],[241,7,241,32,0,0,4,1],[242,9,242,42,0,0,4,0],[262,19,262,33,0,0,4,0],[262,19,262,33,0,0,4,1],[263,9,263,25,0,0,4,0],[263,9,263,25,0,0,4,1],[264,9,264,52,0,0,4,1],[269,7,269,20,0,0,4,0],[270,9,270,42,0,0,4,0]],"_ZN3re222MaybeParsePerlCCEscapeEPSt17basic_string_viewIcSt11char_traitsIcEENS_6Regexp10ParseFlagsE":[[1756,7,1756,16,0,0,4,1]],"_ZN3re2L7MaxRuneEi":[[442,7,442,15,0,0,4,0],[442,7,442,15,0,0,4,1]],"_ZN3re24Prog14ComputeByteMapEv":[[469,11,469,25,0,0,4,0],[469,29,469,38,0,0,4,0],[469,42,469,51,0,0,4,0],[472,13,472,25,0,0,4,1],[474,13,474,25,0,0,4,1],[476,13,476,29,0,0,4,0],[484,11,484,22,0,0,4,0],[485,11,485,49,0,0,4,0],[486,11,486,41,0,0,4,0]],"_ZN3re24Prog13MarkDominatorEiPNS_11SparseArrayIiEES3_PSt6vectorIS4_IiSaIiEESaIS6_EEPNS_10SparseSetTIvEEPS6_":[[739,9,739,19,0,0,4,0],[739,23,739,45,0,0,4,0],[739,23,739,45,0,0,4,1],[751,7,751,20,0,0,4,0],[757,7,757,24,0,0,4,0],[775,9,775,31,0,0,4,0],[776,21,776,22,0,0,4,0],[776,21,776,22,0,0,4,1],[777,13,777,39,0,0,4,0],[777,13,777,39,0,0,4,1],[780,15,780,38,0,0,4,1]],"_ZN3re26Regexp10ParseState12DoRightParenEv":[[723,7,723,19,0,0,4,0]],"_ZN3re29PatchList6AppendEPNS_4Prog4InstES0_S0_":[[64,9,64,21,0,0,4,0],[64,9,64,21,0,0,4,1],[66,9,66,21,0,0,4,1],[69,9,69,18,0,0,4,1]],"_ZN3re28Compiler16AddRuneRangeUTF8Eiib":[[696,7,696,14,0,0,4,1],[700,7,700,17,0,0,4,1],[706,19,706,29,0,0,4,0],[706,19,706,29,0,0,4,1],[708,9,708,18,0,0,4,0],[708,22,708,30,0,0,4,1],[716,7,716,20,0,0,4,0]],"_ZN3re28Compiler18AddSuffixRecursiveEii":[[532,7,532,19,0,0,4,0],[534,9,534,16,0,0,4,1]],"_ZN3re24Prog8OptimizeEv":[[249,9,249,33,0,0,4,0],[251,14,251,20,0,0,4,0],[251,24,251,58,0,0,4,1],[272,9,272,33,0,0,4,0],[275,9,275,33,0,0,4,0],[278,11,278,40,0,0,4,0],[278,11,278,40,0,0,4,1],[278,44,278,58,0,0,4,1],[284,11,284,27,0,0,4,1]],"_ZN3re26Regexp22RequiredPrefixForAccelEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPb":[[744,16,744,36,0,0,4,0],[744,40,744,49,0,0,4,0]],"_ZN3re219NamedCapturesWalker8PreVisitEPNS_6RegexpEiPb":[[598,9,598,35,0,0,4,0],[598,39,598,57,0,0,4,1]],"_ZN3re28Compiler9AddSuffixEi":[[504,7,504,14,0,0,4,1],[507,7,507,29,0,0,4,0],[507,7,507,29,0,0,4,1],[512,7,512,33,0,0,4,0]],"_ZN3re2L11IsAnchorEndEPPNS_6RegexpEi":[[1046,5,1046,12,0,0,4,1],[1048,5,1048,23,0,0,4,0],[1049,11,1049,25,0,0,4,0],[1051,13,1051,39,0,0,4,1],[1063,5,1063,24,0,0,4,0],[1065,11,1065,37,0,0,4,1]],"_ZN3re24Prog9IsOnePassEv":[[462,15,462,30,0,0,4,1],[480,33,480,51,0,0,4,0],[486,17,486,51,0,0,4,1],[488,24,488,37,0,0,4,1],[495,15,495,29,0,0,4,0],[498,30,498,37,0,0,4,0],[498,30,498,37,0,0,4,1],[501,22,501,31,0,0,4,0],[501,35,501,53,0,0,4,0],[501,35,501,53,0,0,4,1],[505,19,505,26,0,0,4,1],[507,19,507,53,0,0,4,1],[509,26,509,39,0,0,4,1],[518,15,518,25,0,0,4,1],[521,15,521,34,0,0,4,1],[527,9,527,26,0,0,4,0],[529,9,529,22,0,0,4,0],[530,15,530,26,0,0,4,0],[530,15,530,26,0,0,4,1],[532,17,532,36,0,0,4,1],[538,15,538,43,0,0,4,0],[538,15,538,43,0,0,4,1],[538,47,538,66,0,0,4,0],[540,15,540,46,0,0,4,1],[550,15,550,39,0,0,4,1]],"_ZN3re26Regexp5ParseESt17basic_string_viewIcSt11char_traitsIcEENS0_10ParseFlagsEPNS_12RegexpStatusE":[[2310,13,2310,33,0,0,4,0],[2310,38,2310,51,0,0,4,0],[2310,55,2310,66,0,0,4,1],[2316,13,2316,38,0,0,4,1],[2320,15,2320,51,0,0,4,1],[2379,13,2379,31,0,0,4,0],[2380,15,2380,25,0,0,4,0],[2380,29,2380,40,0,0,4,1],[2384,15,2384,33,0,0,4,1],[2506,13,2506,22,0,0,4,0],[2510,15,2510,33,0,0,4,1]],"_ZNK3re23RE211ProgramSizeEv":[[324,7,324,20,0,0,4,1]],"_ZN3re24Prog14MarkSuccessorsEPNS_11SparseArrayIiEES3_PSt6vectorIS4_IiSaIiEESaIS6_EEPNS_10SparseSetTIvEEPS6_":[[705,7,705,24,0,0,4,0]],"_ZN3re29PatchList5PatchEPNS_4Prog4InstES0_j":[[52,11,52,19,0,0,4,0]],"_ZN3re2L7IsMatchEPNS_4ProgEPNS0_4InstE":[[207,7,207,14,0,0,4,1],[211,7,211,20,0,0,4,0],[211,7,211,20,0,0,4,1],[212,7,212,25,0,0,4,1],[213,7,213,26,0,0,4,0],[213,7,213,26,0,0,4,1],[214,7,214,21,0,0,4,1],[215,7,215,27,0,0,4,1],[218,7,218,24,0,0,4,1],[219,7,219,20,0,0,4,1],[223,7,223,22,0,0,4,1]],"_ZN3re214CoalesceWalker11CanCoalesceEPNS_6RegexpES2_":[[311,8,311,31,0,0,4,1],[312,8,312,31,0,0,4,1],[313,8,313,32,0,0,4,1],[314,8,314,33,0,0,4,1]],"_ZN3re26RegexpD2Ev":[[55,5,55,24,0,0,4,0]],"_ZN3re28Compiler22UncachedRuneByteSuffixEhhbi":[[466,7,466,16,0,0,4,1]],"_ZN3re24Prog12ComputeHintsEPSt6vectorINS0_4InstESaIS2_EEii":[[914,9,914,23,0,0,4,0],[914,27,914,36,0,0,4,0],[914,40,914,49,0,0,4,0],[917,11,917,23,0,0,4,1],[919,11,919,23,0,0,4,1],[921,11,921,27,0,0,4,0]],"_ZN3re26Regexp14RequiredPrefixEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPbPPS0_":[[706,7,706,27,0,0,4,1],[709,10,709,19,0,0,4,0],[709,23,709,56,0,0,4,1],[711,7,711,13,0,0,4,0]],"cre2_strings_to_ranges":[[323,17,323,25,0,0,4,0],[323,17,323,25,0,0,4,1]],"_ZN3re24Prog8EmitListEiPNS_11SparseArrayIiEEPSt6vectorINS0_4InstESaIS5_EEPNS_10SparseSetTIvEEPS4_IiSaIiEE":[[830,7,830,24,0,0,4,0]],"_ZN3re28Compiler7CaptureENS_4FragEi":[[426,7,426,19,0,0,4,1],[429,7,429,13,0,0,4,1]],"_ZN3re210SparseSetTIvE14InsertInternalEbi":[[158,11,158,23,0,0,4,1]],"_ZN3re28Compiler14ByteRangeEqualEii":[[586,10,586,44,0,0,4,1]],"_ZN3re2L13IsAnchorStartEPPNS_6RegexpEi":[[999,5,999,12,0,0,4,1],[1001,5,1001,23,0,0,4,0],[1002,11,1002,25,0,0,4,0],[1004,13,1004,41,0,0,4,1]],"_ZN3re26Regexp13ComputeSimpleEv":[[77,5,77,24,0,0,4,0]],"_ZN3re214ByteMapBuilder5MergeEv":[[408,11,408,21,0,0,4,1]],"_ZN3re28Compiler12AddRuneRangeEiib":[[635,5,635,12,0,0,4,1],[636,5,636,23,0,0,4,0],[639,5,639,25,0,0,4,1]],"_ZNK3re23RE27Options10ParseFlagsEv":[[189,7,189,15,0,0,4,0]],"cre2_opt_encoding":[[118,3,118,34,0,0,4,0],[120,3,120,36,0,0,4,1],[122,3,122,10,0,0,4,1]],"_ZN3re26Regexp6WalkerIPS0_E12WalkInternalES2_S2_b":[[177,7,177,14,0,0,4,1],[191,13,191,27,0,0,4,0],[193,18,193,31,0,0,4,0],[197,7,197,14,0,0,4,0],[198,13,198,26,0,0,4,0],[200,15,200,31,0,0,4,0],[200,15,200,31,0,0,4,1],[201,17,201,25,0,0,4,0],[201,29,201,37,0,0,4,0],[201,29,201,37,0,0,4,1],[201,41,201,67,0,0,4,1],[212,13,212,26,0,0,4,0],[221,9,221,23,0,0,4,1],[224,9,224,30,0,0,4,0]],"_ZN3re26Regexp10ParseState11DoLeftParenESt17basic_string_viewIcSt11char_traitsIcEE":[[635,7,635,26,0,0,4,1]],"_ZN3re26Regexp6WalkerINS_4FragEE12WalkInternalEPS0_S2_b":[[177,7,177,14,0,0,4,1],[191,13,191,27,0,0,4,0],[193,18,193,31,0,0,4,0],[197,7,197,14,0,0,4,0],[198,13,198,26,0,0,4,0],[200,15,200,31,0,0,4,0],[200,15,200,31,0,0,4,1],[201,17,201,25,0,0,4,1],[212,13,212,26,0,0,4,0],[221,9,221,23,0,0,4,1],[224,9,224,30,0,0,4,0]],"_ZN3re28Compiler13FindByteRangeEii":[[592,7,592,45,0,0,4,0],[592,7,592,45,0,0,4,1],[593,9,593,33,0,0,4,1],[599,10,599,42,0,0,4,0],[601,9,601,33,0,0,4,1],[607,9,607,19,0,0,4,0]],"cre2_find_named_capturing_groups":[[198,6,198,19,0,0,4,1]],"_ZN3re2L16ChildArgsChangedEPNS_6RegexpEPS1_":[[208,19,208,33,0,0,4,0],[208,19,208,33,0,0,4,1],[211,9,211,22,0,0,4,1],[214,19,214,33,0,0,4,0],[214,19,214,33,0,0,4,1]],"_ZN3re28Compiler4PlusENS_4FragEb":[[333,7,333,13,0,0,4,1],[336,7,336,16,0,0,4,1]],"_ZN3re217NumCapturesWalker8PreVisitEPNS_6RegexpEiPb":[[559,9,559,35,0,0,4,0]]},"library_calls":["cre2_version_interface_revision","cre2_opt_new","cre2_opt_set_dot_nl","cre2_opt_case_sensitive","cre2_opt_encoding","cre2_opt_one_line","cre2_new","cre2_program_size","cre2_find_named_capturing_groups","cre2_error_arg","cre2_strings_to_ranges","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_version_interface_revision","cre2_opt_new","cre2_opt_set_dot_nl","cre2_opt_case_sensitive","cre2_opt_encoding","cre2_opt_one_line","cre2_new","cre2_program_size","cre2_find_named_capturing_groups","cre2_error_arg","cre2_strings_to_ranges","cre2_delete","cre2_opt_delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data from the file to construct our test scenario
    // First, read the version interface revision
    int version_rev = cre2_version_interface_revision();
    
    // Create options for regex
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set various options using data from input
    // Use first byte to determine dot_nl flag
    int dot_nl_flag = 0;
    if (size > 0) {
        fread(&dot_nl_flag, 1, 1, in_file);
        dot_nl_flag = dot_nl_flag % 2; // Ensure 0 or 1
        cre2_opt_set_dot_nl(opt, fuzz_int32_t_1);
    }

    // Check case sensitivity option
    int case_sensitive = cre2_opt_case_sensitive(opt);
    
    // Check encoding option
    cre2_encoding_t encoding = cre2_opt_encoding(opt);
    
    // Check one_line option
    int one_line = cre2_opt_one_line(opt);

    // Create a regex pattern from input data
    const char *pattern = "test(\\w+)";
    cre2_regexp_t *re = cre2_new(pattern, strlen(pattern), opt);
    
    if (re) {
        // Get program size
        int prog_size = cre2_program_size(re);
        
        // Try to find named capturing groups
        // Use input data for group name if available
        char group_name[256] = {0};
        if (size > 1) {
            size_t name_len = size - 1 > 255 ? 255 : size - 1;
            fseek(in_file, 1, SEEK_SET);
            fread(group_name, 1, name_len, in_file);
            group_name[name_len] = '\0';
        } else {
            strcpy(group_name, "test");
        }
        
        int group_index = cre2_find_named_capturing_groups(re, group_name);
        
        // Get error argument if any
        cre2_string_t error_arg;
        error_arg.data = NULL;
        error_arg.length = 0;
        cre2_error_arg(re, &error_arg);
        
        // Test strings_to_ranges conversion
        // Prepare test data
        const char *test_text = "Hello World";
        cre2_range_t ranges[2];
        cre2_string_t strings[2];
        
        // Initialize strings
        for (int i = 0; i < 2; i++) {
            strings[i].data = test_text;
            strings[i].length = strlen(test_text);
        }
        
        // Convert strings to ranges
        cre2_strings_to_ranges(fuzz_str_2, ranges, strings, 2);
        
        // Clean up regex
        cre2_delete(re);
    }
    
    // Clean up options
    cre2_opt_delete(opt);
    
    // Close the input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}