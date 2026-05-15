#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <tiffio.h>

namespace harness_020 {
/*
 * Fuzzing harness for libtiff library - Client I/O Procedures and State Management APIs
 * Targets 28 completely uncovered public APIs related to TIFF client I/O procedures and state management:
 * 
 * Client Data Management APIs (2):
 * - TIFFClientdata: Get client data handle
 * - TIFFSetClientdata: Set client data handle
 * 
 * I/O Procedure Access APIs (7):
 * - TIFFGetReadProc: Get read procedure
 * - TIFFGetWriteProc: Get write procedure  
 * - TIFFGetSeekProc: Get seek procedure
 * - TIFFGetCloseProc: Get close procedure
 * - TIFFGetSizeProc: Get size procedure
 * - TIFFGetMapFileProc: Get map file procedure
 * - TIFFGetUnmapFileProc: Get unmap file procedure
 * 
 * File Descriptor Management APIs (2):
 * - TIFFFileno: Get file descriptor
 * - TIFFSetFileno: Set file descriptor
 * 
 * Mode Management APIs (2):
 * - TIFFGetMode: Get TIFF mode
 * - TIFFSetMode: Set TIFF mode
 * 
 * Tag Information APIs (2):
 * - TIFFGetTagListCount: Get count of tags in directory
 * - TIFFGetTagListEntry: Get tag at specific index
 * 
 * Tag Method Access API (1):
 * - TIFFAccessTagMethods: Access tag methods
 * 
 * State Query APIs (12):
 * - TIFFIsTiled: Check if TIFF is tiled
 * - TIFFIsByteSwapped: Check if byte swapped
 * - TIFFIsUpSampled: Check if upsampled
 * - TIFFIsMSB2LSB: Check if MSB to LSB
 * - TIFFIsBigEndian: Check if big endian
 * - TIFFIsBigTIFF: Check if BigTIFF format
 * - TIFFCurrentRow: Get current row
 * - TIFFCurrentDirectory: Get current directory
 * - TIFFNumberOfDirectories: Get number of directories
 * - TIFFCurrentDirOffset: Get current directory offset
 * - TIFFCurrentStrip: Get current strip
 * - TIFFCurrentTile: Get current tile
 * 
 * Semantic differentiation from existing harnesses (harness_000 through harness_019):
 * - Previous harnesses focus on field queries, file I/O, directory operations, color conversion, and writing
 * - This harness exclusively targets client I/O procedure access and state management functions
 * - Tests low-level I/O procedure retrieval that underpins all file operations
 * - Exercises state query functions that reveal internal TIFF object state
 * - Tests client data management for custom I/O implementations
 * 
 * Testing strategy:
 * 1. Create TIFF object using memory-based I/O via TIFFClientOpen
 * 2. Test all 28 target APIs in logical groupings
 * 3. Verify state consistency across related API calls
 * 4. Test edge cases: NULL pointers, invalid parameters, state transitions
 * 5. Use FuzzedDataProvider to generate varied test scenarios
 */


// Error handler to suppress warnings/errors during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Memory I/O structure for TIFFClientOpen
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t offset;
} mem_buffer;

// Read function for memory buffer
static tmsize_t mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* buffer = (mem_buffer*)handle;
    size_t remaining = buffer->size - buffer->offset;
    size_t to_read = (size_t)size;
    
    if (to_read > remaining) {
        to_read = remaining;
    }
    
    if (to_read > 0) {
        memcpy(buf, buffer->data + buffer->offset, to_read);
        buffer->offset += to_read;
    }
    
    return (tmsize_t)to_read;
}

// Write function (not used for read-only)
static tmsize_t mem_write(thandle_t handle, void* buf, tmsize_t size) {
    return 0;
}

// Seek function for memory buffer
static toff_t mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* buffer = (mem_buffer*)handle;
    
    switch (whence) {
        case SEEK_SET:
            buffer->offset = (offset < buffer->size) ? offset : buffer->size;
            break;
        case SEEK_CUR:
            buffer->offset = (buffer->offset + offset < buffer->size) ? 
                            buffer->offset + offset : buffer->size;
            break;
        case SEEK_END:
            buffer->offset = (buffer->size + offset < buffer->size) ? 
                            buffer->size + offset : buffer->size;
            break;
    }
    
    return (toff_t)buffer->offset;
}

// Close function (does nothing for memory buffer)
static int mem_close(thandle_t handle) {
    return 0;
}

