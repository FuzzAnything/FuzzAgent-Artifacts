// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Static helper to create a dummy profile from fuzz data
static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    cmsHPROFILE hProfile = NULL;
    
    // Try to load as an ICC profile if data looks like one (starts with size)
    if (size >= 128) {
        cmsUInt32Number headerSize = 0;
        // Read first 4 bytes as size (big endian)
        if (size >= 4) {
            headerSize = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
            // Basic sanity check for profile size
            if (headerSize >= 128 && headerSize <= size) {
                hProfile = cmsOpenProfileFromMem(data, size);
            }
        }
    }
    
    // If loading failed, try to create a minimal profile for testing
    if (hProfile == NULL) {
        hProfile = cmsCreateProfilePlaceholder(NULL);
    }
    
    return hProfile;
}

// Static helper to create a dummy IO handler
static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *data, size_t size) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    // Initialize with dummy values
    io->stream = NULL;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));
    
    // Set up dummy callbacks (do nothing or minimal operations)
    // We intentionally leave them NULL to test error handling or avoid crashes
    // if the function checks for NULL before calling.
    io->Read = NULL;
    io->Seek = NULL;
    io->Close = NULL;
    io->Tell = NULL;
    io->Write = NULL;
    
    return io;
}

// Static helper to cleanup
static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_io_handler(cmsIOHANDLER *io) {
    if (io != NULL) {
        free(io);
    }
}

// Dummy callback implementations defined at file scope
static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    (void)iohandler; (void)size; (void)Buffer;
    return TRUE;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    (void)iohandler; (void)offset;
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    (void)iohandler;
    return 0;
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsHPROFILE hProfile = NULL;
    cmsIOHANDLER *io = NULL;
    struct tm tmStruct;
    cmsDateTimeNumber dtNumber;
    cmsBool result;
    cmsUInt32Number bytesWritten;
    
    // Initialize tmStruct
    memset(&tmStruct, 0, sizeof(tmStruct));
    memset(&dtNumber, 0, sizeof(dtNumber));
    
    // Create a profile from input data
    hProfile = create_dummy_profile(Data, Size);
    
    if (hProfile != NULL) {
        // Test cmsGetHeaderCreationDateTime
        result = cmsGetHeaderCreationDateTime(hProfile, &tmStruct);
        (void)result; // Suppress unused warning
        
        // Test _cmsDecodeDateTimeNumber with fuzzed data
        if (Size >= sizeof(cmsDateTimeNumber)) {
            memcpy(&dtNumber, Data, sizeof(dtNumber));
            _cmsDecodeDateTimeNumber(&dtNumber, &tmStruct);
        }
        
        // Test _cmsEncodeDateTimeNumber
        _cmsEncodeDateTimeNumber(&dtNumber, &tmStruct);
        
        // Test cmsWriteTag with a safe tag type that doesn't require complex internal structures
        cmsTagSignature sig = (cmsTagSignature)0x00000000;
        if (Size >= 4) {
            memcpy(&sig, Data, 4);
        }
        
        // Test cmsWriteTag with NULL data (delete tag) - safe operation
        result = cmsWriteTag(hProfile, sig, NULL);
        (void)result;
        
        // Create IO handler for testing IO functions
        io = create_dummy_io_handler(Data, Size);
        
        if (io != NULL) {
            // Test _cmsWriteUInt16Number
            // Assign dummy callbacks to avoid assertions inside the library
            io->Write = dummy_write;
            
            cmsUInt16Number num = 0;
            if (Size >= 2) {
                memcpy(&num, Data, 2);
            }
            result = _cmsWriteUInt16Number(io, num);
            (void)result;
            
            // Test cmsSaveProfileToIOhandler
            // This function might also require valid callbacks for Seek, Tell, etc.
            // We provide minimal dummy callbacks to avoid assertions.
            io->Seek = dummy_seek;
            io->Tell = dummy_tell;
            
            bytesWritten = cmsSaveProfileToIOhandler(hProfile, io);
            (void)bytesWritten;
            
            // Test cmsSaveProfileToIOhandler with NULL io
            bytesWritten = cmsSaveProfileToIOhandler(hProfile, NULL);
            (void)bytesWritten;
        }
        
        cleanup_profile(hProfile);
    }
    
    cleanup_io_handler(io);
    
    return 0;
}