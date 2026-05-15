// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static int read_int_from_data(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(int)) {
        int value;
        memcpy(&value, *data, sizeof(int));
        *data += sizeof(int);
        *size -= sizeof(int);
        return value;
    }
    return 0;
}

static const char* read_string_from_data(const uint8_t **data, size_t *size, char *buffer, size_t buffer_size) {
    if (buffer_size == 0) return NULL;
    
    size_t i = 0;
    while (i < buffer_size - 1 && i < *size && (*data)[i] != 0) {
        buffer[i] = (*data)[i];
        i++;
    }
    buffer[i] = '\0';
    
    size_t advance = i + 1;
    if (advance > *size) advance = *size;
    
    *data += advance;
    *size -= advance;
    
    return buffer;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const uint8_t *cursor = Data;
    size_t remaining = Size;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    char source_buf[256];
    char device_buf[256];
    
    // First pcap_statustostr
    int status1 = read_int_from_data(&cursor, &remaining);
    const char *str1 = pcap_statustostr(status1);
    (void)str1;
    
    // Create a pcap handle for testing
    pcap_t *pcap = pcap_create(NULL, errbuf);
    if (!pcap) {
        // Try with a dummy device
        pcap = pcap_create("any", errbuf);
    }
    
    if (pcap) {
        // pcap_set_timeout
        int timeout = read_int_from_data(&cursor, &remaining);
        pcap_set_timeout(pcap, timeout);
        
        // Second pcap_statustostr
        int status2 = read_int_from_data(&cursor, &remaining);
        const char *str2 = pcap_statustostr(status2);
        (void)str2;
        
        // pcap_set_buffer_size
        int buffer_size = read_int_from_data(&cursor, &remaining);
        pcap_set_buffer_size(pcap, buffer_size);
        
        // Third pcap_statustostr
        int status3 = read_int_from_data(&cursor, &remaining);
        const char *str3 = pcap_statustostr(status3);
        (void)str3;
        
        // pcap_activate
        pcap_activate(pcap);
        
        // Fourth pcap_statustostr
        int status4 = read_int_from_data(&cursor, &remaining);
        const char *str4 = pcap_statustostr(status4);
        (void)str4;
        
        // First pcap_geterr
        char *err1 = pcap_geterr(pcap);
        (void)err1;
        
        // Fifth pcap_statustostr
        int status5 = read_int_from_data(&cursor, &remaining);
        const char *str5 = pcap_statustostr(status5);
        (void)str5;
        
        // Second pcap_geterr
        char *err2 = pcap_geterr(pcap);
        (void)err2;
        
        // pcap_close the first handle
        pcap_close(pcap);
    }
    
    // pcap_open
    const char *source = read_string_from_data(&cursor, &remaining, source_buf, sizeof(source_buf));
    int snaplen1 = read_int_from_data(&cursor, &remaining);
    int flags = read_int_from_data(&cursor, &remaining);
    int read_timeout = read_int_from_data(&cursor, &remaining);
    
    // Create dummy file for pcap_open if needed
    write_dummy_file(cursor, remaining > 100 ? 100 : remaining);
    
    pcap_t *pcap_open_handle = pcap_open(source, snaplen1, flags, read_timeout, NULL, errbuf);
    if (pcap_open_handle) {
        pcap_close(pcap_open_handle);
    }
    
    // pcap_open_live
    const char *device = read_string_from_data(&cursor, &remaining, device_buf, sizeof(device_buf));
    int snaplen2 = read_int_from_data(&cursor, &remaining);
    int promisc = read_int_from_data(&cursor, &remaining);
    int timeout2 = read_int_from_data(&cursor, &remaining);
    
    pcap_t *pcap_live_handle = pcap_open_live(device, snaplen2, promisc, timeout2, errbuf);
    if (pcap_live_handle) {
        pcap_close(pcap_live_handle);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}