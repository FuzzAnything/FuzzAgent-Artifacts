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
//<ID> 544
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"TIFFSetupStrips":[[576,9,576,53,0,0,4,0],[579,9,579,37,0,0,4,0],[583,9,583,40,0,0,4,0],[587,9,587,37,0,0,4,0],[553,30,553,64,3,0,4,1],[236,34,236,71,10,0,4,0]],"TIFFReadScanline":[[475,9,475,31,0,0,4,0]],"TIFFGetClientInfo":[[69,12,69,26,0,0,4,1],[72,9,72,23,0,0,4,1]]},"library_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFFileName","TIFFSetupStrips","TIFFGetClientInfo","TIFFCIELabToRGBInit","TIFFGetField","TIFFGetField","TIFFScanlineSize","TIFFReadScanline","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFFileName","TIFFSetupStrips","TIFFGetClientInfo","TIFFCIELabToRGBInit","TIFFGetField","TIFFGetField","TIFFScanlineSize","TIFFReadScanline","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


// Custom memory management for fuzzing
static int  id_000015_customClose(thandle_t fd) {
    return 0; // No-op for fuzzing
}

static tmsize_t  id_000015_customRead(thandle_t fd, void* buf, tmsize_t size) {
    FILE* fp = reinterpret_cast<FILE*>(fd);
    return fread(buf, 1, size, fp);
}

static tmsize_t  id_000015_customWrite(thandle_t fd, void* buf, tmsize_t size) {
    FILE* fp = reinterpret_cast<FILE*>(fd);
    return fwrite(buf, 1, size, fp);
}

static toff_t  id_000015_customSeek(thandle_t fd, toff_t off, int whence) {
    FILE* fp = reinterpret_cast<FILE*>(fd);
    if (fseek(fp, off, whence) == 0) {
        return ftell(fp);
    }
    return -1;
}

static toff_t  id_000015_customSize(thandle_t fd) {
    FILE* fp = reinterpret_cast<FILE*>(fd);
    long current = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, current, SEEK_SET);
    return size;
}

static int  id_000015_customMapFile(thandle_t fd, void** base, toff_t* size) {
    return 0; // Not implemented for fuzzing
}

static void  id_000015_customUnmapFile(thandle_t fd, void* base, toff_t size) {
    // No-op for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0; // Need minimum data for meaningful processing
    }

    // 1. Write input data to a temporary file
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 2. Get libtiff version (API requirement)
    const char* version = TIFFGetVersion();
    (void)version; // Use version info

    // 3. Open the input file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Create TIFF structure using client API
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }

    TIFF* tif = TIFFClientOpenExt(
        "input_file",
        fuzz_str_1,
        reinterpret_cast<thandle_t>(in_file),
         id_000015_customRead,
         id_000015_customWrite,
         id_000015_customSeek,
         id_000015_customClose,
         id_000015_customSize,
         id_000015_customMapFile,
         id_000015_customUnmapFile,
        opts
    );

    if (!tif) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 5. Get file name (API requirement)
    const char* filename = TIFFFileName(tif);
    (void)filename; // Use filename info

    // 6. Setup strips (API requirement)
    int stripSetupResult = TIFFSetupStrips(tif);
    (void)stripSetupResult; // Use result

    // 7. Get client info (API requirement)
    // Use a known tag name for demonstration
    void* clientInfo = TIFFGetClientInfo(tif, fuzz_str_2);
    (void)clientInfo; // Use client info

    // 8. Prepare for CIELab to RGB conversion (API requirement)
    TIFFCIELabToRGB cielabToRgb;
    TIFFDisplay display;
    float refWhite[3] = {1.0f, 1.0f, 1.0f}; // Default white point

    // Initialize display structure with some values
    memset(&display, 0, sizeof(display));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            display.d_mat[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    display.d_YCR = 0.299f;
    display.d_YCG = 0.587f;
    display.d_YCB = 0.114f;
    display.d_Vrwr = 255;
    display.d_Vrwg = 255;
    display.d_Vrwb = 255;
    display.d_Y0R = 0.0f;
    display.d_Y0G = 0.0f;
    display.d_Y0B = 0.0f;
    display.d_gammaR = 2.2f;
    display.d_gammaG = 2.2f;
    display.d_gammaB = 2.2f;

    // Initialize CIELab to RGB converter (API requirement)
    int cielabInitResult = TIFFCIELabToRGBInit(&cielabToRgb, &display, refWhite);
    (void)cielabInitResult; // Use result

    // 9. Process TIFF data based on the input
    // Read basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    // Try to read a scanline if possible
    if (width > 0 && height > 0) {
        tmsize_t scanlineSize = TIFFScanlineSize(tif);
        if (scanlineSize > 0 && scanlineSize < 1000000) { // Reasonable size limit
            uint8_t* scanline = (uint8_t*)malloc(scanlineSize);
            if (scanline) {
                // Try to read first scanline
                TIFFReadScanline(tif, scanline, fuzz_uint32_t_3, fuzz_uint16_t_4);
                free(scanline);
            }
        }
    }

    // 10. Cleanup
    TIFFClose(tif);
    assert_file_closed(&in_file);;
    TIFFOpenOptionsFree(opts);

    // Remove temporary files
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}