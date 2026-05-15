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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 840, nr_unique_branch: 456
//<Quality> {"density":10,"unique_branches":{"_ZN3re26Regexp10ParseState13DoVerticalBarEv":[[659,7,659,31,0,0,4,0],[660,7,660,31,0,0,4,1]],"cre2_named_groups_iter_next":[[234,7,234,57,0,0,4,0]],"_ZNK3re210SparseSetTIvE8max_sizeEv":[[112,9,112,30,0,0,4,0]],"_ZNK3re212RegexpStatus4TextB5cxx11Ev":[[536,7,536,25,0,0,4,0]],"_ZN3re23RE2D2Ev":[[306,7,306,42,0,0,4,0],[308,7,308,44,0,0,4,0],[308,7,308,44,0,0,4,1],[312,7,312,35,0,0,4,0],[312,7,312,35,0,0,4,1],[314,7,314,31,0,0,4,0],[314,7,314,31,0,0,4,1],[316,7,316,21,0,0,4,0],[316,7,316,21,0,0,4,1],[318,7,318,21,0,0,4,0],[318,7,318,21,0,0,4,1]],"_ZN3re28Compiler3CatENS_4FragES1_":[[283,7,283,19,0,0,4,1],[283,23,283,35,0,0,4,1],[288,7,288,34,0,0,4,0],[288,7,288,34,0,0,4,1],[289,7,289,35,0,0,4,0],[290,7,290,24,0,0,4,0],[297,7,297,16,0,0,4,1],[303,31,303,41,0,0,4,0],[303,45,303,55,0,0,4,1]],"cre2_partial_match":[[339,9,339,20,0,0,4,1],[346,19,346,27,0,0,4,0],[346,19,346,27,0,0,4,1],[351,9,351,15,0,0,4,1]],"_ZN3re26Regexp6IncrefEv":[[104,7,104,24,0,0,4,1]],"_ZN3re26Regexp10ParseState8DoFinishEv":[[740,7,740,17,0,0,4,0],[740,21,740,38,0,0,4,1]],"_ZN3re2L13IsAnchorStartEPPNS_6RegexpEi":[[996,7,996,17,0,0,4,1],[996,21,996,31,0,0,4,1],[999,5,999,12,0,0,4,0],[1001,5,1001,23,0,0,4,1],[1016,5,1016,24,0,0,4,1],[1025,5,1025,26,0,0,4,1]],"_ZN3re214ByteMapBuilder7RecolorEi":[[444,7,444,28,0,0,4,1]],"_ZN3re2L11IsAnchorEndEPPNS_6RegexpEi":[[1043,7,1043,17,0,0,4,1],[1043,21,1043,31,0,0,4,1],[1046,5,1046,12,0,0,4,0],[1048,5,1048,23,0,0,4,1],[1063,5,1063,24,0,0,4,1],[1072,5,1072,24,0,0,4,1]],"_ZN3re28Compiler9ByteRangeEiib":[[391,7,391,13,0,0,4,1]],"_ZN3re26Regexp8SimplifyEv":[[183,7,183,18,0,0,4,1],[185,7,185,25,0,0,4,1],[192,7,192,18,0,0,4,1],[194,7,194,25,0,0,4,1]],"_ZN3re26Regexp15AddRuneToStringEi":[[185,7,185,19,0,0,4,0],[185,7,185,19,0,0,4,1],[188,14,188,26,0,0,4,1]],"_ZN3re28Compiler4StarENS_4FragEb":[[352,7,352,17,0,0,4,1],[356,7,356,13,0,0,4,1],[359,7,359,16,0,0,4,0]],"_ZNK3re210SparseSetTIvE8containsEi":[[225,7,225,68,0,0,4,1],[229,10,229,48,0,0,4,0],[229,10,229,48,0,0,4,1],[230,10,230,33,0,0,4,0],[230,10,230,33,0,0,4,1]],"_ZN3re24Prog12ComputeHintsEPSt6vectorINS0_4InstESaIS2_EEii":[[863,22,863,33,0,0,4,0],[863,22,863,33,0,0,4,1],[864,9,864,18,0,0,4,0],[864,9,864,18,0,0,4,1],[865,9,865,47,0,0,4,0],[865,9,865,47,0,0,4,1],[866,11,866,16,0,0,4,0],[866,11,866,16,0,0,4,1],[914,9,914,23,0,0,4,1],[928,9,928,21,0,0,4,1]],"_ZN3re2L5truncB5cxx11ESt17basic_string_viewIcSt11char_traitsIcEE":[[139,7,139,27,0,0,4,1]],"_ZN3re24Prog14ComputeByteMapEv":[[463,20,463,31,0,0,4,0],[463,20,463,31,0,0,4,1],[465,9,465,39,0,0,4,0],[465,9,465,39,0,0,4,1],[469,11,469,25,0,0,4,1],[484,11,484,22,0,0,4,1],[489,16,489,47,0,0,4,1]],"_ZN3re26Regexp14RequiredPrefixEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPbPPS0_":[[706,7,706,27,0,0,4,0]],"_ZN3re23RE24InitESt17basic_string_viewIcSt11char_traitsIcEERKNS0_7OptionsE":[[240,7,240,29,0,0,4,0],[240,7,240,29,0,0,4,1],[241,9,241,30,0,0,4,0],[253,7,253,67,0,0,4,1],[265,7,265,20,0,0,4,1]],"_ZN3re214CoalesceWalker9PostVisitEPNS_6RegexpES2_S2_PS2_i":[[238,7,238,22,0,0,4,0]],"_ZN3re24Prog8EmitListEiPNS_11SparseArrayIiEEPSt6vectorINS0_4InstESaIS5_EEPNS_10SparseSetTIvEEPS4_IiSaIiEE":[[794,10,794,23,0,0,4,0],[794,10,794,23,0,0,4,1],[798,9,798,32,0,0,4,1],[802,9,802,19,0,0,4,0],[802,9,802,19,0,0,4,1],[802,23,802,45,0,0,4,0],[802,23,802,45,0,0,4,1],[813,7,813,14,0,0,4,1],[817,7,817,25,0,0,4,1],[824,7,824,20,0,0,4,0],[824,7,824,20,0,0,4,1],[829,7,829,26,0,0,4,0],[829,7,829,26,0,0,4,1],[830,7,830,24,0,0,4,1],[831,7,831,27,0,0,4,1],[837,7,837,20,0,0,4,1],[841,7,841,22,0,0,4,0],[841,7,841,22,0,0,4,1],[842,7,842,21,0,0,4,0],[842,7,842,21,0,0,4,1]],"cre2_new":[[150,7,150,10,0,0,4,0]],"_ZN3re23RE27OptionsC2ENS0_13CannedOptionsE":[[60,15,60,33,0,0,4,1]],"_ZN3re24Prog9IsOnePassEv":[[384,7,384,19,0,0,4,1],[388,7,388,19,0,0,4,1],[397,7,397,24,0,0,4,1],[397,28,397,63,0,0,4,1],[423,46,423,65,0,0,4,0],[423,46,423,65,0,0,4,1],[430,21,430,39,0,0,4,0],[430,21,430,39,0,0,4,1],[439,12,439,22,0,0,4,0],[439,12,439,22,0,0,4,1],[446,9,446,16,0,0,4,1],[450,9,450,27,0,0,4,1],[460,9,460,28,0,0,4,1],[527,9,527,26,0,0,4,1],[528,9,528,29,0,0,4,1],[529,9,529,22,0,0,4,1],[559,9,559,24,0,0,4,0],[560,15,560,22,0,0,4,1],[570,15,570,25,0,0,4,0],[578,9,578,23,0,0,4,1]],"_ZN3re28Compiler5SetupENS_6Regexp10ParseFlagsElNS_3RE26AnchorE":[[1082,7,1082,29,0,0,4,1],[1085,7,1085,19,0,0,4,1],[1087,14,1087,58,0,0,4,1],[1100,9,1100,19,0,0,4,1],[1103,9,1103,33,0,0,4,1]],"_ZN3re26Regexp10ParseState11PushLiteralEi":[[411,7,411,24,0,0,4,1],[434,7,434,25,0,0,4,1],[438,7,438,35,0,0,4,0],[438,7,438,35,0,0,4,1]],"_ZN3re217NumCapturesWalker8PreVisitEPNS_6RegexpEiPb":[[559,9,559,35,0,0,4,1]],"_ZN3re2L16RegexpErrorToRE2ENS_16RegexpStatusCodeE":[[103,11,103,15,0,0,4,1],[104,5,104,29,0,0,4,1],[106,5,106,35,0,0,4,1],[108,5,108,31,0,0,4,1],[110,5,110,34,0,0,4,1],[112,5,112,34,0,0,4,1],[114,5,114,36,0,0,4,1],[116,5,116,34,0,0,4,1],[118,5,118,37,0,0,4,1],[120,5,120,39,0,0,4,1],[122,5,122,36,0,0,4,1],[124,5,124,32,0,0,4,1],[126,5,126,30,0,0,4,1],[128,5,128,31,0,0,4,1],[130,5,130,29,0,0,4,0],[132,5,132,37,0,0,4,1]],"_ZN3re2L10AddToQueueEPNS_10SparseSetTIvEEi":[[143,7,143,14,0,0,4,0],[143,7,143,14,0,0,4,1]],"_ZZNK3re23RE220NamedCapturingGroupsB5cxx11EvENK3$_0clEPKS0_":[[397,9,397,35,0,0,4,1],[399,9,399,34,0,0,4,0]],"_ZN3re26Regexp13ComputeSimpleEv":[[49,11,49,14,0,0,4,1],[50,5,50,24,0,0,4,1],[51,5,51,27,0,0,4,0],[51,5,51,27,0,0,4,1],[52,5,52,24,0,0,4,0],[52,5,52,24,0,0,4,1],[53,5,53,30,0,0,4,1],[54,5,54,26,0,0,4,1],[55,5,55,24,0,0,4,1],[56,5,56,26,0,0,4,1],[57,5,57,29,0,0,4,1],[58,5,58,31,0,0,4,1],[59,5,59,24,0,0,4,1],[60,5,60,24,0,0,4,1],[61,5,61,24,0,0,4,1],[62,5,62,26,0,0,4,1],[64,5,64,23,0,0,4,1],[65,5,65,26,0,0,4,1],[72,5,72,26,0,0,4,1],[77,5,77,24,0,0,4,1],[80,5,80,21,0,0,4,1],[81,5,81,21,0,0,4,1],[82,5,82,22,0,0,4,1],[97,5,97,23,0,0,4,1]],"_ZN3re2L16StringViewToRuneEPiPSt17basic_string_viewIcSt11char_traitsIcEEPNS_12RegexpStatusE":[[1428,7,1428,78,0,0,4,0],[1434,9,1434,21,0,0,4,1],[1438,11,1438,17,0,0,4,0],[1438,11,1438,17,0,0,4,1],[1438,21,1438,36,0,0,4,0],[1438,21,1438,36,0,0,4,1],[1444,7,1444,21,0,0,4,0]],"_ZN3re26Regexp10ParseStateD2Ev":[[206,32,206,42,0,0,4,0],[206,32,206,42,0,0,4,1],[209,9,209,31,0,0,4,1]],"_ZN3re26Regexp6WalkerIPS0_E5ResetEv":[[150,7,150,22,0,0,4,1]],"_ZN3re28fullruneEPKci":[[194,6,194,11,0,0,4,0],[196,7,196,13,0,0,4,0],[196,7,196,13,0,0,4,1],[198,7,198,12,0,0,4,0],[199,8,199,14,0,0,4,0],[199,8,199,14,0,0,4,1],[201,8,201,13,0,0,4,0],[202,9,202,15,0,0,4,1],[202,19,202,24,0,0,4,0]],"_ZN3re211SparseArrayIiE11SetInternalEbiRKi":[[214,9,214,70,0,0,4,1],[221,9,221,24,0,0,4,0]],"_ZN3re26RegexpD2Ev":[[49,7,49,16,0,0,4,1],[53,5,53,12,0,0,4,0],[53,5,53,12,0,0,4,1],[55,5,55,24,0,0,4,1],[58,5,58,30,0,0,4,0],[58,5,58,30,0,0,4,1],[61,5,61,26,0,0,4,1]],"_ZN3re26Regexp10ParseState10PushRegexpEPS0_":[[243,7,243,34,0,0,4,1],[260,7,260,26,0,0,4,0],[260,7,260,26,0,0,4,1]],"_ZN3re29PatchList5PatchEPNS_4Prog4InstES0_j":[[50,12,50,23,0,0,4,0],[50,12,50,23,0,0,4,1],[52,11,52,19,0,0,4,1]],"_ZN3re28Compiler9AllocInstEi":[[245,7,245,14,0,0,4,1],[245,18,245,41,0,0,4,1],[250,7,250,32,0,0,4,0],[250,7,250,32,0,0,4,1],[252,9,252,17,0,0,4,0],[254,12,254,28,0,0,4,1],[257,9,257,29,0,0,4,1]],"_ZN3re26Regexp6DecrefEv":[[129,7,129,22,0,0,4,1],[142,7,142,16,0,0,4,0],[142,7,142,16,0,0,4,1]],"_ZN3re212RegexpStatus8CodeTextB5cxx11ENS_16RegexpStatusCodeE":[[530,7,530,15,0,0,4,1],[530,19,530,41,0,0,4,1]],"_ZN3re26Regexp6WalkerIPS0_E12WalkInternalES2_S2_b":[[164,7,164,17,0,0,4,1],[177,7,177,14,0,0,4,0],[178,13,178,30,0,0,4,1],[185,13,185,17,0,0,4,0],[185,13,185,17,0,0,4,1],[191,13,191,27,0,0,4,1],[193,18,193,31,0,0,4,1],[197,7,197,14,0,0,4,1],[198,13,198,26,0,0,4,1],[212,13,212,26,0,0,4,1],[221,9,221,23,0,0,4,0]],"_ZN3re23RE2C2EPKc":[[149,7,149,22,0,0,4,1]],"_ZNK3re23RE27DoMatchESt17basic_string_viewIcSt11char_traitsIcEENS0_6AnchorEPmPKPKNS0_3ArgEi":[[924,7,924,12,0,0,4,1],[930,7,930,36,0,0,4,0]],"_ZN3re28Compiler5MatchEi":[[409,7,409,13,0,0,4,1]],"_ZN3re26Regexp10ParseStateC2ENS0_10ParseFlagsESt17basic_string_viewIcSt11char_traitsIcEEPNS_12RegexpStatusE":[[197,7,197,22,0,0,4,1]],"_ZN3re214ByteMapBuilder5MergeEv":[[388,8,388,27,0,0,4,1]],"_ZN3re26Regexp10ParseState17MaybeConcatStringEiNS0_10ParseFlagsE":[[1312,7,1312,32,0,0,4,0],[1312,7,1312,32,0,0,4,1],[1312,36,1312,62,0,0,4,0],[1312,36,1312,62,0,0,4,1],[1315,7,1315,33,0,0,4,1],[1317,7,1317,33,0,0,4,0],[1317,7,1317,33,0,0,4,1],[1317,37,1317,69,0,0,4,1],[1319,7,1319,71,0,0,4,1],[1322,7,1322,33,0,0,4,0],[1322,7,1322,33,0,0,4,1],[1332,7,1332,33,0,0,4,0],[1343,7,1343,13,0,0,4,0]],"_ZNK3re23RE27Options10ParseFlagsEv":[[169,5,169,12,0,0,4,1],[173,5,173,36,0,0,4,0],[175,5,175,38,0,0,4,1],[180,7,180,22,0,0,4,0],[183,7,183,16,0,0,4,1],[186,7,186,17,0,0,4,1],[189,7,189,15,0,0,4,1],[192,7,192,22,0,0,4,1],[195,7,195,24,0,0,4,1],[198,7,198,21,0,0,4,1],[201,7,201,22,0,0,4,1],[204,7,204,17,0,0,4,1]],"_ZN3re26Regexp12QuickDestroyEv":[[72,7,72,17,0,0,4,0]],"_ZN3re210SparseSetTIvE14InsertInternalEbi":[[147,9,147,70,0,0,4,1],[154,9,154,24,0,0,4,0],[154,9,154,24,0,0,4,1],[158,11,158,23,0,0,4,0]],"_ZN3re26Regexp10ParseState12FinishRegexpEPS0_":[[219,7,219,17,0,0,4,1],[223,7,223,34,0,0,4,1]],"_ZN3re210chartoruneEPiPKc":[[61,5,61,11,0,0,4,0],[61,5,61,11,0,0,4,1],[71,5,71,15,0,0,4,1],[73,5,73,11,0,0,4,0],[73,5,73,11,0,0,4,1],[74,6,74,12,0,0,4,0],[88,5,88,15,0,0,4,1],[90,5,90,11,0,0,4,1],[103,6,103,16,0,0,4,1],[105,6,105,12,0,0,4,0],[107,7,107,17,0,0,4,1]],"_ZN3re24Prog14MarkSuccessorsEPNS_11SparseArrayIiEES3_PSt6vectorIS4_IiSaIiEESaIS6_EEPNS_10SparseSetTIvEEPS6_":[[668,7,668,46,0,0,4,0],[670,7,670,35,0,0,4,0],[676,10,676,23,0,0,4,0],[676,10,676,23,0,0,4,1],[680,9,680,32,0,0,4,0],[680,9,680,32,0,0,4,1],[686,7,686,14,0,0,4,1],[690,7,690,25,0,0,4,1],[691,7,691,20,0,0,4,0],[691,7,691,20,0,0,4,1],[693,22,693,23,0,0,4,0],[693,22,693,23,0,0,4,1],[694,15,694,39,0,0,4,0],[704,7,704,26,0,0,4,0],[704,7,704,26,0,0,4,1],[705,7,705,24,0,0,4,1],[706,7,706,27,0,0,4,1],[708,13,708,43,0,0,4,1],[713,7,713,20,0,0,4,1],[717,7,717,22,0,0,4,0],[717,7,717,22,0,0,4,1],[718,7,718,21,0,0,4,1]],"_ZN3re26Regexp22RequiredPrefixForAccelEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEPb":[[744,16,744,36,0,0,4,1],[745,10,745,35,0,0,4,1],[750,7,750,32,0,0,4,0],[751,7,751,38,0,0,4,0]],"_ZN3re24Prog7FlattenEv":[[564,7,564,19,0,0,4,1],[585,8,585,27,0,0,4,0],[585,8,585,27,0,0,4,1],[587,9,587,41,0,0,4,0],[587,9,587,41,0,0,4,1],[587,45,587,66,0,0,4,1],[597,8,597,26,0,0,4,0],[597,8,597,26,0,0,4,1],[608,19,608,31,0,0,4,0],[608,19,608,31,0,0,4,1],[613,20,613,54,0,0,4,0],[613,20,613,54,0,0,4,1],[615,9,615,38,0,0,4,0],[629,7,629,30,0,0,4,1],[631,14,631,43,0,0,4,1],[646,7,646,19,0,0,4,0],[650,21,650,36,0,0,4,0],[650,21,650,36,0,0,4,1]],"_ZNK3re211SparseArrayIiE9has_indexEi":[[354,7,354,68,0,0,4,1],[358,10,358,48,0,0,4,0],[358,10,358,48,0,0,4,1],[359,10,359,40,0,0,4,0],[359,10,359,40,0,0,4,1]],"_ZN3re26Regexp10ParseState15DoConcatenationEv":[[1280,7,1280,17,0,0,4,0]],"_ZN3re26Regexp7DestroyEv":[[148,7,148,21,0,0,4,0]],"_ZN3re26Regexp6WalkerINS_4FragEE5ResetEv":[[150,7,150,22,0,0,4,1]],"_ZN3re26Regexp6WalkerIiE5ResetEv":[[150,7,150,22,0,0,4,1]],"_ZZN3re24Prog12ComputeHintsEPSt6vectorINS0_4InstESaIS2_EEiiENK3$_0clEii":[[886,11,886,18,0,0,4,1],[891,11,891,27,0,0,4,1],[898,14,898,21,0,0,4,0],[904,13,904,23,0,0,4,0]],"_ZN3re214ByteMapBuilder4MarkEii":[[380,7,380,14,0,0,4,0],[380,18,380,27,0,0,4,0]],"_ZN3re26Regexp6WalkerIiE12WalkInternalEPS0_ib":[[164,7,164,17,0,0,4,1],[177,7,177,14,0,0,4,0],[178,13,178,30,0,0,4,1],[185,13,185,17,0,0,4,1],[191,13,191,27,0,0,4,1],[193,18,193,31,0,0,4,1],[197,7,197,14,0,0,4,1],[198,13,198,26,0,0,4,1],[212,13,212,26,0,0,4,1],[221,9,221,23,0,0,4,0]],"_ZN3re28Compiler8PreVisitEPNS_6RegexpENS_4FragEPb":[[811,7,811,14,0,0,4,1]],"_ZN3re26Regexp10ParseState10DoCollapseENS_8RegexpOpE":[[1241,25,1241,36,0,0,4,0],[1241,25,1241,36,0,0,4,1],[1241,40,1241,60,0,0,4,0],[1243,9,1243,23,0,0,4,1],[1251,7,1251,24,0,0,4,0],[1251,28,1251,52,0,0,4,0]],"_ZN3re24Prog8OptimizeEv":[[237,39,237,51,0,0,4,0],[237,39,237,51,0,0,4,1],[243,12,243,18,0,0,4,1],[249,9,249,33,0,0,4,1],[268,39,268,51,0,0,4,0],[268,39,268,51,0,0,4,1],[272,9,272,33,0,0,4,1],[275,9,275,33,0,0,4,1]],"_ZNK3re211SparseArrayIiE8max_sizeEv":[[162,9,162,30,0,0,4,0]],"_ZN3re2L4AddQEPNS_10SparseSetTIvEEi":[[355,7,355,14,0,0,4,1],[357,7,357,22,0,0,4,1]],"_ZNK3re29Bitmap25614FindNextSetBitEi":[[20,7,20,16,0,0,4,1],[26,5,26,11,0,0,4,0],[27,11,27,25,0,0,4,1],[30,5,30,11,0,0,4,1],[31,11,31,25,0,0,4,1],[34,5,34,11,0,0,4,1],[35,11,35,25,0,0,4,0],[38,5,38,12,0,0,4,1]],"_ZN3re26Regexp5ParseESt17basic_string_viewIcSt11char_traitsIcEENS0_10ParseFlagsEPNS_12RegexpStatusE":[[2269,7,2269,21,0,0,4,1],[2276,7,2276,28,0,0,4,1],[2283,7,2283,29,0,0,4,1],[2296,10,2296,20,0,0,4,0],[2296,10,2296,20,0,0,4,1],[2299,7,2299,14,0,0,4,0],[2301,13,2301,49,0,0,4,0],[2301,13,2301,49,0,0,4,1],[2303,13,2303,31,0,0,4,1],[2308,7,2308,15,0,0,4,1],[2326,7,2326,15,0,0,4,1],[2332,7,2332,15,0,0,4,1],[2338,7,2338,15,0,0,4,1],[2344,7,2344,15,0,0,4,1],[2350,7,2350,15,0,0,4,1],[2356,7,2356,15,0,0,4,1],[2365,7,2365,15,0,0,4,1],[2369,7,2369,15,0,0,4,1],[2372,7,2372,15,0,0,4,1],[2403,7,2403,15,0,0,4,1],[2436,7,2436,16,0,0,4,1]],"_ZN3re28Compiler6FinishEPNS_6RegexpE":[[1164,7,1164,14,0,0,4,1],[1167,7,1167,26,0,0,4,1],[1180,7,1180,25,0,0,4,0],[1183,9,1183,62,0,0,4,1],[1188,7,1188,20,0,0,4,1],[1193,9,1193,29,0,0,4,0],[1195,9,1195,14,0,0,4,1]],"_ZN3re214SimplifyWalker8PreVisitEPNS_6RegexpES2_Pb":[[460,7,460,19,0,0,4,0]],"_ZN3re26Regexp6WalkerINS_4FragEE12WalkInternalEPS0_S2_b":[[164,7,164,17,0,0,4,1],[177,7,177,14,0,0,4,0],[178,13,178,30,0,0,4,1],[185,13,185,17,0,0,4,1],[191,13,191,27,0,0,4,1],[193,18,193,31,0,0,4,1],[197,7,197,14,0,0,4,1],[198,13,198,26,0,0,4,1],[212,13,212,26,0,0,4,1],[221,9,221,23,0,0,4,0]],"_ZN3re28Compiler7CompileEPNS_6RegexpEbl":[[1123,7,1123,18,0,0,4,1],[1134,7,1134,16,0,0,4,1],[1144,7,1144,26,0,0,4,1],[1153,7,1153,31,0,0,4,0]],"_ZN3re24ProgD2Ev":[[136,7,136,23,0,0,4,1]],"_ZN3re28Compiler9PostVisitEPNS_6RegexpENS_4FragES3_PS3_i":[[844,7,844,14,0,0,4,1],[848,11,848,19,0,0,4,1],[849,5,849,23,0,0,4,1],[853,5,853,24,0,0,4,1],[856,5,856,27,0,0,4,0],[859,5,859,26,0,0,4,1],[869,5,869,23,0,0,4,1],[876,5,876,26,0,0,4,1],[883,5,883,21,0,0,4,1],[886,5,886,21,0,0,4,1],[889,5,889,22,0,0,4,1],[892,5,892,24,0,0,4,1],[895,5,895,30,0,0,4,1],[911,5,911,24,0,0,4,1],[916,5,916,24,0,0,4,1],[919,5,919,26,0,0,4,1],[956,5,956,24,0,0,4,1],[963,5,963,26,0,0,4,1],[966,5,966,24,0,0,4,1],[969,5,969,26,0,0,4,1],[972,5,972,24,0,0,4,1],[975,5,975,29,0,0,4,1],[978,5,978,31,0,0,4,1]],"_ZN3re214ByteMapBuilder5BuildEPhPi":[[422,10,422,17,0,0,4,0],[422,10,422,17,0,0,4,1],[425,12,425,21,0,0,4,0],[425,12,425,21,0,0,4,1]],"_ZN3re28Compiler3NopEv":[[400,7,400,13,0,0,4,1]]},"library_calls":["cre2_version_string","cre2_version_interface_revision","cre2_opt_new","cre2_new","cre2_opt_delete","cre2_num_capturing_groups","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_named_groups_iter_delete","cre2_partial_match","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_version_string","cre2_version_interface_revision","cre2_opt_new","cre2_new","cre2_num_capturing_groups","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_named_groups_iter_delete","cre2_partial_match","cre2_delete","cre2_opt_delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open input as memory file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get version information
    const char *version_str = cre2_version_string();
    int interface_rev = cre2_version_interface_revision();
    
    // Read pattern and text from input data
    // First 4 bytes: pattern length
    // Next: pattern string
    // Next 4 bytes: text length  
    // Next: text string
    // Rest: capture groups count
    
    uint32_t pattern_len = 0;
    uint32_t text_len = 0;
    int32_t capture_groups = 0;
    
    // Read pattern length
    if (fread(&pattern_len, sizeof(pattern_len), 1, in_file) != 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Limit pattern length to prevent excessive memory usage
    if (pattern_len > 1024 || pattern_len == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read pattern
    std::vector<char> pattern(pattern_len + 1);
    if (fread(pattern.data(), 1, pattern_len, in_file) != pattern_len) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    pattern[pattern_len] = '\0';
    
    // Read text length
    if (fread(&text_len, sizeof(text_len), 1, in_file) != 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Limit text length
    if (text_len > 4096 || text_len == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read text
    std::vector<char> text(text_len + 1);
    if (fread(text.data(), 1, text_len, in_file) != text_len) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    text[text_len] = '\0';
    
    // Read capture groups count
    if (fread(&capture_groups, sizeof(capture_groups), 1, in_file) != 1) {
        capture_groups = 10; // Default value
    }
    
    assert_file_closed(&in_file);;
    
    // Create regexp object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    cre2_regexp_t *re = cre2_new(pattern.data(), pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get number of capturing groups
    int num_groups = cre2_num_capturing_groups(re);
    
    // Create named groups iterator
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *group_name = nullptr;
        int group_index = 0;
        
        // Iterate through named groups
        while (cre2_named_groups_iter_next(iter, &group_name, &group_index)) {
            // Process named group (just for fuzzing)
            if (group_name) {
                // Access group_name to ensure it's read
                volatile char c = group_name[0];
                (void)c;
            }
        }
        
        cre2_named_groups_iter_delete(iter);
    }
    
    // Prepare strings for partial match
    cre2_string_t text_str;
    text_str.data = text.data();
    text_str.length = text_len;
    
    // Allocate match array
    std::vector<cre2_string_t> matches(capture_groups > 0 ? capture_groups : 10);
    
    // Perform partial match
    int match_result = cre2_partial_match(pattern.data(), &text_str, matches.data(), 
                                          matches.size());
    
    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    
    assert_file_closed(&in_file);
	return 0;
}