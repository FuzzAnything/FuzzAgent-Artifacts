#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <tiffio.h>

namespace harness_014 {
/* Fuzzing harness for libtiff targeting TIFFClientOpen with custom I/O pathways
 * Focus: Custom I/O operations via TIFFClientOpen API with 370+ undiscovered branches
 * Target API: TIFFClientOpen (primary target - critical uncovered API)
 * Strategy: Implement custom I/O callback functions to test all I/O pathways through TIFFClientOpen
 *            including read/write/seek/close/size/map/unmap operations
 * Coverage Gap: TIFFClientOpen has complex error handling and state management for custom I/O
 *               that is rarely exercised in normal file-based operations
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

/* Custom I/O callback implementations for TIFFClientOpen */

// Custom I/O context structure
typedef struct {
    std::vector<uint8_t>* buffer;     // Data buffer
    size_t position;                  // Current read/write position
    bool read_only;                   // Is buffer read-only?
    bool error_triggered;             // Should next operation fail?
} custom_io_context;

// Custom read procedure
static tmsize_t custom_read_proc(thandle_t handle, void* buf, tmsize_t size) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx || !ctx->buffer || ctx->error_triggered) {
        return static_cast<tmsize_t>(-1);
    }
    
    if (ctx->position >= ctx->buffer->size()) {
        return 0; // EOF
    }
    
    size_t bytes_to_read = std::min(static_cast<size_t>(size), 
                                     ctx->buffer->size() - ctx->position);
    if (bytes_to_read == 0) {
        return 0;
    }
    
    memcpy(buf, ctx->buffer->data() + ctx->position, bytes_to_read);
    ctx->position += bytes_to_read;
    
    return static_cast<tmsize_t>(bytes_to_read);
}

// Custom write procedure
static tmsize_t custom_write_proc(thandle_t handle, void* buf, tmsize_t size) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx || !ctx->buffer || ctx->read_only || ctx->error_triggered) {
        return static_cast<tmsize_t>(-1);
    }
    
    // Expand buffer if needed
    if (ctx->position + size > ctx->buffer->size()) {
        ctx->buffer->resize(ctx->position + size);
    }
    
    memcpy(ctx->buffer->data() + ctx->position, buf, size);
    ctx->position += size;
    
    return size;
}

// Custom seek procedure
static uint64_t custom_seek_proc(thandle_t handle, uint64_t offset, int whence) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx || !ctx->buffer || ctx->error_triggered) {
        return static_cast<uint64_t>(-1);
    }
    
    uint64_t new_position = 0;
    
    switch (whence) {
        case SEEK_SET:
            new_position = offset;
            break;
        case SEEK_CUR:
            new_position = ctx->position + offset;
            break;
        case SEEK_END:
            new_position = ctx->buffer->size() + offset;
            break;
        default:
            return static_cast<uint64_t>(-1);
    }
    
    // Check for overflow/wrap-around
    if (new_position > SIZE_MAX) {
        return static_cast<uint64_t>(-1);
    }
    
    ctx->position = static_cast<size_t>(new_position);
    return new_position;
}

// Custom close procedure
static int custom_close_proc(thandle_t handle) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx) {
        return -1;
    }
    
    // Clean up context
    delete ctx->buffer;
    delete ctx;
    return 0;
}

// Custom size procedure
static uint64_t custom_size_proc(thandle_t handle) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx || !ctx->buffer || ctx->error_triggered) {
        return static_cast<uint64_t>(-1);
    }
    
    return static_cast<uint64_t>(ctx->buffer->size());
}

// Custom map procedure (dummy implementation)
static int custom_map_proc(thandle_t handle, void** base, toff_t* size) {
    custom_io_context* ctx = reinterpret_cast<custom_io_context*>(handle);
    
    if (!ctx || !ctx->buffer || ctx->error_triggered) {
        return -1;
    }
    
    *base = ctx->buffer->data();
    *size = static_cast<toff_t>(ctx->buffer->size());
    return 0;
}

