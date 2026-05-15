#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 1224, nr_unique_branch: 536
//<Quality> {"density":36,"unique_branches":{"_Fax3Close":[[1303,9,1303,52,0,0,4,1]],"TIFFWriteDirectoryTagShortPerSample":[[1668,9,1668,20,0,0,4,0],[1668,9,1668,20,0,0,4,1],[1676,9,1676,18,0,0,4,1],[1681,26,1681,62,0,0,4,0],[1681,26,1681,62,0,0,4,1]],"tagCompare":[[608,16,608,44,0,0,4,1]],"TIFFAppendToStrip":[[775,9,775,37,0,0,4,1],[780,9,780,40,0,0,4,1],[785,9,785,25,0,0,4,1],[791,9,791,29,0,0,4,0],[794,9,794,41,0,0,4,0],[798,13,798,48,0,0,4,1],[839,9,839,41,0,0,4,0],[841,9,841,30,0,0,4,1],[841,34,841,52,0,0,4,1],[847,9,847,35,0,0,4,1],[930,9,930,17,0,0,4,1],[939,9,939,66,0,0,4,0]],"TIFFWriteBufferSetup":[[692,9,692,31,0,0,4,0],[697,13,697,49,0,0,4,0],[702,13,702,28,0,0,4,0],[702,13,702,28,0,0,4,1],[706,9,706,19,0,0,4,0],[709,13,709,23,0,0,4,0],[709,13,709,23,0,0,4,1],[282,22,282,62,3,0,4,1]],"OJPEGReadHeaderInfoSec":[[1503,9,1503,27,0,0,4,1],[1609,9,1609,31,0,0,4,1],[1611,9,1611,25,0,0,4,0],[1613,13,1613,57,0,0,4,0]],"DoubleToSrational":[[2955,9,2955,18,0,0,4,0],[2955,9,2955,18,0,0,4,1],[2962,9,2962,28,0,0,4,0],[2962,9,2962,28,0,0,4,1],[2969,9,2969,34,0,0,4,0]],"TIFFWriteDirectoryTagCheckedDoubleArray":[[3043,9,3043,20,0,0,4,0],[3043,9,3043,20,0,0,4,1],[3049,9,3049,35,0,0,4,1]],"TIFFClampDoubleToUInt8":[[1501,9,1501,16,0,0,4,1],[1503,9,1503,18,0,0,4,1],[1503,22,1503,32,0,0,4,1]],"TIFFWriteDirectoryTagCheckedShortArray":[[2408,9,2408,20,0,0,4,0],[2408,9,2408,20,0,0,4,1],[2413,9,2413,35,0,0,4,1]],"TIFFWriteDirectoryTagSampleformatArray":[[1536,9,1536,21,0,0,4,1],[1544,9,1544,33,0,0,4,1],[1558,9,1558,30,0,0,4,1],[1581,9,1581,31,0,0,4,0],[1582,17,1582,51,0,0,4,0],[1582,17,1582,51,0,0,4,1],[1584,29,1584,38,0,0,4,0],[1584,29,1584,38,0,0,4,1],[1589,22,1589,57,0,0,4,1],[1598,29,1598,38,0,0,4,0],[1598,29,1598,38,0,0,4,1],[1604,9,1604,16,0,0,4,1]],"TIFFWriteDirectoryTagCheckedUndefinedArray":[[2343,9,2343,20,0,0,4,0],[2343,9,2343,20,0,0,4,1]],"TIFFFillTile":[[1238,9,1238,47,0,0,4,1]],"TIFFWriteDirectoryTagCheckedRationalArray":[[2590,9,2590,20,0,0,4,0],[2590,9,2590,20,0,0,4,1],[2596,9,2596,18,0,0,4,1],[2601,38,2601,48,0,0,4,0],[2601,38,2601,48,0,0,4,1],[2605,9,2605,35,0,0,4,1]],"TIFFStartTile":[[1493,9,1493,48,0,0,4,0],[1495,13,1495,42,0,0,4,1],[1500,9,1500,30,0,0,4,1],[1506,9,1506,23,0,0,4,1],[1513,9,1513,23,0,0,4,1],[1520,9,1520,40,0,0,4,0],[314,6,314,54,3,0,4,0],[314,6,314,54,4,0,4,0]],"TIFFWriteDirectoryTagLongLong8Array":[[1965,9,1965,48,0,0,4,1],[1979,9,1979,38,0,0,4,1],[2002,9,2002,18,0,0,4,0],[2002,9,2002,18,0,0,4,1],[2002,22,2002,52,0,0,4,0],[2002,22,2002,52,0,0,4,1],[2006,14,2006,23,0,0,4,0],[2006,14,2006,23,0,0,4,1],[2006,27,2006,56,0,0,4,1],[2010,9,2010,22,0,0,4,0],[2010,9,2010,22,0,0,4,1],[2022,13,2022,22,0,0,4,1],[2028,41,2028,51,0,0,4,0],[2028,41,2028,51,0,0,4,1],[2030,17,2030,33,0,0,4,0],[2030,17,2030,33,0,0,4,1],[2052,13,2052,22,0,0,4,1],[2058,41,2058,51,0,0,4,0],[2058,41,2058,51,0,0,4,1],[2060,17,2060,29,0,0,4,0],[2060,17,2060,29,0,0,4,1]],"TIFFReadDirEntryLongArray":[[2128,9,2128,25,0,0,4,0]],"TIFFReadDirEntryShortArray":[[1880,25,1880,34,0,0,4,1]],"TIFFWriteDirectoryTagCheckedIfdArray":[[3061,9,3061,20,0,0,4,0],[3061,9,3061,20,0,0,4,1],[3066,9,3066,35,0,0,4,1]],"TIFFCheckTile":[[75,9,75,31,0,0,4,1],[81,9,81,32,0,0,4,1],[88,9,88,31,0,0,4,1],[94,9,94,53,0,0,4,1]],"TIFFWriteDirectoryTagCheckedSrationalArray":[[2626,9,2626,20,0,0,4,0],[2626,9,2626,20,0,0,4,1],[2632,9,2632,18,0,0,4,1],[2637,38,2637,48,0,0,4,0],[2637,38,2637,48,0,0,4,1],[2641,9,2641,35,0,0,4,1]],"OJPEGReadHeaderInfoSecTablesQTable":[[2025,9,2025,34,0,0,4,0]],"ToRationalEuclideanGCD":[[2764,9,2764,25,0,0,4,0],[2764,9,2764,25,0,0,4,1],[2780,9,2780,26,0,0,4,1],[2802,12,2802,35,0,0,4,0],[2802,12,2802,35,0,0,4,1],[2802,39,2802,53,0,0,4,0],[2802,57,2802,74,0,0,4,0],[2802,57,2802,74,0,0,4,1],[2812,17,2812,35,0,0,4,0],[2812,17,2812,35,0,0,4,1],[2816,13,2816,26,0,0,4,0],[2816,13,2816,26,0,0,4,1],[2830,13,2830,56,0,0,4,0],[2830,13,2830,56,0,0,4,1],[2833,17,2833,31,0,0,4,0],[2833,17,2833,31,0,0,4,1],[2833,35,2833,58,0,0,4,1],[2853,12,2853,35,0,0,4,1],[2853,39,2853,64,0,0,4,1]],"TIFFWriteDirectoryTagSubifd":[[2257,9,2257,37,0,0,4,0],[2257,9,2257,37,0,0,4,1],[2260,9,2260,41,0,0,4,0],[2268,13,2268,22,0,0,4,1],[2275,21,2275,48,0,0,4,0],[2275,21,2275,48,0,0,4,1],[2281,17,2281,35,0,0,4,0],[2281,17,2281,35,0,0,4,1],[2298,9,2298,20,0,0,4,0],[2298,9,2298,20,0,0,4,1],[2303,9,2303,11,0,0,4,1],[2315,9,2315,37,0,0,4,1]],"Fax3VGetField":[[1440,9,1440,34,0,0,4,0],[1446,9,1446,16,0,0,4,1]],"TIFFFlushData1":[[952,9,952,27,0,0,4,0],[952,9,952,27,0,0,4,1],[952,31,952,62,0,0,4,0],[954,13,954,25,0,0,4,1],[957,13,959,50,0,0,4,1],[282,22,282,62,4,0,4,1]],"OJPEGReadHeaderInfo":[[1193,9,1193,45,0,0,4,0],[1193,9,1193,45,0,0,4,1],[1203,13,1203,49,0,0,4,1],[1213,13,1213,64,0,0,4,0],[1218,9,1218,45,0,0,4,1],[1240,9,1240,41,0,0,4,0],[282,22,282,62,1,0,4,0]],"TIFFWriteDirectoryTagCheckedLongArray":[[2463,9,2463,20,0,0,4,0],[2463,9,2463,20,0,0,4,1],[2468,9,2468,35,0,0,4,1]],"PredictorVGetField":[[1077,9,1077,31,0,0,4,0],[1080,9,1080,16,0,0,4,1]],"_TIFFCheckDirNumberAndOffset":[[5762,9,5762,20,0,0,4,0],[5805,13,5805,42,0,0,4,0]],"DumpModeEncode":[[44,12,44,18,0,0,4,0],[44,12,44,18,0,0,4,1],[49,13,49,54,0,0,4,1],[58,13,58,33,0,0,4,0],[64,13,64,51,0,0,4,1]],"TIFFFlush":[[43,9,43,43,0,0,4,0],[44,9,44,45,0,0,4,1]],"TIFFCheckRead":[[1539,9,1539,34,0,0,4,1],[1544,9,1544,24,0,0,4,1]],"TIFFWriteDirectoryTagShortLong":[[1908,9,1908,24,0,0,4,1]],"TIFFWriteDirectoryTagCheckedByteArray":[[2358,9,2358,20,0,0,4,0],[2358,9,2358,20,0,0,4,1]],"TIFFClientOpenExt":[[421,26,421,37,0,0,4,1],[556,9,556,22,0,0,4,0]],"_WriteAsType":[[1919,9,1919,40,0,0,4,0],[1919,9,1919,40,0,0,4,1],[1923,14,1923,45,0,0,4,1],[1924,14,1924,44,0,0,4,0],[1924,14,1924,44,0,0,4,1],[1925,14,1925,54,0,0,4,1],[1926,14,1926,48,0,0,4,1],[1927,14,1927,45,0,0,4,1],[1928,14,1928,45,0,0,4,1],[1929,14,1929,45,0,0,4,1],[1930,14,1930,45,0,0,4,1],[1930,49,1930,79,0,0,4,1]],"TIFFReadEncodedTile":[[977,9,977,31,0,0,4,1],[979,9,979,31,0,0,4,1],[988,9,988,47,0,0,4,0],[988,9,988,47,0,0,4,1],[988,51,988,73,0,0,4,0],[989,9,989,25,0,0,4,0],[989,29,989,38,0,0,4,0],[990,9,990,49,0,0,4,0],[992,13,992,75,0,0,4,0],[1003,9,1003,31,0,0,4,1],[1005,14,1005,29,0,0,4,1],[1007,9,1007,33,0,0,4,0],[1010,13,1010,16,0,0,4,0]],"TIFFWriteScanline":[[72,9,72,34,0,0,4,1],[87,9,87,53,0,0,4,1],[107,9,107,32,0,0,4,1],[109,9,109,35,0,0,4,0],[109,9,109,35,0,0,4,1],[114,13,114,32,0,0,4,1],[122,13,122,43,0,0,4,1],[125,13,125,39,0,0,4,1],[131,13,131,52,0,0,4,0],[133,17,133,46,0,0,4,1],[144,13,144,48,0,0,4,1],[153,9,153,28,0,0,4,1],[38,6,38,43,1,0,4,0],[38,6,38,43,1,0,4,1],[38,47,38,79,1,0,4,0],[42,7,42,44,2,0,4,0],[42,7,42,44,2,0,4,1],[42,48,42,64,2,0,4,0],[43,6,43,53,2,0,4,0],[43,6,43,53,2,0,4,1]],"TIFFComputeTile":[[44,9,44,31,0,0,4,0],[46,9,46,27,0,0,4,1],[48,9,48,27,0,0,4,1],[50,9,50,27,0,0,4,1],[52,9,52,16,0,0,4,0],[52,20,52,27,0,0,4,0],[52,31,52,38,0,0,4,0],[58,13,58,57,0,0,4,1],[314,6,314,54,1,0,4,0],[314,6,314,54,2,0,4,0],[314,6,314,54,3,0,4,0]],"TIFFReadRawTile1":[[1141,9,1141,18,0,0,4,0],[1145,13,1145,20,0,0,4,1],[1154,13,1154,23,0,0,4,0]],"JPEGResetUpsampled":[[2613,9,2613,34,0,0,4,0]],"DoubleToRational":[[2878,9,2878,22,0,0,4,1],[2887,9,2887,29,0,0,4,0],[2887,9,2887,29,0,0,4,1],[2894,9,2894,35,0,0,4,0],[2894,9,2894,35,0,0,4,1],[2901,9,2901,43,0,0,4,0],[2901,9,2901,43,0,0,4,1],[2915,9,2915,30,0,0,4,1],[2915,34,2915,57,0,0,4,1],[2916,9,2916,31,0,0,4,1],[2916,35,2916,59,0,0,4,1],[2929,9,2929,27,0,0,4,0],[2929,9,2929,27,0,0,4,1]],"TIFFReadDirEntryLong":[[728,17,728,45,0,0,4,0]],"TIFFWriteDirectoryTagCheckedAscii":[[2327,9,2327,20,0,0,4,0],[2327,9,2327,20,0,0,4,1]],"EvaluateIFDdatasizeWrite":[[2142,9,2142,69,0,0,4,0],[2142,9,2142,69,0,0,4,1],[2142,23,2142,54,0,0,4,1],[2146,13,2146,27,0,0,4,0],[2146,13,2146,27,0,0,4,1]],"OJPEGSubsamplingCorrect":[[1093,9,1093,47,0,0,4,0],[1097,13,1097,37,0,0,4,1]],"TIFFReadDirectory":[[4725,35,4725,66,0,0,4,0]],"TIFFLinkDirectory":[[3249,13,3249,53,0,0,4,0],[3257,17,3257,25,0,0,4,1],[3262,17,3262,49,0,0,4,0],[3262,17,3262,49,0,0,4,1]],"TIFFDeferStrileArrayWriting":[[271,9,271,34,0,0,4,1],[276,9,276,29,0,0,4,0],[276,9,276,29,0,0,4,1]],"TIFFWriteCheck":[[609,9,609,34,0,0,4,1],[614,9,614,24,0,0,4,1],[634,9,634,22,0,0,4,1],[640,9,640,46,0,0,4,0],[640,50,640,71,0,0,4,1],[656,9,656,35,0,0,4,1],[660,9,660,56,0,0,4,1],[282,22,282,62,4,0,4,1]],"TIFFYCbCrtoRGB":[[197,26,197,37,1,0,4,1],[196,6,196,23,2,0,4,1],[196,34,196,45,2,0,4,1],[196,6,196,23,3,0,4,1],[196,34,196,45,3,0,4,1],[196,6,196,23,4,0,4,1],[196,34,196,45,4,0,4,0],[196,6,196,23,6,0,4,1],[196,34,196,45,6,0,4,1],[196,6,196,23,7,0,4,1],[196,34,196,45,7,0,4,0]],"TIFFWriteDirectoryTagColormap":[[2161,9,2161,20,0,0,4,0],[2161,9,2161,20,0,0,4,1],[2168,9,2168,18,0,0,4,1]],"_TIFFVGetField":[[1228,9,1228,33,0,0,4,0],[1249,9,1249,37,0,0,4,0],[1339,9,1339,34,0,0,4,0],[1343,9,1343,31,0,0,4,0],[1346,9,1346,32,0,0,4,0]],"_TIFFErrorEarly":[[84,9,84,13,0,0,4,1]],"TIFFClampDoubleToUInt32":[[1519,9,1519,16,0,0,4,0],[1519,9,1519,16,0,0,4,1],[1521,9,1521,26,0,0,4,1],[1521,30,1521,40,0,0,4,1]],"OJPEGPreDecode":[[714,9,714,41,0,0,4,0],[714,9,714,41,0,0,4,1],[716,9,716,33,0,0,4,0],[718,13,718,42,0,0,4,0]],"TIFFWriteDirectoryTagCheckedRational":[[2547,9,2547,18,0,0,4,1],[2552,14,2552,28,0,0,4,1],[2558,9,2558,20,0,0,4,0],[2558,9,2558,20,0,0,4,1],[2561,13,2561,44,0,0,4,1],[2568,9,2568,35,0,0,4,1]],"TIFFWriteDirectoryTagData":[[3102,13,3102,34,0,0,4,0],[3106,9,3106,20,0,0,4,0],[3109,25,3109,30,0,0,4,0],[3109,25,3109,30,0,0,4,1],[3116,9,3116,70,0,0,4,1],[3118,13,3118,17,0,0,4,1],[3128,13,3128,45,0,0,4,0],[3130,13,3130,22,0,0,4,1],[3130,26,3130,43,0,0,4,1],[3135,13,3135,20,0,0,4,1],[3140,13,3140,39,0,0,4,1],[3147,13,3147,21,0,0,4,1],[3153,13,3153,33,0,0,4,0],[3153,13,3153,33,0,0,4,1],[3155,13,3155,45,0,0,4,0],[3159,17,3159,43,0,0,4,1]],"_TIFFVSetField":[[386,17,386,33,0,0,4,0]],"TIFFWriteDirectorySec":[[536,13,536,45,0,0,4,0],[536,49,536,65,0,0,4,0],[536,49,536,65,0,0,4,1],[584,21,586,79,0,0,4,1],[588,21,590,80,0,0,4,1],[595,21,597,80,0,0,4,1],[599,21,601,80,0,0,4,1],[606,21,608,78,0,0,4,1],[610,21,612,78,0,0,4,1],[617,21,619,76,0,0,4,1],[624,21,626,55,0,0,4,1],[638,21,640,77,0,0,4,1],[645,21,647,79,0,0,4,1],[652,21,654,75,0,0,4,1],[659,21,661,77,0,0,4,1],[680,21,682,56,0,0,4,1],[687,21,689,56,0,0,4,1],[701,21,703,80,0,0,4,1],[708,21,710,56,0,0,4,1],[716,21,716,61,0,0,4,1],[723,21,723,29,0,0,4,0],[723,21,723,29,0,0,4,1],[725,25,728,62,0,0,4,0],[725,25,728,62,0,0,4,1],[733,25,736,62,0,0,4,1],[743,21,743,58,0,0,4,0],[743,21,743,58,0,0,4,1],[750,21,750,29,0,0,4,0],[750,21,750,29,0,0,4,1],[763,25,763,62,0,0,4,0],[764,25,767,59,0,0,4,0],[764,25,767,59,0,0,4,1],[772,25,775,59,0,0,4,1],[781,21,781,68,0,0,4,1],[786,21,786,49,0,0,4,0],[786,21,786,49,0,0,4,1],[791,25,792,75,0,0,4,1],[813,21,816,57,0,0,4,1],[842,21,844,62,0,0,4,1],[868,21,871,50,0,0,4,1],[876,21,878,78,0,0,4,1],[883,21,883,66,0,0,4,0],[883,21,883,66,0,0,4,1],[897,25,897,58,0,0,4,0],[901,29,901,51,0,0,4,1],[916,29,916,52,0,0,4,0],[916,29,916,52,0,0,4,1],[923,37,925,43,0,0,4,1],[929,29,929,52,0,0,4,1],[942,29,942,55,0,0,4,0],[942,29,942,55,0,0,4,1],[950,37,952,59,0,0,4,1],[956,29,956,55,0,0,4,1],[957,29,957,51,0,0,4,1],[958,29,958,52,0,0,4,1],[959,29,959,52,0,0,4,1],[960,29,960,52,0,0,4,0],[960,29,960,52,0,0,4,1],[961,29,961,52,0,0,4,1],[962,29,962,51,0,0,4,1],[963,29,963,52,0,0,4,1],[964,29,964,50,0,0,4,1],[965,29,965,49,0,0,4,1],[966,29,966,57,0,0,4,1],[967,29,967,54,0,0,4,1],[968,29,968,54,0,0,4,1],[969,29,969,54,0,0,4,1],[970,29,970,55,0,0,4,1],[971,29,971,55,0,0,4,1],[972,29,972,55,0,0,4,1],[973,29,973,55,0,0,4,1],[974,29,974,55,0,0,4,1],[975,29,975,55,0,0,4,1],[976,29,976,54,0,0,4,1],[977,29,977,55,0,0,4,1],[978,29,978,53,0,0,4,1],[979,29,979,55,0,0,4,1],[980,29,980,55,0,0,4,1],[981,29,981,55,0,0,4,1],[982,29,982,56,0,0,4,1],[983,29,983,56,0,0,4,1],[984,29,984,56,0,0,4,1],[985,29,985,56,0,0,4,1],[986,29,986,56,0,0,4,1],[987,29,987,56,0,0,4,1],[988,29,988,55,0,0,4,1],[989,29,989,56,0,0,4,1],[990,29,990,54,0,0,4,1],[991,29,991,55,0,0,4,1],[992,29,992,55,0,0,4,1],[993,29,993,56,0,0,4,1],[994,29,994,56,0,0,4,1],[995,29,995,56,0,0,4,1],[996,29,996,56,0,0,4,1],[997,29,997,56,0,0,4,1],[998,29,998,56,0,0,4,1],[999,29,999,55,0,0,4,1],[1000,29,1000,56,0,0,4,1],[1001,29,1001,54,0,0,4,1],[1002,29,1002,51,0,0,4,1],[1003,29,1003,51,0,0,4,1],[1004,29,1004,36,0,0,4,1],[1009,37,1009,50,0,0,4,0],[1018,21,1018,69,0,0,4,0],[1025,17,1025,32,0,0,4,0],[1025,17,1025,32,0,0,4,1],[1026,25,1028,75,0,0,4,1],[1031,17,1031,36,0,0,4,1],[1037,17,1037,31,0,0,4,0],[1037,17,1037,31,0,0,4,1],[1038,25,1040,78,0,0,4,1],[1043,17,1043,32,0,0,4,1],[1049,17,1049,32,0,0,4,0],[1049,17,1049,32,0,0,4,1],[1050,25,1052,79,0,0,4,1],[1055,17,1055,33,0,0,4,1],[1061,17,1061,31,0,0,4,0],[1061,17,1061,31,0,0,4,1],[1062,25,1064,79,0,0,4,1],[1067,17,1067,32,0,0,4,1],[1073,17,1073,32,0,0,4,1],[1079,17,1079,33,0,0,4,1],[1085,17,1085,35,0,0,4,0],[1085,17,1085,35,0,0,4,1],[1093,25,1093,37,0,0,4,1],[1104,29,1106,80,0,0,4,1],[1110,29,1110,41,0,0,4,1],[1121,17,1121,36,0,0,4,0],[1121,17,1121,36,0,0,4,1],[1129,25,1129,37,0,0,4,1],[1140,29,1142,80,0,0,4,1],[1146,29,1146,41,0,0,4,1],[1157,17,1157,32,0,0,4,1],[1163,17,1163,33,0,0,4,0],[1163,17,1163,33,0,0,4,1],[1164,25,1166,77,0,0,4,1],[1169,17,1169,30,0,0,4,0],[1169,17,1169,30,0,0,4,1],[1170,25,1172,79,0,0,4,1],[1175,17,1175,31,0,0,4,1],[1181,17,1181,33,0,0,4,1],[1182,17,1182,24,0,0,4,1],[1215,17,1215,37,0,0,4,1],[1220,22,1221,54,0,0,4,0],[1220,22,1221,54,0,0,4,1],[1223,21,1223,32,0,0,4,0],[1228,21,1228,79,0,0,4,1],[1276,48,1276,73,0,0,4,0],[1276,48,1276,73,0,0,4,1],[1282,21,1282,31,0,0,4,0],[1282,21,1282,31,0,0,4,1],[1287,21,1287,51,0,0,4,1],[1390,20,1390,52,0,0,4,0],[1392,15,1392,47,0,0,4,1],[1396,17,1396,69,0,0,4,0],[1429,20,1429,52,0,0,4,0],[1430,11,1430,43,0,0,4,1],[1437,22,1437,54,0,0,4,0],[1437,58,1437,79,0,0,4,1],[1441,9,1441,77,0,0,4,0],[1465,9,1465,20,0,0,4,0],[236,34,236,71,12,0,4,0],[236,34,236,71,15,0,4,0],[236,34,236,71,18,0,4,0],[236,34,236,71,21,0,4,0],[236,34,236,71,23,0,4,0],[236,34,236,71,27,0,4,0],[236,34,236,71,29,0,4,0],[236,34,236,71,31,0,4,0],[236,34,236,71,33,0,4,0],[236,34,236,71,39,0,4,0],[236,34,236,71,41,0,4,0],[236,34,236,71,43,0,4,1],[236,34,236,71,45,0,4,0],[236,34,236,71,47,0,4,0],[236,34,236,71,53,0,4,0],[236,34,236,71,57,0,4,0],[236,34,236,71,58,0,4,0],[236,34,236,71,65,0,4,0],[236,34,236,71,73,0,4,0],[236,34,236,71,80,0,4,0],[236,34,236,71,82,0,4,0],[236,34,236,71,84,0,4,0],[236,34,236,71,91,0,4,0]],"TIFFWriteDirectoryTagCheckedLong":[[2443,9,2443,20,0,0,4,0],[2443,9,2443,20,0,0,4,1],[2450,9,2450,35,0,0,4,1]],"TIFFFlushData":[[148,9,148,49,0,0,4,1],[150,9,150,41,0,0,4,0],[150,9,150,41,0,0,4,1],[153,13,153,41,0,0,4,1]],"_TIFFgetMode":[[62,9,62,17,0,0,4,1],[67,9,67,17,0,0,4,0],[70,17,70,31,0,0,4,0]],"JPEGVGetField":[[2685,9,2685,32,0,0,4,0],[2698,9,2698,16,0,0,4,1]],"TIFFSetupStrips":[[569,9,570,70,0,0,4,1],[570,24,570,55,0,0,4,1],[576,9,576,53,0,0,4,1],[579,9,579,37,0,0,4,1],[583,9,583,40,0,0,4,1],[587,9,587,37,0,0,4,1],[587,41,587,72,0,0,4,1],[282,22,282,62,1,0,4,1],[236,34,236,71,10,0,4,1]],"TIFFRewriteDirectorySec":[[321,9,321,29,0,0,4,0],[332,13,332,67,0,0,4,0],[338,17,338,25,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFFileno","TIFFGetSizeProc","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFCheckTile","TIFFComputeTile","TIFFTileSize","_TIFFmalloc","TIFFReadEncodedTile","TIFFWriteRawTile","_TIFFfree","TIFFDeferStrileArrayWriting","TIFFFindField","TIFFFieldIsAnonymous","TIFFGetField","TIFFSetField","TIFFGetField","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","TIFFYCbCrtoRGB","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","TIFFWriteDirectory","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFScanlineSize","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFOpen","TIFFFileno","TIFFGetSizeProc","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFCheckTile","TIFFComputeTile","TIFFTileSize","_TIFFmalloc","TIFFReadEncodedTile","TIFFWriteRawTile","_TIFFfree","TIFFDeferStrileArrayWriting","TIFFFindField","TIFFFieldIsAnonymous","TIFFGetField","TIFFSetField","TIFFGetField","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","TIFFYCbCrtoRGB","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","TIFFWriteDirectory","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFScanlineSize","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFClose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Create input and output files
    const char *input_filename = "input_file";
    const char *output_filename = "output_file";
    
    // Write input data to file
    FILE *in_fp = fopen(input_filename, "wb");
    if (!in_fp) {
	assert_file_closed(&in_fp);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, in_fp);
    assert_file_closed(&in_fp);;
    
    // Step 2: Open TIFF file
    TIFF *tif = TIFFOpen(input_filename, "r+");
    if (!tif) {
        remove(input_filename);
        assert_file_closed(&in_fp);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Use TIFFFileno API
    int fd = TIFFFileno(tif);
    
    // Step 4: Use TIFFGetSizeProc API
    TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
    
    // Step 5: Use TIFFCheckTile API
    uint32_t width = 0, height = 0;
    uint16_t samples = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    
    // Check a valid tile if possible
    uint32_t tile_width = 0, tile_height = 0;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height)) {
        for (uint32_t y = 0; y < height; y += tile_height) {
            for (uint32_t x = 0; x < width; x += tile_width) {
                int tile_check = TIFFCheckTile(tif, x, y, 0, 0);
                if (tile_check) {
                    // Found a valid tile coordinate
                    uint32_t tile = TIFFComputeTile(tif, x, y, 0, 0);
                    
                    // Step 6: Use TIFFWriteRawTile API
                    tmsize_t tile_size = TIFFTileSize(tif);
                    if (tile_size > 0) {
                        void *tile_buffer = _TIFFmalloc(tile_size);
                        if (tile_buffer) {
                            // Read existing tile data
                            tmsize_t bytes_read = TIFFReadEncodedTile(tif, tile, tile_buffer, tile_size);
                            if (bytes_read > 0) {
                                // Write it back
                                TIFFWriteRawTile(tif, tile, tile_buffer, tile_size);
                            }
                            _TIFFfree(tile_buffer);
                        }
                    }
                    break;
                }
            }
        }
    }
    
    // Step 7: Use TIFFDeferStrileArrayWriting API
    int defer_result = TIFFDeferStrileArrayWriting(tif);
    
    // Step 8: Use TIFFVSetField API (via wrapper)
    // First find a field to set
    const TIFFField *field = TIFFFindField(tif, TIFFTAG_PHOTOMETRIC, TIFF_SHORT);
    if (field) {
        // Step 9: Use TIFFFieldIsAnonymous API
        int is_anonymous = TIFFFieldIsAnonymous(field);
        
        // Set photometric interpretation if not already set
        uint16_t photometric = 0;
        if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
            // Use va_list to pass argument
            uint16_t new_photometric = PHOTOMETRIC_RGB;
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, new_photometric);
        }
    }
    
    // Step 10: Use TIFFYCbCrtoRGB API
    // Check if image is YCbCr
    uint16_t photometric = 0;
    if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) &&
        photometric == PHOTOMETRIC_YCBCR) {
        
        // Allocate and initialize YCbCr to RGB conversion structure
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(sizeof(TIFFYCbCrToRGB));
        if (ycbcr) {
            // Initialize conversion tables (simplified - in real code you'd use TIFFYCbCrToRGBInit)
            // For fuzzing, we'll just set up minimal structure
            ycbcr->clamptab = (TIFFRGBValue *)_TIFFmalloc(256 * sizeof(TIFFRGBValue));
            ycbcr->Cr_r_tab = (int *)_TIFFmalloc(256 * sizeof(int));
            ycbcr->Cb_b_tab = (int *)_TIFFmalloc(256 * sizeof(int));
            ycbcr->Cr_g_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
            ycbcr->Cb_g_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
            ycbcr->Y_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
            
            if (ycbcr->clamptab && ycbcr->Cr_r_tab && ycbcr->Cb_b_tab &&
                ycbcr->Cr_g_tab && ycbcr->Cb_g_tab && ycbcr->Y_tab) {
                
                // Initialize simple linear tables for fuzzing
                for (int i = 0; i < 256; i++) {
                    ycbcr->clamptab[i] = (TIFFRGBValue)i;
                    ycbcr->Cr_r_tab[i] = i;
                    ycbcr->Cb_b_tab[i] = i;
                    ycbcr->Cr_g_tab[i] = i;
                    ycbcr->Cb_g_tab[i] = i;
                    ycbcr->Y_tab[i] = i;
                }
                
                // Convert a sample pixel
                uint32_t r, g, b;
                uint32_t y = 128, cb = 128, cr = 128;
                TIFFYCbCrtoRGB(ycbcr, y, (int32_t)cb, (int32_t)cr, &r, &g, &b);
            }
            
            // Clean up
            if (ycbcr->clamptab) _TIFFfree(ycbcr->clamptab);
            if (ycbcr->Cr_r_tab) _TIFFfree(ycbcr->Cr_r_tab);
            if (ycbcr->Cb_b_tab) _TIFFfree(ycbcr->Cb_b_tab);
            if (ycbcr->Cr_g_tab) _TIFFfree(ycbcr->Cr_g_tab);
            if (ycbcr->Cb_g_tab) _TIFFfree(ycbcr->Cb_g_tab);
            if (ycbcr->Y_tab) _TIFFfree(ycbcr->Y_tab);
            _TIFFfree(ycbcr);
        }
    }
    
    // Step 11: Write directory changes
    TIFFWriteDirectory(tif);
    
    // Step 12: Create output file
    TIFF *out_tif = TIFFOpen(output_filename, "w");
    if (out_tif) {
        // Copy basic parameters from input
        if (width > 0 && height > 0) {
            TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, samples > 0 ? samples : 1);
            TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
            TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(out_tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            // Write dummy scanline
            tmsize_t scanline_size = TIFFScanlineSize(out_tif);
            if (scanline_size > 0) {
                uint8_t *scanline = (uint8_t *)_TIFFmalloc(scanline_size);
                if (scanline) {
                    memset(scanline, 0, scanline_size);
                    for (uint32_t row = 0; row < height && row < 10; row++) {
                        TIFFWriteScanline(out_tif, scanline, row, 0);
                    }
                    _TIFFfree(scanline);
                }
            }
        }
        TIFFClose(out_tif);
    }
    
    // Step 13: Cleanup
    TIFFClose(tif);
    
    // Remove temporary files
    remove(input_filename);
    remove(output_filename);
    
    assert_file_closed(&in_fp);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}