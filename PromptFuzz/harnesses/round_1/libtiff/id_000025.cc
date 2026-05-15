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
//<ID> 1572
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 22
//<Quality> {"density":22,"unique_branches":{"JPEGResetUpsampled":[[282,22,282,62,7,0,4,1]],"JPEGDefaultStripSize":[[2727,9,2727,31,0,0,4,0],[2727,9,2727,31,0,0,4,1],[314,6,314,54,3,0,4,0]],"JPEGInitializeLibJPEG":[[2768,9,2768,30,0,0,4,0],[2770,13,2770,24,0,0,4,1],[2772,18,2772,28,0,0,4,0],[2772,32,2772,63,0,0,4,1]],"TIFFReadScanline":[[475,9,475,31,0,0,4,0],[477,9,477,46,0,0,4,0],[488,13,488,14,0,0,4,0],[488,13,488,14,0,0,4,1],[497,13,497,18,0,0,4,0]],"TIFFSeek":[[336,9,336,34,0,0,4,1],[343,9,343,53,0,0,4,1],[397,9,397,35,0,0,4,0],[397,9,397,35,0,0,4,1],[400,13,400,24,0,0,4,0],[402,17,402,43,0,0,4,0],[402,17,402,43,0,0,4,1],[432,9,432,27,0,0,4,1],[455,9,455,28,0,0,4,1]]},"library_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFReadDirectory","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFSetField","TIFFDefaultStripSize","TIFFNumberOfStrips","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFIsTiled","TIFFComputeTile","TIFFReadScanline","TIFFWriteEncodedStrip","_TIFFfree","TIFFGetField","TIFFCIELabToRGBInit","LogL10fromY","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFReadDirectory","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFSetField","TIFFDefaultStripSize","TIFFNumberOfStrips","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFIsTiled","TIFFComputeTile","TIFFReadScanline","TIFFWriteEncodedStrip","_TIFFfree","TIFFGetField","TIFFCIELabToRGBInit","LogL10fromY","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/*Looking at the provided APIs, I'll create a fuzz driver that reads a TIFF file from memory, processes its strips, and performs color conversion. This will use most of the requested APIs:

*/


// Custom memory buffer structure for TIFF client I/O
struct MemBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// TIFF I/O callback functions
static tmsize_t  id_000025_memRead(thandle_t handle, void* buffer, tmsize_t size) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    tmsize_t available = static_cast<tmsize_t>(mem->size - mem->pos);
    tmsize_t to_read = (size < available) ? size : available;
    
    if (to_read > 0) {
        memcpy(buffer, mem->data + mem->pos, to_read);
        mem->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000025_memWrite(thandle_t handle, void* buffer, tmsize_t size) {
    // Read-only mode
    return 0;
}

static toff_t  id_000025_memSeek(thandle_t handle, toff_t offset, int whence) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            mem->pos = (offset < mem->size) ? offset : mem->size;
            break;
        case SEEK_CUR:
            mem->pos = ((mem->pos + offset) < mem->size) ? (mem->pos + offset) : mem->size;
            break;
        case SEEK_END:
            mem->pos = (offset < mem->size) ? (mem->size - offset) : 0;
            break;
    }
    return static_cast<toff_t>(mem->pos);
}

static int  id_000025_memClose(thandle_t handle) {
    // Memory is managed elsewhere, just return success
    return 0;
}

static toff_t  id_000025_memSize(thandle_t handle) {
    MemBuffer* mem = reinterpret_cast<MemBuffer*>(handle);
    return static_cast<toff_t>(mem->size);
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 100) {
        return 0;
    }
    
    // Get libtiff version (requested API)
    const char* version = TIFFGetVersion();
    (void)version; // Mark as used
    
    // Create memory buffer for TIFF client I/O
    MemBuffer memBuffer = {data, size, 0};
    
    // Allocate and configure TIFF open options
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Set memory allocation limits
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
    
    // Open TIFF from memory using client API
    TIFF* tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2, 
                                  reinterpret_cast<thandle_t>(&memBuffer),
                                   id_000025_memRead,  id_000025_memWrite,  id_000025_memSeek, 
                                   id_000025_memClose,  id_000025_memSize, 
                                  nullptr, nullptr, opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    
    // Try to read the first directory
    if (TIFFReadDirectory(tif)) {
        // Get basic image information
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        // Get field info for a specific tag
        const TIFFField* widthField = TIFFFieldWithTag(tif, fuzz_uint32_t_3);
        (void)widthField; // Mark as used
        
        // Set some fields (in read mode, this will fail but we call the API)
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Try using TIFFVSetField with variable arguments
        // Note: We need to construct a proper va_list
        if (width > 0 && height > 0) {
            // Calculate strip/tile parameters
            uint32_t stripSize = TIFFDefaultStripSize(tif, fuzz_uint32_t_4);
            uint32_t numStrips = TIFFNumberOfStrips(tif);
            
            // Get strip size using TIFFStripSize
            tmsize_t calculatedStripSize = TIFFStripSize(tif);
            
            // Read strips if we have enough memory
            if (numStrips > 0 && calculatedStripSize > 0 && 
                calculatedStripSize < 10 * 1024 * 1024) { // Limit to 10MB per strip
                
                // Allocate buffer for strip reading using _TIFFcalloc
                void* stripBuffer = _TIFFcalloc(fuzz_int64_t_5, calculatedStripSize);
                if (stripBuffer) {
                    // Read each strip
                    for (uint32_t strip = 0; strip < numStrips; strip++) {
                        tmsize_t readSize = TIFFReadEncodedStrip(tif, fuzz_uint32_t_6, 
                                                                stripBuffer, 
                                                                calculatedStripSize);
                        (void)readSize; // Mark as used
                        
                        // Try to compute tile position if image is tiled
                        if (TIFFIsTiled(tif)) {
                            uint32_t tile = TIFFComputeTile(tif, strip % width, 
                                                           strip / width, fuzz_uint32_t_7, fuzz_uint16_t_8);
                            (void)tile; // Mark as used
                        }
                        
                        // Also try reading by scanline for first few rows
                        if (strip == 0) {
                            uint32_t row = 0;
                            uint16_t sample = 0;
                            int scanlineResult = TIFFReadScanline(tif, stripBuffer, 
                                                                  fuzz_uint32_t_9, fuzz_uint16_t_10);
                            (void)scanlineResult; // Mark as used
                        }
                    }
                    
                    // Try writing a strip (this will fail in read mode but we call the API)
                    tmsize_t writeSize = TIFFWriteEncodedStrip(tif, fuzz_uint32_t_11, stripBuffer, 
                                                              calculatedStripSize);
                    (void)writeSize; // Mark as used
                    
                    _TIFFfree(stripBuffer);
                }
            }
        }
        
        // Try color conversion if appropriate
        uint16_t photometric = 0;
        if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
            if (photometric == PHOTOMETRIC_CIELAB) {
                // Initialize CIELab to RGB conversion
                TIFFDisplay display;
                memset(&display, 0, sizeof(display));
                display.d_gammaR = display.d_gammaG = display.d_gammaB = 2.2;
                
                TIFFCIELabToRGB cielabToRgb;
                float refWhite[3] = {1.0f, 1.0f, 1.0f};
                
                int initResult = TIFFCIELabToRGBInit(&cielabToRgb, &display, refWhite);
                (void)initResult; // Mark as used
                
                // Test LogL10 conversion
                double yValue = 0.5;
                int logl10Result = LogL10fromY(yValue, fuzz_int32_t_12);
                (void)logl10Result; // Mark as used
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    return 0;
}