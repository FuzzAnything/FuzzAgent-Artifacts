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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 255, nr_unique_branch: 45
//<Quality> {"density":34,"unique_branches":{"TIFFWriteBufferSetup":[[702,13,702,28,0,0,4,1],[709,13,709,23,0,0,4,0],[282,22,282,62,3,0,4,1]],"TIFFWriteCheck":[[640,50,640,71,0,0,4,0],[282,22,282,62,3,0,4,1]],"LZWDecode":[[380,21,380,37,19,0,4,0]],"_TIFFReserveLargeEnoughWriteBuffer":[[194,9,194,51,0,0,4,1]],"LZWDecodeCompat":[[767,13,767,43,5,0,4,0]],"TIFFCIELabToRGBInit":[[149,17,149,43,0,0,4,0],[149,17,149,43,0,0,4,1],[159,17,159,43,0,0,4,0],[159,17,159,43,0,0,4,1],[169,17,169,43,0,0,4,0],[169,17,169,43,0,0,4,1]],"TIFFSetupStrips":[[587,9,587,37,0,0,4,0]],"TIFFWriteEncodedStrip":[[235,9,235,32,0,0,4,1],[264,9,264,56,0,0,4,1],[272,9,272,35,0,0,4,1],[279,9,279,48,0,0,4,0],[281,13,281,42,0,0,4,1],[289,9,289,47,0,0,4,0],[294,13,294,25,0,0,4,1],[298,13,298,19,0,0,4,0],[298,23,298,74,0,0,4,1],[38,47,38,79,1,0,4,0],[42,7,42,44,4,0,4,1],[43,6,43,53,4,0,4,0],[43,6,43,53,4,0,4,1]],"Fax3Decode1D":[[335,13,335,22,36,0,4,1],[337,13,337,29,36,0,4,0],[340,20,340,35,36,0,4,1],[342,17,342,32,36,0,4,0],[344,21,344,27,36,0,4,1],[346,21,346,39,36,0,4,1],[170,21,170,35,48,0,4,0],[268,13,268,38,61,0,4,1],[117,21,117,31,85,0,4,0]],"TIFFFlush":[[44,9,44,45,0,0,4,1]],"NeXTDecode":[[84,21,84,27,0,0,4,1],[111,21,111,31,0,0,4,1],[111,35,111,53,0,0,4,0]],"_TIFFreallocExt":[[254,14,255,66,0,0,4,0]],"TIFFClientOpenExt":[[421,26,421,37,0,0,4,1],[556,9,556,22,0,0,4,0]],"_TIFFmallocExt":[[183,13,184,53,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFGetVersion","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFStripSize","_TIFFcalloc","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFWriteEncodedStrip","TIFFClose","TIFFOpenOptionsFree","_TIFFfree","TIFFCIELabToRGBInit","LogL10fromY","TIFFReadDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFGetVersion","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFStripSize","_TIFFcalloc","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFWriteEncodedStrip","TIFFClose","TIFFOpenOptionsFree","_TIFFfree","TIFFCIELabToRGBInit","LogL10fromY","TIFFReadDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/**/


extern "C" {
}

// Helper to create FILE* from memory buffer
struct MemFile {
    FILE* fp;
    const uint8_t* data;
    size_t size;
    size_t pos;
    
    static int closeFunc(thandle_t handle) {
        MemFile* mf = static_cast<MemFile*>(handle);
        if (mf->fp) {
            fclose(mf->fp);
        }
        delete mf;
        return 0;
    }
    
    static tmsize_t readFunc(thandle_t handle, void* buf, tmsize_t size) {
        MemFile* mf = static_cast<MemFile*>(handle);
        size_t remaining = mf->size - mf->pos;
        tmsize_t to_read = (size < static_cast<tmsize_t>(remaining)) ? size : static_cast<tmsize_t>(remaining);
        if (to_read > 0) {
            memcpy(buf, mf->data + mf->pos, to_read);
            mf->pos += to_read;
        }
        return to_read;
    }
    
    static tmsize_t writeFunc(thandle_t handle, void* buf, tmsize_t size) {
        MemFile* mf = static_cast<MemFile*>(handle);
        // For writing, we'd need a growable buffer, but for fuzzing we'll just discard
        return size; // Pretend we wrote it
    }
    
    static toff_t seekFunc(thandle_t handle, toff_t offset, int whence) {
        MemFile* mf = static_cast<MemFile*>(handle);
        switch (whence) {
            case SEEK_SET:
                mf->pos = (offset < mf->size) ? offset : mf->size;
                break;
            case SEEK_CUR:
                mf->pos = (mf->pos + offset < mf->size) ? mf->pos + offset : mf->size;
                break;
            case SEEK_END:
                mf->pos = (mf->size + offset < mf->size) ? mf->size + offset : mf->size;
                break;
        }
        return static_cast<toff_t>(mf->pos);
    }
    
