// This is the entry of 14 fuzz drivers:
// 1, 2, 6, 9, 21, 23, 28, 29, 32, 33, 36, 40, 43, 44
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);

// Entry function
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Driver selector
    if (Size < 1) {
        return 0;
    }
    const uint8_t *selector = Data;
    unsigned int driverIndex = 0;
    memcpy(&driverIndex, selector, 1);

    // Remain data and size
    const uint8_t *remainData = Data + 1;
    size_t remainSize = Size - 1;
    if (remainSize == 0) {
        return 0;
    }

    // Select driver
    switch (driverIndex % 14) {
        case 0:
            return LLVMFuzzerTestOneInput_1(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_2(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_6(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_9(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_21(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_23(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_28(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_29(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_32(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_33(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_36(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_40(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_43(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_44(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
