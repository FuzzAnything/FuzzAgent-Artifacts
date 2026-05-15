// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

// Variadic wrapper functions for VGetField and VSetField
static int call_TIFFVGetField(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    int result;
    va_start(ap, tag);
    result = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return result;
}

static int call_TIFFVGetFieldDefaulted(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    int result;
    va_start(ap, tag);
    result = TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
    return result;
}

static int call_TIFFVSetField(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    int result;
    va_start(ap, tag);
    result = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return result;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Set up error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy TIFF file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to file
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // If we can't open as read-only, try to create a new file
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            unlink(filename);
            return 0;
        }
        
        // Write some minimal valid TIFF data
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        
        unsigned char pixel = 0;
        TIFFWriteEncodedStrip(tif, 0, &pixel, 1);
        TIFFClose(tif);
        
        // Reopen for reading
        tif = TIFFOpen(filename, "r");
        if (!tif) {
            unlink(filename);
            return 0;
        }
    }
    
    // Use fuzzer data to generate tag values
    uint32_t tag = 0;
    size_t data_offset = 0;
    if (Size >= sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
        data_offset += sizeof(uint32_t);
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFindField with various data types
    if (Size - data_offset > 0) {
        TIFFDataType dtype = (TIFFDataType)(Data[data_offset] % (TIFF_IFD8 + 1));
        const TIFFField* found_field = TIFFFindField(tif, tag, dtype);
        (void)found_field; // Use result to avoid compiler warnings
        data_offset++;
    }
    
    // Test TIFFVGetField with different argument types
    if (field && Size - data_offset > 0) {
        int arg_type = Data[data_offset] % 4;
        
        switch(arg_type) {
            case 0: {
                uint32_t val = 0;
                if (Size - data_offset >= sizeof(uint32_t)) {
                    memcpy(&val, Data + data_offset, sizeof(uint32_t));
                }
                call_TIFFVGetField(tif, tag, &val);
                break;
            }
            case 1: {
                uint16_t val = 0;
                if (Size - data_offset >= sizeof(uint16_t)) {
                    memcpy(&val, Data + data_offset, sizeof(uint16_t));
                }
                call_TIFFVGetField(tif, tag, &val);
                break;
            }
            case 2: {
                float val = 0.0f;
                if (Size - data_offset >= sizeof(float)) {
                    memcpy(&val, Data + data_offset, sizeof(float));
                }
                call_TIFFVGetField(tif, tag, &val);
                break;
            }
            case 3: {
                const char* val = "dummy";
                call_TIFFVGetField(tif, tag, &val);
                break;
            }
        }
        data_offset++;
    }
    
    // Test TIFFVGetFieldDefaulted with different argument types
    if (field && Size - data_offset > 0) {
        int arg_type = Data[data_offset] % 4;
        
        switch(arg_type) {
            case 0: {
                uint32_t val = 0;
                if (Size - data_offset >= sizeof(uint32_t)) {
                    memcpy(&val, Data + data_offset, sizeof(uint32_t));
                }
                call_TIFFVGetFieldDefaulted(tif, tag, &val);
                break;
            }
            case 1: {
                uint16_t val = 0;
                if (Size - data_offset >= sizeof(uint16_t)) {
                    memcpy(&val, Data + data_offset, sizeof(uint16_t));
                }
                call_TIFFVGetFieldDefaulted(tif, tag, &val);
                break;
            }
            case 2: {
                float val = 0.0f;
                if (Size - data_offset >= sizeof(float)) {
                    memcpy(&val, Data + data_offset, sizeof(float));
                }
                call_TIFFVGetFieldDefaulted(tif, tag, &val);
                break;
            }
            case 3: {
                const char* val = "dummy";
                call_TIFFVGetFieldDefaulted(tif, tag, &val);
                break;
            }
        }
        data_offset++;
    }
    
    // Test TIFFGetFieldDefaulted with various argument types
    if (field && Size - data_offset > 0) {
        int arg_type = Data[data_offset] % 4;
        
        // Try different argument types based on fuzzer data
        switch (arg_type) {
            case 0: {
                uint32_t uint32_val = 0;
                TIFFGetFieldDefaulted(tif, tag, &uint32_val);
                break;
            }
            case 1: {
                uint16_t uint16_val = 0;
                TIFFGetFieldDefaulted(tif, tag, &uint16_val);
                break;
            }
            case 2: {
                float float_val = 0.0f;
                TIFFGetFieldDefaulted(tif, tag, &float_val);
                break;
            }
            case 3: {
                const char* string_val = NULL;
                TIFFGetFieldDefaulted(tif, tag, &string_val);
                break;
            }
        }
        data_offset++;
    }
    
    // Test TIFFVSetField if we have write access
    TIFF* tif_write = TIFFOpen(filename, "r+");
    if (tif_write && Size - data_offset > 0) {
        // Try to set some common writable tags
        uint32_t tags_to_try[] = {
            TIFFTAG_IMAGEWIDTH,
            TIFFTAG_IMAGELENGTH,
            TIFFTAG_BITSPERSAMPLE,
            TIFFTAG_SAMPLESPERPIXEL,
            TIFFTAG_PHOTOMETRIC,
            TIFFTAG_PLANARCONFIG,
            TIFFTAG_ROWSPERSTRIP
        };
        
        uint32_t tag_to_set = tags_to_try[Data[data_offset] % (sizeof(tags_to_try)/sizeof(tags_to_try[0]))];
        data_offset++;
        
        if (Size - data_offset > 0) {
            int arg_type = Data[data_offset] % 4;
            
            switch(arg_type) {
                case 0: {
                    uint32_t val = 0;
                    if (Size - data_offset >= sizeof(uint32_t)) {
                        memcpy(&val, Data + data_offset, sizeof(uint32_t));
                    }
                    call_TIFFVSetField(tif_write, tag_to_set, val);
                    break;
                }
                case 1: {
                    uint16_t val = 0;
                    if (Size - data_offset >= sizeof(uint16_t)) {
                        memcpy(&val, Data + data_offset, sizeof(uint16_t));
                    }
                    call_TIFFVSetField(tif_write, tag_to_set, val);
                    break;
                }
                case 2: {
                    float val = 0.0f;
                    if (Size - data_offset >= sizeof(float)) {
                        memcpy(&val, Data + data_offset, sizeof(float));
                    }
                    call_TIFFVSetField(tif_write, tag_to_set, val);
                    break;
                }
                case 3: {
                    const char* val = "dummy";
                    call_TIFFVSetField(tif_write, tag_to_set, val);
                    break;
                }
            }
            data_offset++;
        }
        
        TIFFClose(tif_write);
    }
    
    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    
    unlink(filename);
    return 0;
}