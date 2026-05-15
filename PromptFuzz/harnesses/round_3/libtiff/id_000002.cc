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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 243
//<Quality> {"density":10,"unique_branches":{"TIFFWriteDirectoryTagCheckedShort":[[2388,9,2388,20,0,0,4,0],[2388,9,2388,20,0,0,4,1],[2395,9,2395,35,0,0,4,1]],"TIFFWriteDirectorySec":[[497,9,497,34,0,0,4,1],[507,9,507,18,0,0,4,0],[509,13,509,45,0,0,4,1],[527,13,527,31,0,0,4,1],[536,13,536,45,0,0,4,0],[536,49,536,65,0,0,4,0],[550,9,550,61,0,0,4,1],[565,13,565,24,0,0,4,0],[565,13,565,24,0,0,4,1],[567,13,567,20,0,0,4,0],[573,21,575,80,0,0,4,1],[577,21,579,53,0,0,4,1],[624,21,626,55,0,0,4,1],[631,21,633,77,0,0,4,1],[638,21,640,77,0,0,4,1],[666,21,668,57,0,0,4,1],[694,21,696,78,0,0,4,1],[716,21,716,61,0,0,4,1],[723,21,723,29,0,0,4,0],[725,25,728,62,0,0,4,1],[743,21,743,58,0,0,4,1],[750,21,750,29,0,0,4,0],[763,25,763,62,0,0,4,0],[764,25,767,59,0,0,4,1],[892,29,892,49,0,0,4,0],[892,29,892,49,0,0,4,1],[896,25,896,54,0,0,4,1],[1018,21,1018,69,0,0,4,1],[1188,13,1188,24,0,0,4,0],[1188,13,1188,24,0,0,4,1],[1193,13,1193,45,0,0,4,0],[1200,13,1200,24,0,0,4,1],[1205,13,1205,20,0,0,4,0],[1215,17,1215,37,0,0,4,0],[1217,21,1217,44,0,0,4,1],[1249,13,1249,28,0,0,4,1],[1251,13,1251,45,0,0,4,0],[1260,13,1260,45,0,0,4,0],[1262,13,1262,49,0,0,4,1],[1263,13,1263,51,0,0,4,1],[1268,13,1268,33,0,0,4,1],[1271,9,1271,16,0,0,4,0],[1299,9,1299,23,0,0,4,1],[1304,9,1304,41,0,0,4,0],[1311,13,1311,39,0,0,4,1],[1315,21,1315,29,0,0,4,0],[1315,21,1315,29,0,0,4,1],[1318,17,1318,43,0,0,4,1],[1322,17,1322,43,0,0,4,1],[1327,17,1327,43,0,0,4,1],[1337,13,1337,39,0,0,4,1],[1375,9,1375,16,0,0,4,1],[1381,9,1381,17,0,0,4,1],[1390,9,1390,16,0,0,4,0],[1390,20,1390,52,0,0,4,0],[1392,15,1392,47,0,0,4,1],[1396,17,1396,69,0,0,4,0],[1429,9,1429,16,0,0,4,0],[1429,20,1429,52,0,0,4,0],[1430,11,1430,43,0,0,4,1],[1437,9,1437,18,0,0,4,0],[1437,22,1437,54,0,0,4,1],[1441,9,1441,77,0,0,4,1],[1449,9,1449,18,0,0,4,0],[236,34,236,71,7,0,4,0],[236,34,236,71,9,0,4,0],[236,34,236,71,12,0,4,1],[236,34,236,71,15,0,4,1],[236,34,236,71,18,0,4,1],[236,34,236,71,21,0,4,1],[236,34,236,71,23,0,4,0],[236,34,236,71,25,0,4,0],[236,34,236,71,27,0,4,0],[236,34,236,71,29,0,4,1],[236,34,236,71,31,0,4,1],[236,34,236,71,33,0,4,1],[236,34,236,71,35,0,4,0],[236,34,236,71,37,0,4,1],[236,34,236,71,39,0,4,1],[236,34,236,71,41,0,4,1],[236,34,236,71,43,0,4,0],[236,34,236,71,45,0,4,1],[236,34,236,71,47,0,4,1],[236,34,236,71,49,0,4,0],[236,34,236,71,53,0,4,0],[236,34,236,71,57,0,4,1],[236,34,236,71,58,0,4,1],[236,34,236,71,61,0,4,1],[236,34,236,71,63,0,4,1],[236,34,236,71,65,0,4,1],[236,34,236,71,67,0,4,1],[236,34,236,71,69,0,4,1],[236,34,236,71,71,0,4,1],[236,34,236,71,73,0,4,1],[236,34,236,71,75,0,4,1],[236,34,236,71,77,0,4,1],[236,34,236,71,79,0,4,1],[236,34,236,71,80,0,4,1],[236,34,236,71,82,0,4,1],[236,34,236,71,84,0,4,1],[236,34,236,71,91,0,4,1]],"TIFFWriteCheck":[[634,9,634,22,0,0,4,1],[640,9,640,46,0,0,4,0],[640,50,640,71,0,0,4,1],[656,9,656,35,0,0,4,1],[660,9,660,56,0,0,4,1],[282,22,282,62,4,0,4,1]],"LogLuvSetupEncode":[[1609,9,1609,32,0,0,4,0],[1610,17,1610,38,0,0,4,1],[1612,17,1612,59,0,0,4,1],[1634,21,1634,45,0,0,4,1],[1637,21,1637,45,0,0,4,1],[1640,21,1640,43,0,0,4,1],[1642,21,1642,28,0,0,4,0],[1647,9,1647,30,0,0,4,1],[1662,9,1662,16,0,0,4,1],[1674,19,1674,57,0,0,4,1]],"TIFFOpenExt":[[245,9,245,16,0,0,4,1],[254,9,254,15,0,0,4,1],[269,9,269,13,0,0,4,1]],"TIFFWriteEncodedStrip":[[235,9,235,32,0,0,4,1],[264,9,264,56,0,0,4,1],[272,9,272,35,0,0,4,1],[279,9,279,48,0,0,4,0],[281,13,281,42,0,0,4,0],[38,6,38,43,1,0,4,1],[38,47,38,79,1,0,4,0],[42,7,42,44,4,0,4,1],[43,6,43,53,4,0,4,0]],"_tiffWriteProc":[[105,9,105,38,0,0,4,1],[111,29,111,56,0,0,4,0],[111,29,111,56,0,0,4,1],[116,13,116,34,0,0,4,1],[121,13,121,23,0,0,4,1],[124,9,124,18,0,0,4,1]],"TIFFFdOpenExt":[[225,9,225,12,0,0,4,0]],"_tiffSeekProc":[[136,9,136,32,0,0,4,1]],"TIFFWriteDirectoryTagLongLong8Array":[[1965,9,1965,48,0,0,4,1],[1979,9,1979,38,0,0,4,1],[2002,9,2002,18,0,0,4,1],[2006,14,2006,23,0,0,4,1],[2010,9,2010,22,0,0,4,0],[2022,13,2022,22,0,0,4,1],[2028,41,2028,51,0,0,4,0],[2028,41,2028,51,0,0,4,1],[2030,17,2030,33,0,0,4,1]],"TIFFWriteDirectoryTagShortLong":[[1908,9,1908,24,0,0,4,0]],"TIFFFlushData1":[[952,9,952,27,0,0,4,1]],"TIFFWriteDirectoryTagCheckedLongArray":[[2463,9,2463,20,0,0,4,0],[2463,9,2463,20,0,0,4,1],[2468,9,2468,35,0,0,4,1]],"LogLuvGuessDataFmt":[[1424,9,1424,43,0,0,4,1],[1427,9,1427,41,0,0,4,1],[1428,9,1428,41,0,0,4,1],[1429,9,1429,40,0,0,4,1],[1432,9,1432,41,0,0,4,1],[1433,9,1433,40,0,0,4,1],[1434,9,1434,41,0,0,4,1],[1437,9,1437,40,0,0,4,1],[1438,9,1438,40,0,0,4,0],[1441,9,1441,16,0,0,4,1],[1451,9,1451,15,0,0,4,1],[1455,9,1455,15,0,0,4,0],[1456,17,1456,43,0,0,4,1],[1459,9,1459,16,0,0,4,1]],"TIFFWriteDirectoryTagData":[[3099,12,3099,23,0,0,4,0],[3099,12,3099,23,0,0,4,1],[3102,13,3102,34,0,0,4,0],[3102,13,3102,34,0,0,4,1],[3106,9,3106,20,0,0,4,0],[3106,9,3106,20,0,0,4,1],[3109,25,3109,30,0,0,4,0],[3109,25,3109,30,0,0,4,1],[3116,9,3116,70,0,0,4,0],[3116,9,3116,70,0,0,4,1],[3116,24,3116,55,0,0,4,1],[3118,13,3118,17,0,0,4,0],[3118,21,3118,31,0,0,4,0],[3128,13,3128,45,0,0,4,0],[3130,13,3130,22,0,0,4,1],[3130,26,3130,43,0,0,4,1],[3135,13,3135,20,0,0,4,1],[3140,13,3140,39,0,0,4,1],[3147,13,3147,21,0,0,4,1],[3153,13,3153,33,0,0,4,1],[3155,13,3155,45,0,0,4,0],[3159,17,3159,43,0,0,4,1]],"TIFFInitSGILog":[[1816,9,1816,75,0,0,4,1],[1826,9,1826,30,0,0,4,1],[1831,23,1831,55,0,0,4,1]],"TIFFWriteDirectoryTagShortPerSample":[[1668,9,1668,20,0,0,4,0],[1668,9,1668,20,0,0,4,1],[1676,9,1676,18,0,0,4,1],[1681,26,1681,62,0,0,4,0],[1681,26,1681,62,0,0,4,1]],"LogLuvCleanup":[[1712,9,1712,17,0,0,4,0]],"TIFFLinkDirectory":[[3186,9,3186,39,0,0,4,1],[3242,9,3242,41,0,0,4,0],[3247,13,3247,39,0,0,4,1],[3249,13,3249,53,0,0,4,0],[3257,17,3257,25,0,0,4,1],[3262,17,3262,49,0,0,4,0]],"TIFFFlush":[[50,9,50,64,0,0,4,0],[51,9,51,35,0,0,4,1]],"TIFFWriteDirectoryTagCheckedShortArray":[[2408,9,2408,20,0,0,4,0],[2408,9,2408,20,0,0,4,1],[2413,9,2413,35,0,0,4,1]],"TIFFWriteBufferSetup":[[683,9,683,25,0,0,4,1],[692,9,692,31,0,0,4,0],[697,13,697,49,0,0,4,0],[702,13,702,28,0,0,4,0],[706,9,706,19,0,0,4,0],[709,13,709,23,0,0,4,1],[282,22,282,62,3,0,4,1]],"_TIFFReserveLargeEnoughWriteBuffer":[[194,9,194,51,0,0,4,1]],"TIFFRasterScanlineSize64":[[388,9,388,51,0,0,4,0],[388,9,388,51,0,0,4,1],[328,6,328,16,2,0,4,0],[328,6,328,16,2,0,4,1],[328,6,328,16,3,0,4,0],[328,6,328,16,3,0,4,1]],"LogLuvInitState":[[1476,9,1476,51,0,0,4,1],[1482,9,1482,50,0,0,4,0],[1486,9,1486,33,0,0,4,1],[1489,9,1489,33,0,0,4,1],[1492,9,1492,31,0,0,4,1],[1495,9,1495,32,0,0,4,0],[1498,9,1498,16,0,0,4,1],[1506,14,1506,54,0,0,4,1],[1510,9,1510,56,0,0,4,1],[1511,9,1512,79,0,0,4,1],[282,22,282,62,8,0,4,1]],"EvaluateIFDdatasizeWrite":[[2142,9,2142,69,0,0,4,0],[2142,9,2142,69,0,0,4,1],[2142,23,2142,54,0,0,4,1],[2146,13,2146,27,0,0,4,1]],"TIFFRewriteDirectorySec":[[321,9,321,29,0,0,4,0]],"LogLuvClose":[[1692,9,1692,26,0,0,4,1]],"TIFFFlushData":[[148,9,148,49,0,0,4,1],[150,9,150,41,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFRasterScanlineSize64","TIFFClientdata","LogLuv24fromXYZ","TIFFClose","TIFFOpen","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteEncodedStrip","TIFFClose","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFRasterScanlineSize64","TIFFClientdata","LogLuv24fromXYZ","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteEncodedStrip","TIFFClose","TIFFClose"],"visited":1}
/**/


