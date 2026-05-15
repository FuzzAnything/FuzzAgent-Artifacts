// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static int dummy_close_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    if (file) {
        fclose(file);
    }
    return 0;
}

static tmsize_t dummy_read_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    if (!file) return -1;
    return fread(buf, 1, size, file);
}

static tmsize_t dummy_write_proc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    if (!file) return -1;
    return fwrite(buf, 1, size, file);
}

static toff_t dummy_seek_proc(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (!file) return -1;
    if (fseek(file, offset, whence) == 0) {
        return ftell(file);
    }
    return -1;
}

static toff_t dummy_size_proc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    if (!file) return -1;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

static int dummy_map_proc(thandle_t handle, void** base, toff_t* size) {
    return 0;
}

static void dummy_unmap_proc(thandle_t handle, void* base, toff_t size) {
    // Do nothing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    const char* filename = "./dummy_file";
    FILE* file = fopen(filename, "wb");
    if (!file) {
        return NULL;
    }
    
    // Write some minimal TIFF header to make it recognizable
    // This is a minimal valid TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic number
        0x08, 0x00, 0x00, 0x00   // First IFD offset
    };
    fwrite(header, 1, sizeof(header), file);
    
    // Write the fuzzer data as additional content
    if (size > 0) {
        size_t write_size = size > 1024 ? 1024 : size;
        fwrite(data, 1, write_size, file);
    }
    
    fclose(file);
    
    // Reopen for reading
    file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }
    
    // Create TIFF handle using client interface
    TIFF* tif = TIFFClientOpen(filename, "r", 
                              (thandle_t)file,
                              dummy_read_proc,
                              dummy_write_proc,
                              dummy_seek_proc,
                              dummy_close_proc,
                              dummy_size_proc,
                              dummy_map_proc,
                              dummy_unmap_proc);
    
    return tif;
}

int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    // Create a dummy TIFF file with the fuzzer data
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        return 0;
    }
    
    // Get directory count to determine valid directory indices
    tdir_t dir_count = TIFFNumberOfDirectories(tif);
    
    // Use fuzzer data to determine directory indices
    uint32_t dir_index1 = 0;
    uint32_t dir_index2 = 0;
    
    if (Size >= 8) {
        memcpy(&dir_index1, Data, sizeof(uint32_t));
        memcpy(&dir_index2, Data + 4, sizeof(uint32_t));
    } else if (Size >= 4) {
        memcpy(&dir_index1, Data, sizeof(uint32_t));
        dir_index2 = dir_index1;
    } else if (Size >= 1) {
        dir_index1 = Data[0];
        dir_index2 = Size >= 2 ? Data[1] : dir_index1;
    }
    
    // Normalize directory indices to valid range
    if (dir_count > 0) {
        dir_index1 = dir_index1 % dir_count;
        dir_index2 = dir_index2 % dir_count;
    } else {
        dir_index1 = 0;
        dir_index2 = 0;
    }
    
    // Call functions in the specified order
    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index1);
    
    // 2. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index2);
    
    // 3. TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset;
    
    // 4. TIFFGetSeekProc
    TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
    (void)seek_proc;
    
    // 5. TIFFIsByteSwapped
    int is_swapped = TIFFIsByteSwapped(tif);
    (void)is_swapped;
    
    // 6. TIFFSwabLong
    uint32_t value_to_swap = dir_index1;
    TIFFSwabLong(&value_to_swap);
    
    // 7. TIFFGetWriteProc
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc;
    
    // 8. TIFFNumberOfDirectories
    dir_count = TIFFNumberOfDirectories(tif);
    (void)dir_count;
    
    // 9. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index1);
    
    // 10. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index2);
    
    // 11. TIFFClose
    TIFFClose(tif);
    
    // 12. TIFFClose - create another TIFF handle to close
    TIFF* tif2 = create_dummy_tiff(Data, Size);
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}