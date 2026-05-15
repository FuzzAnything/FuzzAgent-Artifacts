// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetASCII at cmsnamed.c:336:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/include/lcms2.h"
#include "code/include/lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy I/O handler implementation for fuzzing
static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    cmsUInt32Number total = size * count;
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        total = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, total);
        iohandler->UsedSpace += total;
    }
    return total / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) {
        return FALSE;
    }
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) {
        return FALSE;
    }
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    memcpy(io->stream, data, size);
    io->ReportedSize = size;
    io->UsedSpace = 0;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

// Dummy profile creation for fuzzing
static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    // Create a simple sRGB profile with fuzzer data in description
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    if (!profile) return NULL;
    
    // Try to set some tags with fuzzer data
    if (size > 0) {
        char desc[256];
        size_t desc_len = size < 255 ? size : 255;
        memcpy(desc, data, desc_len);
        desc[desc_len] = '\0';
        
        cmsMLU* mlu = cmsMLUalloc(NULL, 1);
        if (mlu) {
            cmsMLUsetASCII(mlu, "en", "US", desc);
            cmsWriteTag(profile, cmsSigProfileDescriptionTag, mlu);
            cmsMLUfree(mlu);
        }
    }
    
    return profile;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    // 1. Test _cms15Fixed16toDouble
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(fix32));
        cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
        (void)result1; // Avoid unused variable warning
    }
    
    // 2. Test _cmsRead15Fixed16Number
    if (Size > 0) {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
        if (io) {
            cmsFloat64Number n;
            cmsBool read_result = _cmsRead15Fixed16Number(io, &n);
            (void)read_result; // Avoid unused variable warning
            io->Close(io);
        }
    }
    
    // 3. Test cmsGetEncodedCMMversion
    int version = cmsGetEncodedCMMversion();
    (void)version; // Avoid unused variable warning
    
    // 4. Test cmsGetProfileVersion
    if (Size > 0) {
        cmsHPROFILE profile = create_dummy_profile(Data, Size);
        if (profile) {
            cmsFloat64Number profile_version = cmsGetProfileVersion(profile);
            (void)profile_version; // Avoid unused variable warning
            cmsCloseProfile(profile);
        }
    }
    
    // 5. Test _cmsDoubleTo15Fixed16
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number v;
        memcpy(&v, Data, sizeof(v));
        cmsS15Fixed16Number result5 = _cmsDoubleTo15Fixed16(v);
        (void)result5; // Avoid unused variable warning
    }
    
    // 6. Test _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number n;
        memcpy(&n, Data, sizeof(n));
        
        // Create write buffer
        uint8_t* write_buffer = (uint8_t*)malloc(Size);
        if (write_buffer) {
            cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
            if (io) {
                io->stream = write_buffer;
                io->ReportedSize = Size;
                io->UsedSpace = 0;
                io->Read = dummy_read;
                io->Seek = dummy_seek;
                io->Close = dummy_close;
                io->Tell = dummy_tell;
                io->Write = dummy_write;
                
                cmsBool write_result = _cmsWrite15Fixed16Number(io, n);
                (void)write_result; // Avoid unused variable warning
                
                io->Close(io);
            } else {
                free(write_buffer);
            }
        }
    }
    
    return 0;
}