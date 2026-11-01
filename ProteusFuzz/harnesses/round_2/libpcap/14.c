// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

// Helper to create a dummy pcap savefile for testing
static pcap_t *create_dummy_pcap(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    
    // Write some data to the dummy file to simulate a pcap file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return NULL;
    }
    
    // Write at least a minimal pcap header if data is provided
    if (size > 0) {
        size_t write_size = (size < 24) ? size : 24;
        write(fd, data, write_size);
    } else {
        // Write a minimal valid pcap header (24 bytes) if no data provided
        unsigned char header[24] = {0};
        header[0] = 0xa1; header[1] = 0xb2; // Magic number
        header[2] = 0x02; header[3] = 0x00; // Version major
        header[4] = 0x04; header[5] = 0x00; // Version minor
        header[6] = 0x00; header[7] = 0x00; // Zone
        header[8] = 0x00; header[9] = 0x00; // Sigfigs
        header[10] = 0xff; header[11] = 0xff; // Snaplen
        header[12] = 0x01; header[13] = 0x00; // Network (DLT_EN10MB)
        write(fd, header, 24);
    }
    
    close(fd);
    
    // Open the file as a pcap handle
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(filename, errbuf);
    
    return handle;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    int ret;
    const char *device = "any"; // Default device for lookupnet
    
    // Step 1: Prepare environment
    // Create a dummy pcap handle from the input data
    if (Size > 0) {
        handle = create_dummy_pcap(Data, Size);
    } else {
        // If no data, create a minimal file
        handle = create_dummy_pcap(NULL, 0);
    }
    
    // Step 2: Invoke target functions in the specified order
    
    // 1. pcap_close
    if (handle != NULL) {
        pcap_close(handle);
        handle = NULL;
    }
    
    // 2. pcap_lookupnet
    ret = pcap_lookupnet(device, &net, &mask, errbuf);
    (void)ret; // Ignore return value for fuzzing
    
    // 3. pcap_datalink_name_to_val
    // Convert a datalink name to value. We'll use a string from the input if possible.
    // Ensure we have a valid, null-terminated string that stays in scope.
    const char *datalink_name = "EN10MB"; // Default
    if (Size > 0) {
        // Extract a null-terminated string from the input
        size_t name_len = 0;
        while (name_len < Size && Data[name_len] != '\0') {
            name_len++;
        }
        if (name_len > 0 && name_len < 100) {
            // Allocate memory to ensure the string stays valid
            char *temp_name = (char *)malloc(name_len + 1);
            if (temp_name != NULL) {
                memcpy(temp_name, Data, name_len);
                temp_name[name_len] = '\0';
                datalink_name = temp_name;
                
                int dlt_val = pcap_datalink_name_to_val(datalink_name);
                (void)dlt_val; // Ignore return value for fuzzing
                
                free(temp_name);
            }
        }
    } else {
        int dlt_val = pcap_datalink_name_to_val(datalink_name);
        (void)dlt_val; // Ignore return value for fuzzing
    }
    
    // 4. pcap_set_datalink
    // We need a valid handle for this. Create a new one.
    handle = create_dummy_pcap(Data, Size);
    if (handle != NULL) {
        // Try to set datalink to a value from the input or default
        int dlt_to_set = DLT_EN10MB;
        if (Size >= 4) {
            dlt_to_set = *(int *)(Data);
        }
        ret = pcap_set_datalink(handle, dlt_to_set);
        (void)ret; // Ignore return value for fuzzing
    }
    
    // 5. pcap_geterr
    // Get error message from the handle
    if (handle != NULL) {
        const char *err = pcap_geterr(handle);
        (void)err; // Ignore return value for fuzzing
    }
    
    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }
    
    // Remove the dummy file
    unlink("./dummy_file");
    
    return 0;
}