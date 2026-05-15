// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the dummy file as a profile
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    if (hProfile) {
        // Get I/O handler from profile
        cmsIOHANDLER* io = cmsGetProfileIOhandler(hProfile);
        
        if (io) {
            // Test _cmsReadTypeBase
            cmsTagTypeSignature sig = _cmsReadTypeBase(io);
            
            // Test _cmsReadAlignment
            cmsBool align_read = _cmsReadAlignment(io);
            
            // Test _cmsWriteAlignment
            cmsBool align_write = _cmsWriteAlignment(io);
            
            // Test _cmsWriteTypeBase with a signature from the fuzzing data
            cmsTagTypeSignature write_sig = 0;
            if (Size >= sizeof(cmsTagTypeSignature)) {
                memcpy(&write_sig, Data, sizeof(cmsTagTypeSignature));
            }
            cmsBool write_type = _cmsWriteTypeBase(io, write_sig);
            
            // Test _cmsIOPrintf with a simple format string
            // Use a small buffer to avoid overflow
            char format[32];
            if (Size > 0) {
                size_t format_len = Size < 31 ? Size : 31;
                memcpy(format, Data, format_len);
                format[format_len] = '\0';
                // Ensure format string is null-terminated and safe
                for (size_t i = 0; i < format_len; i++) {
                    if (format[i] == '\0') break;
                    if (format[i] < 32 || format[i] > 126) format[i] = 'a';
                }
                _cmsIOPrintf(io, "%s", format);
            }
        }
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    // Also test with a memory-based I/O handler
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        // Create memory I/O handler for reading
        cmsIOHANDLER* mem_io = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
        if (mem_io) {
            // Test read functions
            cmsTagTypeSignature mem_sig = _cmsReadTypeBase(mem_io);
            cmsBool mem_align = _cmsReadAlignment(mem_io);
            
            // Create another memory I/O handler for writing
            uint8_t write_buffer[1024];
            cmsIOHANDLER* write_io = cmsOpenIOhandlerFromMem(ctx, write_buffer, sizeof(write_buffer), "w");
            if (write_io) {
                // Test write functions
                cmsTagTypeSignature write_sig = 0;
                if (Size >= sizeof(cmsTagTypeSignature)) {
                    memcpy(&write_sig, Data, sizeof(cmsTagTypeSignature));
                }
                cmsBool write_type = _cmsWriteTypeBase(write_io, write_sig);
                cmsBool write_align = _cmsWriteAlignment(write_io);
                
                // Test printf with write handler
                if (Size > 0) {
                    char test_str[32];
                    size_t str_len = Size < 31 ? Size : 31;
                    memcpy(test_str, Data, str_len);
                    test_str[str_len] = '\0';
                    for (size_t i = 0; i < str_len; i++) {
                        if (test_str[i] == '\0') break;
                        if (test_str[i] < 32 || test_str[i] > 126) test_str[i] = 'a';
                    }
                    _cmsIOPrintf(write_io, "Test: %s", test_str);
                }
                
                cmsCloseIOhandler(write_io);
            }
            
            cmsCloseIOhandler(mem_io);
        }
        
        cmsDeleteContext(ctx);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif