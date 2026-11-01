// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzdopen at gzlib.c:298:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>

static int fuzz_gzdirect(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int ret = 0;

    // Create a temporary file for testing
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    if (Size > 0) {
        write(fd, Data, Size);
    }
    lseek(fd, 0, SEEK_SET);

    // Test with read mode
    file = gzdopen(fd, "rb");
    if (file != NULL) {
        ret = gzdirect(file);
        gzclose(file);
    }

    // Test with write mode (create new file)
    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        file = gzdopen(fd, "wb");
        if (file != NULL) {
            ret = gzdirect(file);
            gzclose(file);
        }
    }

    // Test with NULL input
    ret = gzdirect(NULL);

    close(fd);
    return 0;
}

static int fuzz_gzflush(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int fd;
    int ret = 0;
    int flush_mode = 0;

    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    file = gzdopen(fd, "wb");
    if (file == NULL) {
        close(fd);
        return 0;
    }

    // Write some data
    if (Size > 0) {
        gzwrite(file, Data, Size);
    }

    // Fuzz flush modes (0 to Z_FINISH)
    flush_mode = (Size > 0) ? (Data[0] % (Z_FINISH + 1)) : 0;
    ret = gzflush(file, flush_mode);

    gzclose(file);
    close(fd);
    return 0;
}

static int fuzz_gzclose_w(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int fd;
    int ret = 0;

    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    file = gzdopen(fd, "wb");
    if (file == NULL) {
        close(fd);
        return 0;
    }

    // Write some data
    if (Size > 0) {
        gzwrite(file, Data, Size);
    }

    // Close with gzclose_w
    ret = gzclose_w(file);

    close(fd);
    return 0;
}

static int fuzz_gzsetparams(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int fd;
    int ret = 0;
    int level = 0;
    int strategy = 0;

    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    file = gzdopen(fd, "wb");
    if (file == NULL) {
        close(fd);
        return 0;
    }

    // Write some data first
    if (Size > 0) {
        gzwrite(file, Data, Size);
    }

    // Fuzz level and strategy
    level = (Size > 0) ? (Data[0] % (Z_BEST_COMPRESSION + 1)) : Z_DEFAULT_COMPRESSION;
    strategy = (Size > 1) ? (Data[1] % (Z_FIXED + 1)) : Z_DEFAULT_STRATEGY;

    ret = gzsetparams(file, level, strategy);

    gzclose(file);
    close(fd);
    return 0;
}

static int fuzz_gzdopen(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int fd;
    int ret = 0;
    const char *mode = "rb";

    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    if (Size > 0) {
        write(fd, Data, Size);
    }
    lseek(fd, 0, SEEK_SET);

    // Test read mode
    file = gzdopen(fd, "rb");
    if (file != NULL) {
        gzclose(file);
    }

    // Test write mode
    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        file = gzdopen(fd, "wb");
        if (file != NULL) {
            gzclose(file);
        }
    }

    // Test invalid fd
    file = gzdopen(-1, "rb");

    close(fd);
    return 0;
}

static int fuzz_gzbuffer(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int fd;
    int ret = 0;
    unsigned size = 0;

    fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;

    file = gzdopen(fd, "wb");
    if (file == NULL) {
        close(fd);
        return 0;
    }

    // Fuzz buffer size
    if (Size > 0) {
        size = (Data[0] % 1000) + 8; // Ensure minimum size of 8
    } else {
        size = 8;
    }

    ret = gzbuffer(file, size);

    gzclose(file);
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Run all fuzzing functions
    fuzz_gzdirect(Data, Size);
    fuzz_gzflush(Data, Size);
    fuzz_gzclose_w(Data, Size);
    fuzz_gzsetparams(Data, Size);
    fuzz_gzdopen(Data, Size);
    fuzz_gzbuffer(Data, Size);

    return 0;
}