    static toff_t sizeFunc(thandle_t handle) {
        MemFile* mf = static_cast<MemFile*>(handle);
        return static_cast<toff_t>(mf->size);
    }
};

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small to be meaningful
    
    // 1. Create memory file for input
    MemFile* memInput = new MemFile{nullptr, data, size, 0};
    
    // 2. Allocate and configure TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        delete memInput;
        return 0;
    }
    
    // Set memory allocation limits for security
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB
    
    // 3. Open TIFF from memory using client functions
    TIFF* tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2,
        reinterpret_cast<thandle_t>(memInput),
        MemFile::readFunc,
        MemFile::writeFunc,
        MemFile::seekFunc,
        MemFile::closeFunc,
        MemFile::sizeFunc,
        nullptr, // Map file proc
        nullptr, // Unmap file proc
        opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        delete memInput;
        return 0;
    }
    
    // 4. Get libtiff version
    const char* version = TIFFGetVersion();
    (void)version; // Use version to avoid unused warning
    
    // 5. Process TIFF file
    do {
        // Get basic information
        uint32_t width = 0, height = 0;
        uint16_t samplesperpixel = 1, bitspersample = 8;
        uint16_t photometric = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
        
        // 6. Use TIFFFieldWithTag and TIFFFieldIsAnonymous
        const TIFFField* field = TIFFFieldWithTag(tif, fuzz_uint32_t_3);
        if (field) {
            // Just check if field exists
            (void)field;
        }
        
        // 7. Try to read strips
        tmsize_t stripSize = TIFFStripSize(tif);
        if (stripSize > 0 && stripSize < 10 * 1024 * 1024) { // Reasonable size limit
            void* buffer = _TIFFcalloc(fuzz_int64_t_4, stripSize);
            if (buffer) {
                uint32_t stripCount = TIFFNumberOfStrips(tif);
                for (uint32_t strip = 0; strip < stripCount && strip < 10; strip++) {
                    tmsize_t read = TIFFReadEncodedStrip(tif, fuzz_uint32_t_5, buffer, stripSize);
                    (void)read; // Result not critical for fuzzing
                    
                    // 8. Try to write to output (simulated)
                    if (strip == 0) {
                        // Create a simple output TIFF in memory
                        // We'll use file-based output as per instructions
                        FILE* out_file = fopen("output_file", "wb");
                        if (out_file) {
                            // Create a simple output TIFF
                            TIFFOpenOptions* outOpts = TIFFOpenOptionsAlloc();
                            if (outOpts) {
                                TIFFOpenOptionsSetMaxCumulatedMemAlloc(outOpts, 50 * 1024 * 1024);
                                
                                TIFF* outTif = TIFFOpenExt("output_file", fuzz_str_6, outOpts);
                                if (outTif) {
                                    // Set basic fields for output
                                    TIFFSetField(outTif, TIFFTAG_IMAGEWIDTH, width > 0 ? width : 1);
                                    TIFFSetField(outTif, TIFFTAG_IMAGELENGTH, height > 0 ? height : 1);
                                    TIFFSetField(outTif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
                                    TIFFSetField(outTif, TIFFTAG_BITSPERSAMPLE, bitspersample);
                                    TIFFSetField(outTif, TIFFTAG_PHOTOMETRIC, photometric);
                                    TIFFSetField(outTif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
                                    TIFFSetField(outTif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                                    TIFFSetField(outTif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
                                    TIFFSetField(outTif, TIFFTAG_ROWSPERSTRIP, 
                                                TIFFDefaultStripSize(outTif, fuzz_uint32_t_7));
                                    
                                    // Write a strip
                                    if (stripSize > 0) {
                                        // Fill buffer with pattern if empty
                                        if (read == 0) {
                                            memset(buffer, 0, stripSize);
                                        }
                                        tmsize_t written = TIFFWriteEncodedStrip(outTif, 0, buffer, 
                                            stripSize < 1024 ? stripSize : 1024);
                                        (void)written;
                                    }
                                    
                                    TIFFClose(outTif);
                                }
                                TIFFOpenOptionsFree(outOpts);
                            }
                            assert_file_closed(&out_file);;
                            remove("output_file");
                        }
                    }
                }
                _TIFFfree(buffer);
            }
        }
        
        // 9. Try color conversion functions
        TIFFCIELabToRGB cielab;
        TIFFDisplay display;
        memset(&display, 0, sizeof(display));
        display.d_mat[0][0] = 1.0f;
        display.d_mat[1][1] = 1.0f;
        display.d_mat[2][2] = 1.0f;
        display.d_YCR = display.d_YCG = display.d_YCB = 1.0f;
        display.d_gammaR = display.d_gammaG = display.d_gammaB = 2.2f;
        
        float refWhite[3] = {1.0f, 1.0f, 1.0f};
        int cielabInit = TIFFCIELabToRGBInit(&cielab, &display, refWhite);
        (void)cielabInit;
        
        // 10. Try LogL conversion
        double yValue = 0.5;
        int logLValue = LogL10fromY(yValue, fuzz_int32_t_8);
        (void)logLValue;
        
    } while (TIFFReadDirectory(tif)); // Process all directories
    
    // 11. Cleanup
    TIFFClose(tif); // This will call our closeFunc
    TIFFOpenOptionsFree(opts);
    
    return 0;
}