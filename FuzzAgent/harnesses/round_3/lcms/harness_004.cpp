#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_004 {
// Fuzzing harness for LittleCMS (lcms) profile I/O operations
// This harness specifically targets profile I/O functions from cmsio0.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsOpenProfileFromStream, cmsOpenProfileFromStreamTHR,
// cmsOpenProfileFromFile, cmsOpenProfileFromFileTHR,
// cmsOpenProfileFromIOhandler2THR, cmsOpenProfileFromIOhandlerTHR,
// cmsSaveProfileToMem, cmsSaveProfileToStream


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing of I/O operations
    // Minimum: parameters for context creation + buffer sizes + file operations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Create a test profile (sRGB) to save and reload
    cmsHPROFILE source_profile = cmsCreate_sRGBProfileTHR(context);
    if (source_profile == NULL) {
        // Fall back to non-thread-safe version if available
        source_profile = cmsCreate_sRGBProfile();
        if (source_profile == NULL) {
            cmsDeleteContext(context);
            return 0; // Cannot create test profile
        }
    }
    
    // TEST 1: Save profile to memory and reload
    // First, determine required buffer size
    cmsUInt32Number mem_size = 0;
    cmsSaveProfileToMem(source_profile, NULL, &mem_size);
    
    if (mem_size > 0) {
        // Allocate buffer for profile data
        std::vector<uint8_t> mem_buffer(mem_size);
        
        // Save profile to memory
        cmsBool save_result = cmsSaveProfileToMem(source_profile, mem_buffer.data(), &mem_size);
        if (save_result) {
            // TEST 1a: Open profile from memory buffer (already covered but good to test)
            cmsHPROFILE mem_profile = cmsOpenProfileFromMem(mem_buffer.data(), mem_size);
            if (mem_profile != NULL) {
                // Use the loaded profile in a simple transform
                cmsHPROFILE dest_profile = cmsCreate_sRGBProfile();
                if (dest_profile != NULL) {
                    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
                    cmsHTRANSFORM transform = cmsCreateTransform(mem_profile, TYPE_RGB_8,
                                                                 dest_profile, TYPE_RGB_8,
                                                                 intent, 0);
                    if (transform != NULL) {
                        uint8_t rgb_buffer[3] = {0, 0, 0};
                        cmsDoTransform(transform, rgb_buffer, rgb_buffer, 1);
                        cmsDeleteTransform(transform);
                    }
                    cmsCloseProfile(dest_profile);
                }
                cmsCloseProfile(mem_profile);
            }
        }
    }
    
    // TEST 2: Save profile to stream and reload
    // Create a temporary file for stream operations
    char temp_filename[] = "/tmp/lcms_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd != -1) {
        FILE* temp_stream = fdopen(temp_fd, "w+b");
        if (temp_stream != NULL) {
            // Save profile to stream
            cmsBool stream_save_result = cmsSaveProfileToStream(source_profile, temp_stream);
            if (stream_save_result) {
                // cmsSaveProfileToStream takes ownership and closes the stream via FileClose
                // The stream is now closed - cannot use it for reading
                // Do NOT call fclose here
                // fclose(temp_stream); // REMOVED: causes double-free
            } else {
                // Save failed, stream is still open
                // We can test cmsOpenProfileFromStream with the still-open stream
                rewind(temp_stream);
                
                // TEST 2a: Open profile from stream (non-thread-safe)
                cmsHPROFILE stream_profile = cmsOpenProfileFromStream(temp_stream, "r");
                if (stream_profile != NULL) {
                    cmsCloseProfile(stream_profile);
                }
                
                // Reset stream position again
                rewind(temp_stream);
                
                // TEST 2b: Open profile from stream (thread-safe)
                cmsHPROFILE stream_profile_thr = cmsOpenProfileFromStreamTHR(context, temp_stream, "r");
                if (stream_profile_thr != NULL) {
                    cmsCloseProfile(stream_profile_thr);
                }
                // Close the stream since save failed and library didn't close it
                fclose(temp_stream);
            }
            // Note: Do NOT call close(temp_fd) here because:
            // - If stream_save_result is TRUE: cmsSaveProfileToStream closed the stream via FileClose
            // - If stream_save_result is FALSE: fclose(temp_stream) closed the stream
            // Both fclose and FileClose close the underlying file descriptor
        } else {
            // fdopen failed, close the file descriptor
            close(temp_fd);
        }
        unlink(temp_filename);
    }
    
    // TEST 3: Save profile to file and reload
    // Create another temporary file for file operations
    char temp_file2[] = "/tmp/lcms_fuzz_file_XXXXXX";
    int temp_fd2 = mkstemp(temp_file2);
    if (temp_fd2 != -1) {
        FILE* temp_file_stream = fdopen(temp_fd2, "w+b");
        if (temp_file_stream != NULL) {
            // Save profile to file via stream (library takes ownership of stream)
            cmsBool file_save_result = cmsSaveProfileToStream(source_profile, temp_file_stream);
            if (file_save_result) {
                // cmsSaveProfileToStream already closed the stream via FileClose
                // Do NOT call fclose here
                // fclose(temp_file_stream); // REMOVED: causes double-free
                
                // TEST 3a: Open profile from file (non-thread-safe)
                cmsHPROFILE file_profile = cmsOpenProfileFromFile(temp_file2, "r");
                if (file_profile != NULL) {
                    cmsCloseProfile(file_profile);
                }
                
                // TEST 3b: Open profile from file (thread-safe)
                cmsHPROFILE file_profile_thr = cmsOpenProfileFromFileTHR(context, temp_file2, "r");
                if (file_profile_thr != NULL) {
                    cmsCloseProfile(file_profile_thr);
                }
            } else {
                // Save failed, stream is still open, need to close it
                fclose(temp_file_stream);
            }
        } else {
            close(temp_fd2);
        }
        unlink(temp_file2);
    }
    
    // TEST 4: I/O handler operations
    // First, we need to save profile to memory buffer for I/O handler
    cmsUInt32Number handler_mem_size = 0;
    cmsSaveProfileToMem(source_profile, NULL, &handler_mem_size);
    
    if (handler_mem_size > 0 && fdp.remaining_bytes() >= handler_mem_size) {
        std::vector<uint8_t> handler_buffer(handler_mem_size);
        cmsSaveProfileToMem(source_profile, handler_buffer.data(), &handler_mem_size);
        
        // Create I/O handler from memory
        cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromMem(context, handler_buffer.data(), 
                                                          handler_mem_size, "r");
        if (io_handler != NULL) {
            // TEST 4a: Open profile from I/O handler (thread-safe)
            cmsHPROFILE io_profile = cmsOpenProfileFromIOhandlerTHR(context, io_handler);
            if (io_profile != NULL) {
                // Success: ownership transferred to profile, profile will close handler
                cmsCloseProfile(io_profile);
                // io_handler is now invalid (closed by cmsCloseProfile), don't use it
                // Create a new handler for TEST 4b
                io_handler = cmsOpenIOhandlerFromMem(context, handler_buffer.data(), 
                                                    handler_mem_size, "r");
            } else {
                // Failed: ownership not transferred, need to close handler
                cmsCloseIOhandler(io_handler);
                // Create a new handler for TEST 4b
                io_handler = cmsOpenIOhandlerFromMem(context, handler_buffer.data(), 
                                                    handler_mem_size, "r");
            }
            
            if (io_handler != NULL) {
                // TEST 4b: Open profile from I/O handler 2 (with write flag)
                cmsHPROFILE io_profile2 = cmsOpenProfileFromIOhandler2THR(context, io_handler, FALSE);
                if (io_profile2 != NULL) {
                    // Success: ownership transferred to profile, profile will close handler
                    cmsCloseProfile(io_profile2);
                    // io_handler is now invalid, don't close it
                } else {
                    // Failed: ownership not transferred, need to close handler
                    cmsCloseIOhandler(io_handler);
                }
            }
        }
    }
    
    // TEST 5: Test profile saving to I/O handler
    if (fdp.remaining_bytes() > 1024) {
        // Create a memory buffer for I/O handler write testing
        size_t write_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
        std::vector<uint8_t> write_buffer(write_buffer_size);
        
        cmsIOHANDLER* write_handler = cmsOpenIOhandlerFromMem(context, write_buffer.data(),
                                                             write_buffer_size, "w");
        if (write_handler != NULL) {
            cmsUInt32Number bytes_written = cmsSaveProfileToIOhandler(source_profile, write_handler);
            // bytes_written may be 0 if failed, but that's OK for fuzzing
            cmsCloseIOhandler(write_handler);
        }
    }
    
    // Cleanup
    cmsCloseProfile(source_profile);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_004
