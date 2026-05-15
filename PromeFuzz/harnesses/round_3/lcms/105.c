// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_read_write_cleanup(cmsIOHANDLER* io) {
    if (io) {
        if (io->Close) {
            io->Close(io);
        }
    }
}

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer || size == 0 || count == 0) return 0;
    
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    cmsUInt32Number to_read = total_bytes < available ? total_bytes : available;
    
    if (to_read > 0) {
        memcpy(Buffer, (const uint8_t*)iohandler->stream + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    return to_read / size;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (!iohandler) return FALSE;
    if (offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return iohandler ? iohandler->UsedSpace : 0;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler && iohandler->stream) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    if (!iohandler || !Buffer || size == 0) return FALSE;
    
    size_t new_used = iohandler->UsedSpace + size;
    if (new_used > iohandler->ReportedSize) {
        size_t new_size = new_used * 2;
        void* new_stream = realloc(iohandler->stream, new_size);
        if (!new_stream) return FALSE;
        iohandler->stream = new_stream;
        iohandler->ReportedSize = new_size;
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
    io->Tell = dummy_tell;
    io->Close = dummy_close;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // 1. Test cmsGetProfileVersion
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    if (profile) {
        cmsFloat64Number version = cmsGetProfileVersion(profile);
        (void)version; // Use result to avoid optimization
        cmsCloseProfile(profile);
    }
    
    // 2. Test _cmsDoubleTo15Fixed16
    double test_double = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&test_double, Data, sizeof(double));
    } else {
        test_double = (double)(Data[0] / 255.0);
    }
    cmsS15Fixed16Number fixed = _cmsDoubleTo15Fixed16(test_double);
    (void)fixed;
    
    // 3. Test cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    (void)cmm_version;
    
    // 4. Test _cms15Fixed16toDouble
    cmsS15Fixed16Number test_fixed = 0;
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        memcpy(&test_fixed, Data, sizeof(cmsS15Fixed16Number));
    } else {
        test_fixed = Data[0];
    }
    double converted_double = _cms15Fixed16toDouble(test_fixed);
    (void)converted_double;
    
    // 5. Test _cmsRead15Fixed16Number
    cmsIOHANDLER* read_io = create_dummy_iohandler(Data, Size);
    if (read_io) {
        cmsFloat64Number read_number = 0.0;
        cmsBool read_result = _cmsRead15Fixed16Number(read_io, &read_number);
        (void)read_result;
        (void)read_number;
        dummy_read_write_cleanup(read_io);
        free(read_io);
    }
    
    // 6. Test _cmsWrite15Fixed16Number
    cmsIOHANDLER* write_io = create_dummy_iohandler(NULL, 0);
    if (write_io) {
        double write_number = test_double;
        cmsBool write_result = _cmsWrite15Fixed16Number(write_io, write_number);
        (void)write_result;
        dummy_read_write_cleanup(write_io);
        free(write_io);
    }
    
    return 0;
}