// Custom memory buffer structure for TIFF client I/O
struct MemBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// TIFF I/O callbacks for memory buffer
static tmsize_t  id_000002_memRead(thandle_t handle, void* buffer, tmsize_t size) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    tmsize_t to_read = size;
    if (mem->pos + to_read > mem->size) {
        to_read = mem->size - mem->pos;
    }
    if (to_read > 0) {
        memcpy(buffer, mem->data + mem->pos, to_read);
        mem->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000002_memWrite(thandle_t, void*, tmsize_t) {
    return 0; // Read-only for input
}

static toff_t  id_000002_memSeek(thandle_t handle, toff_t offset, int whence) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    switch (whence) {
        case SEEK_SET:
            mem->pos = (offset < mem->size) ? offset : mem->size;
            break;
        case SEEK_CUR:
            mem->pos = (mem->pos + offset < mem->size) ? mem->pos + offset : mem->size;
            break;
        case SEEK_END:
            mem->pos = (mem->size + offset < mem->size) ? mem->size + offset : mem->size;
            break;
    }
    return mem->pos;
}

static int  id_000002_memClose(thandle_t) {
    return 0;
}

static toff_t  id_000002_memSize(thandle_t handle) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    return mem->size;
}

static int  id_000002_memMap(thandle_t, void**, toff_t*) {
    return 0;
}