// Custom unmap procedure (dummy implementation)
static void custom_unmap_proc(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
    // Nothing to do for memory buffer
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration
    if (size < 64) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation configuration
    uint8_t mode_selection = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: read, 1: write, 2: read/write
    uint8_t io_behavior = fdp.ConsumeIntegral<uint8_t>() % 4;     // Different I/O behavior patterns
    bool trigger_errors = fdp.ConsumeBool();                      // Should trigger I/O errors
    bool test_map_unmap = fdp.ConsumeBool();                      // Test map/unmap operations
    
    // Determine mode string
    const char* mode = "r";
    bool read_only = true;
    switch (mode_selection) {
        case 0:
            mode = "r";
            read_only = true;
            break;
        case 1:
            mode = "w";
            read_only = false;
            break;
        case 2:
            mode = "r+";
            read_only = false;
            break;
    }

    // Consume initial buffer data
    size_t initial_buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    std::vector<uint8_t> initial_buffer = fdp.ConsumeBytes<uint8_t>(initial_buffer_size);
    
    // Create custom I/O context
    custom_io_context* ctx = new custom_io_context();
    if (!ctx) {
        return 0;
    }
    
    ctx->buffer = new std::vector<uint8_t>(initial_buffer);
    ctx->position = 0;
    ctx->read_only = read_only;
    ctx->error_triggered = trigger_errors;
    
    // Adjust I/O behavior based on configuration
    switch (io_behavior) {
        case 0:  // Normal behavior
            // Already set up
            break;
        case 1:  // Start at end of buffer
            ctx->position = ctx->buffer->size();
            break;
        case 2:  // Start in middle of buffer
            if (!ctx->buffer->empty()) {
                ctx->position = ctx->buffer->size() / 2;
            }
            break;
        case 3:  // Start beyond buffer (error condition)
            ctx->position = ctx->buffer->size() + 100;
            break;
    }

    // Open TIFF using custom I/O callbacks
    TIFF* tif = TIFFClientOpen("CustomIOTIFF", mode, 
                               reinterpret_cast<thandle_t>(ctx),
                               custom_read_proc,
                               custom_write_proc,
                               custom_seek_proc,
                               custom_close_proc,
                               custom_size_proc,
                               test_map_unmap ? custom_map_proc : NULL,
                               test_map_unmap ? custom_unmap_proc : NULL);
    
    if (!tif) {
        // Clean up context if TIFFClientOpen failed
        delete ctx->buffer;
        delete ctx;
        return 0;
    }

    // Test various TIFF operations to exercise the custom I/O pathways
    
    // 1. Try to read basic TIFF fields if in read mode
    if (mode_selection == 0 || mode_selection == 2) {  // Read or read/write mode
        uint32_t width = 0, height = 0;
        uint16_t samples = 0, bits = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
        
        // Try to get strip/tile information
        uint32_t strips = TIFFNumberOfStrips(tif);
        uint32_t tiles = TIFFNumberOfTiles(tif);
        
        // Try directory operations
        uint16_t dir_count = 0;
        do {
            dir_count++;
        } while (TIFFReadDirectory(tif) && dir_count < 5);  // Limit to avoid infinite loops
    }
    
    // 2. Try to write TIFF fields if in write mode
    if (mode_selection == 1 || mode_selection == 2) {  // Write or read/write mode
        // Set basic image parameters
        uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
        uint16_t samples = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        uint16_t bits = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
        
        // Try to write a directory
        TIFFWriteDirectory(tif);
    }
    
    // 3. Test seek operations through TIFF
    if (!trigger_errors) {
        // Get current directory offset
        uint64_t dir_offset = TIFFCurrentDirOffset(tif);
        
        // Try to flush data (exercises write pathways)
        TIFFFlush(tif);
    }
    
    // 4. Test client data functions
    thandle_t client_data = TIFFClientdata(tif);
    if (client_data) {
        // The client data should be our context
        TIFFSetClientdata(tif, client_data);
    }
    
    // Close the TIFF file (will call our custom_close_proc)
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_014
