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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"TIFFAdvanceDirectory":[[1941,17,1941,24,0,0,4,0]]},"library_calls":["TIFFClientOpenExt","TIFFNumberOfDirectories","LogL16fromY","TIFFStripSize64","TIFFVStripSize64","_TIFFmalloc","_TIFFrealloc","TIFFWriteBufferSetup","_TIFFfree","_TIFFfree","TIFFCleanup"],"critical_calls":["TIFFClientOpenExt","TIFFNumberOfDirectories","LogL16fromY","TIFFStripSize64","TIFFVStripSize64","_TIFFmalloc","_TIFFrealloc","TIFFWriteBufferSetup","_TIFFfree","TIFFCleanup"],"visited":0}
/*Here's a fuzz driver that uses the specified libtiff APIs to process TIFF data from the input stream:

*/


extern "C" {
}

// Custom memory buffer handler for TIFFClientOpen
struct MemBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// TIFF callback functions
static tmsize_t  id_000007_tiffReadProc(thandle_t handle, void* buf, tmsize_t size) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(handle);
    tmsize_t remaining = buffer->size - buffer->pos;
    tmsize_t to_read = size < remaining ? size : remaining;
    
    if (to_read > 0) {
        memcpy(buf, buffer->data + buffer->pos, to_read);
        buffer->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000007_tiffWriteProc(thandle_t handle, void* buf, tmsize_t size) {
    // We're not writing in this fuzzer, but function must exist
    return 0;
}

static toff_t  id_000007_tiffSeekProc(thandle_t handle, toff_t offset, int whence) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            buffer->pos = offset < buffer->size ? offset : buffer->size;
            break;
        case SEEK_CUR:
            buffer->pos = (buffer->pos + offset) < buffer->size ? 
                         (buffer->pos + offset) : buffer->size;
            break;
        case SEEK_END:
            buffer->pos = buffer->size;
            break;
    }
    return buffer->pos;
}

static int  id_000007_tiffCloseProc(thandle_t handle) {
    // Nothing to close for memory buffer
    return 0;
}

static toff_t  id_000007_tiffSizeProc(thandle_t handle) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(handle);
    return buffer->size;
}

static int  id_000007_tiffMapFileProc(thandle_t handle, void** base, toff_t* size) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(handle);
    *base = const_cast<void*>(reinterpret_cast<const void*>(buffer->data));
    *size = buffer->size;
    return 0;
}

static void  id_000007_tiffUnmapFileProc(thandle_t handle, void* base, toff_t size) {
    // Nothing to unmap
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data for meaningful fuzzing
    }

    // Initialize memory buffer for TIFF input
    MemBuffer memBuffer = {data, size, 0};
    
    // Open TIFF from memory using TIFFClientOpen (via TIFFClientOpenExt with NULL options)
    TIFF* tif = TIFFClientOpenExt(
        fuzz_str_1, 
        fuzz_str_2, 
        reinterpret_cast<thandle_t>(&memBuffer),
         id_000007_tiffReadProc,
         id_000007_tiffWriteProc,
         id_000007_tiffSeekProc,
         id_000007_tiffCloseProc,
         id_000007_tiffSizeProc,
         id_000007_tiffMapFileProc,
         id_000007_tiffUnmapFileProc,
        nullptr
    );
    
    if (!tif) {
        return 0;
    }

    // Get number of directories (TIFFNumberOfDirectories)
    tdir_t numDirs = TIFFNumberOfDirectories(tif);
    
    // Call LogL16fromY with some sample values
    // Using values derived from input data for fuzzing
    double yValue = static_cast<double>(data[0]) / 255.0;
    int colorTemp = static_cast<int>(data[1] % 1000) + 1000; // Random temp between 1000-1999
    int logL16 = LogL16fromY(yValue, colorTemp);
    
    // Get strip size information
    uint64_t stripSize = TIFFStripSize64(tif);
    
    // Calculate vertical strip size for various row counts
    // Use modulo to get reasonable nrows values
    uint32_t nrows = static_cast<uint32_t>(data[2] % 100) + 1;
    uint64_t vStripSize = TIFFVStripSize64(tif, nrows);
    
    // Set up write buffer using TIFFWriteBufferSetup
    // Allocate buffer using _TIFFrealloc
    tmsize_t bufferSize = static_cast<tmsize_t>(stripSize > 0 ? stripSize : 4096);
    void* writeBuffer = _TIFFmalloc(bufferSize);
    if (writeBuffer) {
        // Try to reallocate with different size
        tmsize_t newBufferSize = bufferSize * 2;
        void* newBuffer = _TIFFrealloc(writeBuffer, newBufferSize);
        
        if (newBuffer) {
            writeBuffer = newBuffer;
            bufferSize = newBufferSize;
            
            // Set up write buffer in TIFF
            int setupResult = TIFFWriteBufferSetup(tif, writeBuffer, bufferSize);
            
            // Clean up write buffer
            _TIFFfree(writeBuffer);
        } else {
            _TIFFfree(writeBuffer);
        }
    }

    // Clean up
    TIFFCleanup(tif);
    
    return 0;
}