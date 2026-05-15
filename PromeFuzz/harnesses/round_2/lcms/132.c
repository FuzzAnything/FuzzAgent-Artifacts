// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void custom_error_handler(cmsContext context, cmsUInt32Number error_code, const char* error_text) {
    // Custom error handler that does nothing to avoid output during fuzzing
    (void)context;
    (void)error_code;
    (void)error_text;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Set custom error handler
    cmsSetLogErrorHandlerTHR(ctx, custom_error_handler);

    // Use first byte to determine which function to test more extensively
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t offset = Size > 0 ? 1 : 0;
    size_t remaining = Size > offset ? Size - offset : 0;

    // Test cmsOpenProfileFromMemTHR with the input data
    cmsHPROFILE mem_profile = cmsOpenProfileFromMemTHR(ctx, Data + offset, remaining);
    if (mem_profile) {
        cmsCloseProfile(mem_profile);
    }

    // Test cmsOpenProfileFromFileTHR by writing data to a file
    const char* filename = "./dummy_file";
    FILE* f = fopen(filename, "wb");
    if (f) {
        if (remaining > 0) {
            fwrite(Data + offset, 1, remaining, f);
        }
        fclose(f);
        
        // Try opening with different access modes based on selector
        const char* access_mode = (selector & 1) ? "r" : "w";
        cmsHPROFILE file_profile = cmsOpenProfileFromFileTHR(ctx, filename, access_mode);
        if (file_profile) {
            cmsCloseProfile(file_profile);
        }
        
        // Clean up the dummy file
        remove(filename);
    }

    // Test cmsSignalError with various error codes and messages
    if (remaining > 0) {
        // Use part of the data as error code (first 4 bytes if available)
        cmsUInt32Number error_code = 0;
        size_t code_size = remaining > sizeof(cmsUInt32Number) ? sizeof(cmsUInt32Number) : remaining;
        memcpy(&error_code, Data + offset, code_size);
        
        // Create error message from remaining data
        char error_msg[256];
        size_t msg_len = remaining > 255 ? 255 : remaining;
        if (msg_len > 0) {
            memcpy(error_msg, Data + offset, msg_len);
            error_msg[msg_len] = '\0';
            cmsSignalError(ctx, error_code, "%s", error_msg);
        } else {
            cmsSignalError(ctx, error_code, "Test error");
        }
    }

    // Test cmsCreateTransformTHR with profiles created from memory
    if (remaining > 100) {  // Need enough data for two profiles
        // Split remaining data into two parts for input and output profiles
        size_t split = remaining / 2;
        
        cmsHPROFILE input_profile = cmsOpenProfileFromMemTHR(ctx, Data + offset, split);
        cmsHPROFILE output_profile = cmsOpenProfileFromMemTHR(ctx, Data + offset + split, remaining - split);
        
        if (input_profile || output_profile) {
            // Try different combinations based on selector bits
            cmsUInt32Number input_format = (selector & 0x0F) << 8;
            cmsUInt32Number output_format = ((selector & 0xF0) >> 4) << 8;
            cmsUInt32Number intent = selector % 4;  // 0-3
            cmsUInt32Number flags = selector;
            
            cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx,
                input_profile,
                input_format,
                output_profile,
                output_format,
                intent,
                flags);
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
        }
        
        if (input_profile) {
            cmsCloseProfile(input_profile);
        }
        if (output_profile) {
            cmsCloseProfile(output_profile);
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);

    return 0;
}