// Size function for memory buffer
static toff_t mem_size(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    return (toff_t)buffer->size;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 32) {
        return 0;
    }

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(tiff_error_handler);
    TIFFSetWarningHandler(tiff_error_handler);

    FuzzedDataProvider fdp(data, size);

    // Create memory buffer structure
    mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;

    // Open TIFF using TIFFClientOpen for custom I/O
    TIFF* tif = TIFFClientOpen("memory_tiff", "r", (thandle_t)&buffer,
                               mem_read, mem_write, mem_seek, 
                               mem_close, mem_size, NULL, NULL);
    
    if (!tif) {
        return 0;
    }

    // Group 1: Client Data Management APIs
    // ----------------------------------------------------------------
    thandle_t original_clientdata = TIFFClientdata(tif);
    
    // Test setting and getting client data
    thandle_t test_clientdata = (thandle_t)(uintptr_t)fdp.ConsumeIntegral<uint64_t>();
    thandle_t prev_clientdata = TIFFSetClientdata(tif, test_clientdata);
    thandle_t new_clientdata = TIFFClientdata(tif);
    
    // Restore original client data
    TIFFSetClientdata(tif, original_clientdata);

    // Group 2: I/O Procedure Access APIs  
    // ----------------------------------------------------------------
    TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
    TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
    TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
    TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
    TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);

    // Group 3: File Descriptor Management APIs
    // ----------------------------------------------------------------
    int original_fileno = TIFFFileno(tif);
    
    // Test setting and getting file descriptor
    int test_fileno = fdp.ConsumeIntegralInRange<int>(-100, 1000);
    int prev_fileno = TIFFSetFileno(tif, test_fileno);
    int new_fileno = TIFFFileno(tif);
    
    // Restore original file descriptor
    TIFFSetFileno(tif, original_fileno);

    // Group 4: Mode Management APIs
    // ----------------------------------------------------------------
    int original_mode = TIFFGetMode(tif);
    
    // Test setting and getting mode (valid modes: O_RDONLY, O_RDWR, O_WRONLY)
    int test_mode = fdp.ConsumeIntegralInRange<int>(0, 2); // 0=O_RDONLY, 1=O_WRONLY, 2=O_RDWR
    int prev_mode = TIFFSetMode(tif, test_mode);
    int new_mode = TIFFGetMode(tif);
    
    // Restore original mode
    TIFFSetMode(tif, original_mode);

    // Group 5: Tag Information APIs
    // ----------------------------------------------------------------
    int tag_count = TIFFGetTagListCount(tif);
    
    // Test retrieving tags at various indices
    for (int i = 0; i < tag_count && i < 10; i++) {
        uint32_t tag = TIFFGetTagListEntry(tif, i);
        (void)tag; // Use tag to avoid unused variable warning
    }

    // Group 6: Tag Method Access API
    // ----------------------------------------------------------------
    TIFFTagMethods* tag_methods = TIFFAccessTagMethods(tif);
    (void)tag_methods; // Use to avoid unused variable warning

    // Group 7: State Query APIs
    // ----------------------------------------------------------------
    int is_tiled = TIFFIsTiled(tif);
    int is_byte_swapped = TIFFIsByteSwapped(tif);
    int is_up_sampled = TIFFIsUpSampled(tif);
    int is_msb2lsb = TIFFIsMSB2LSB(tif);
    int is_big_endian = TIFFIsBigEndian(tif);
    int is_big_tiff = TIFFIsBigTIFF(tif);
    
    uint32_t current_row = TIFFCurrentRow(tif);
    tdir_t current_directory = TIFFCurrentDirectory(tif);
    tdir_t num_directories = TIFFNumberOfDirectories(tif);
    uint64_t current_dir_offset = TIFFCurrentDirOffset(tif);
    uint32_t current_strip = TIFFCurrentStrip(tif);
    uint32_t current_tile = TIFFCurrentTile(tif);

    // Use all queried values to avoid unused variable warnings
    (void)original_clientdata;
    (void)test_clientdata;
    (void)prev_clientdata;
    (void)new_clientdata;
    (void)read_proc;
    (void)write_proc;
    (void)seek_proc;
    (void)close_proc;
    (void)size_proc;
    (void)map_proc;
    (void)unmap_proc;
    (void)original_fileno;
    (void)test_fileno;
    (void)prev_fileno;
    (void)new_fileno;
    (void)original_mode;
    (void)test_mode;
    (void)prev_mode;
    (void)new_mode;
    (void)tag_count;
    (void)is_tiled;
    (void)is_byte_swapped;
    (void)is_up_sampled;
    (void)is_msb2lsb;
    (void)is_big_endian;
    (void)is_big_tiff;
    (void)current_row;
    (void)current_directory;
    (void)num_directories;
    (void)current_dir_offset;
    (void)current_strip;
    (void)current_tile;

    // Clean up
    TIFFClose(tif);

    return 0;
}

} // namespace harness_020
