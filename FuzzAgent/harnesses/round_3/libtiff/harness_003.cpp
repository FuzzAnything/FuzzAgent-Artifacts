#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <tiffio.h>

namespace harness_003 {
/* Fuzzing harness for libtiff targeting TIFF file opening and RGBA image reading operations */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for TIFF file and configuration
    if (size < 64) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation type to determine file opening method (0: TIFFOpen, 1: TIFFFdOpen)
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume configuration for directory operations
    bool use_subdirectory = fdp.ConsumeBool();
    uint64_t subdir_offset = 0;
    if (use_subdirectory) {
        subdir_offset = fdp.ConsumeIntegral<uint64_t>();
    }
    
    // Consume tile/reading parameters
    bool use_tile_reading = fdp.ConsumeBool();
    uint32_t tile_x = 0, tile_y = 0;
    uint32_t strip_num = 0;
    
    if (use_tile_reading) {
        tile_x = fdp.ConsumeIntegral<uint32_t>();
        tile_y = fdp.ConsumeIntegral<uint32_t>();
    } else {
        strip_num = fdp.ConsumeIntegral<uint32_t>();
    }
    
    // Consume whether to use extended versions
    bool use_extended = fdp.ConsumeBool();
    int stop_on_error = fdp.ConsumeIntegral<int>() % 2;

    // Use remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_rgba_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    
    // Write TIFF data to the temporary file
    if (write(fd, tiff_data.c_str(), tiff_data.size()) != (ssize_t)tiff_data.size()) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    // Reset file position for reading
    lseek(fd, 0, SEEK_SET);
    
    TIFF* tif = nullptr;
    TIFFOpenOptions* opts = nullptr;
    
    // Allocate TIFFOpenOptions if using extended APIs
    if (use_extended) {
        opts = TIFFOpenOptionsAlloc();
        if (opts) {
            // Configure some options from fuzzer input
            tmsize_t max_mem = fdp.ConsumeIntegralInRange<tmsize_t>(1024, 1048576);
            TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_mem);
            TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
        }
    }
    
    // Open TIFF file using the selected method
    if (open_method == 0) {
        // Use TIFFOpen or TIFFOpenExt
        if (use_extended && opts) {
            tif = TIFFOpenExt(filename, "r", opts);
        } else {
            tif = TIFFOpen(filename, "r");
        }
    } else {
        // Use TIFFFdOpen or TIFFFdOpenExt
        if (use_extended && opts) {
            tif = TIFFFdOpenExt(fd, filename, "r", opts);
        } else {
            tif = TIFFFdOpen(fd, filename, "r");
        }
        // For TIFFFdOpen, we don't close fd separately - TIFFClose will handle it
    }
    
    if (!tif) {
        if (opts) TIFFOpenOptionsFree(opts);
        if (open_method == 0) close(fd); // Only close if we used TIFFOpen
        unlink(filename);
        return 0;
    }
    
    // Read the first directory if available
    int dir_read = TIFFReadDirectory(tif);
    
    // If we have a directory, perform additional operations
    if (dir_read) {
        // Optionally navigate to subdirectory
        if (use_subdirectory) {
            TIFFSetSubDirectory(tif, subdir_offset);
        }
        
        // Print directory information (to /dev/null to avoid console spam)
        FILE* null_output = fopen("/dev/null", "w");
        if (null_output) {
            TIFFPrintDirectory(tif, null_output, 0);
            fclose(null_output);
        }
        
        // Try to read RGBA data based on image type
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        if (width > 0 && height > 0) {
            // Allocate buffer for RGBA data (4 bytes per pixel)
            size_t rgba_buffer_size = width * height * 4;
            if (rgba_buffer_size > 0 && rgba_buffer_size < 16777216) { // Limit to 16MB
                uint32_t* rgba_buffer = (uint32_t*)_TIFFmalloc(rgba_buffer_size);
                if (rgba_buffer) {
                    if (use_tile_reading && TIFFIsTiled(tif)) {
                        // Read RGBA tile data
                        if (use_extended) {
                            TIFFReadRGBATileExt(tif, tile_x, tile_y, rgba_buffer, stop_on_error);
                        } else {
                            TIFFReadRGBATile(tif, tile_x, tile_y, rgba_buffer);
                        }
                    } else {
                        // Read RGBA strip data
                        if (use_extended) {
                            TIFFReadRGBAStripExt(tif, strip_num, rgba_buffer, stop_on_error);
                        } else {
                            TIFFReadRGBAStrip(tif, strip_num, rgba_buffer);
                        }
                    }
                    _TIFFfree(rgba_buffer);
                }
            }
        }
        
        // Try to read additional directories if available
        while (TIFFReadDirectory(tif)) {
            // Read a few more directories to exercise multi-page TIFF handling
            static int max_additional_dirs = 3;
            if (--max_additional_dirs <= 0) break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    if (opts) TIFFOpenOptionsFree(opts);
    
    // If we used TIFFOpen (not TIFFFdOpen), close the file descriptor
    if (open_method == 0) {
        close(fd);
    }
    
    unlink(filename);
    return 0;
}

} // namespace harness_003
