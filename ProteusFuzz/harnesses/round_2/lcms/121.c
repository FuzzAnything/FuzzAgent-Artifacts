// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_ctx = NULL;

static void cleanup_context(void) {
    if (g_ctx != NULL) {
        cmsDeleteContext(g_ctx);
        g_ctx = NULL;
    }
}

static cmsContext get_context(void) {
    if (g_ctx == NULL) {
        g_ctx = cmsCreateContext(NULL, NULL);
        if (g_ctx == NULL) {
            return NULL;
        }
    }
    return g_ctx;
}

// Helper to create a dummy IO handler using a file
static cmsIOHANDLER* create_io_handler(const char* filename, int mode) {
    cmsIOHANDLER* io = NULL;
    int fd = -1;

    if (mode == O_RDONLY) {
        fd = open(filename, O_RDONLY);
    } else {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    if (fd < 0) {
        return NULL;
    }

    // Use cmsOpenIOhandlerFromFile which is the public API for file-based IO
    // Note: cmsOpenIOhandlerFromFile takes a filename, not an FD directly.
    // We close the FD we opened and let lcms open it again, or we use the filename directly.
    close(fd);

    if (mode == O_RDONLY) {
        io = cmsOpenIOhandlerFromFile(g_ctx, filename, "r");
    } else {
        io = cmsOpenIOhandlerFromFile(g_ctx, filename, "w");
    }

    return io;
}

static void destroy_io_handler(cmsIOHANDLER* io) {
    if (io != NULL) {
        cmsCloseIOhandler(io);
    }
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    cmsContext ctx = get_context();
    if (ctx == NULL) {
        return 0;
    }

    // Ensure cleanup on exit
    atexit(cleanup_context);

    // Fuzz _cms8Fixed8toDouble
    if (Size >= 2) {
        cmsUInt16Number val8 = (Data[0] << 8) | Data[1];
        cmsFloat64Number res8 = _cms8Fixed8toDouble(val8);
        (void)res8; 
    }

    // Fuzz _cms15Fixed16toDouble
    if (Size >= 4) {
        cmsS15Fixed16Number val16 = *(const cmsS15Fixed16Number*)(Data);
        cmsFloat64Number res16 = _cms15Fixed16toDouble(val16);
        (void)res16;
    }

    // Fuzz _cmsDoubleTo8Fixed8
    if (Size >= 8) {
        cmsFloat64Number dval = *(const cmsFloat64Number*)(Data);
        cmsUInt16Number res8out = _cmsDoubleTo8Fixed8(dval);
        (void)res8out;
    }

    // Fuzz _cmsDoubleTo15Fixed16
    if (Size >= 8) {
        cmsFloat64Number dval2 = *(const cmsFloat64Number*)(Data);
        cmsS15Fixed16Number res16out = _cmsDoubleTo15Fixed16(dval2);
        (void)res16out;
    }

    // Fuzz _cmsWrite15Fixed16Number and _cmsRead15Fixed16Number
    if (Size >= 8) {
        cmsFloat64Number valToWrite = *(const cmsFloat64Number*)(Data);
        
        // Write to dummy file
        write_dummy_file(Data, Size);
        
        // Open file for writing via IO handler
        cmsIOHANDLER* ioWrite = create_io_handler("./dummy_file", O_WRONLY);
        if (ioWrite != NULL) {
            cmsBool writeRes = _cmsWrite15Fixed16Number(ioWrite, valToWrite);
            (void)writeRes;
            destroy_io_handler(ioWrite);
        }

        // Open file for reading via IO handler
        cmsIOHANDLER* ioRead = create_io_handler("./dummy_file", O_RDONLY);
        if (ioRead != NULL) {
            cmsFloat64Number readVal;
            cmsBool readRes = _cmsRead15Fixed16Number(ioRead, &readVal);
            (void)readRes;
            (void)readVal;
            destroy_io_handler(ioRead);
        }
    }

    return 0;
}