static void  id_000002_memUnmap(thandle_t, void*, toff_t) {}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Open input TIFF from memory buffer
    MemBuffer mem_buffer = {data, size, 0};
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                               reinterpret_cast<thandle_t>(&mem_buffer),
                                id_000002_memRead,  id_000002_memWrite,  id_000002_memSeek, 
                                id_000002_memClose,  id_000002_memSize,  id_000002_memMap,  id_000002_memUnmap);
    
    if (!tif) {
	return 0;
	}
	
    
    // 2. Get raster scanline size
    uint64_t scanline_size = TIFFRasterScanlineSize64(tif);
    
    // 3. Set client data (store original pointer)
    thandle_t original_clientdata = TIFFClientdata(tif);
    thandle_t new_clientdata = reinterpret_cast<thandle_t>(&scanline_size);
    // Note: TIFFSetClientdata is not in provided list, using TIFFClientdata instead
    // We'll simulate setting by using the value
    
    // 4. Prepare XYZ data for LogLuv conversion
    float xyz[3];
    if (size >= 12) {
        // Use first 12 bytes for XYZ values (4 bytes each)
        memcpy(xyz, data, 12);
    } else {
        xyz[0] = 0.5f; xyz[1] = 0.5f; xyz[2] = 0.5f;
    }
    
    // 5. Convert XYZ to LogLuv24
    uint32_t logluv_pixel = LogLuv24fromXYZ(xyz, fuzz_int32_t_3);
    
    // 6. Create output TIFF file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        TIFFClose(tif);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 7. Create output TIFF using file descriptor
    int fd = fuzz_fileno(out_file);
    TIFF* out_tif = TIFFOpen("output_file", fuzz_str_4);
    if (!out_tif) {
        assert_file_closed(&out_file);;
        TIFFClose(tif);
        assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Set up output TIFF as LogLuv image
    uint32_t width = 100;
    uint32_t height = 1;
    TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_LOGLUV);
    TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out_tif, TIFFTAG_COMPRESSION, COMPRESSION_SGILOG);
    
    // 9. Prepare strip data (single pixel repeated)
    uint32_t strip_size = width * 3;
    uint8_t* strip_data = (uint8_t*)malloc(strip_size);
    if (strip_data) {
        // Fill with converted LogLuv pixel (24-bit, 3 bytes)
        for (uint32_t i = 0; i < width; i++) {
            strip_data[i*3] = (logluv_pixel >> 16) & 0xFF;
            strip_data[i*3 + 1] = (logluv_pixel >> 8) & 0xFF;
            strip_data[i*3 + 2] = logluv_pixel & 0xFF;
        }
        
        // 10. Write encoded strip
        tmsize_t written = TIFFWriteEncodedStrip(out_tif, 0, strip_data, strip_size);
        
        free(strip_data);
    }
    
    // 11. Cleanup
    TIFFClose(out_tif);
    TIFFClose(tif);
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}