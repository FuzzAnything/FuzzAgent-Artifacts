// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
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

static int extract_int(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(int)) {
        int value;
        memcpy(&value, *data, sizeof(int));
        *data += sizeof(int);
        *size -= sizeof(int);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    const uint8_t *data_ptr = Data;
    size_t data_size = Size;
    
    write_dummy_file(Data, Size);
    
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return 0;
    }
    
    pcap_geterr(pcap);
    
    int *dlt_list = NULL;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    
    int status_code = extract_int(&data_ptr, &data_size);
    pcap_statustostr(status_code);
    
    pcap_geterr(pcap);
    
    int dlt_value = extract_int(&data_ptr, &data_size);
    pcap_datalink_val_to_name(dlt_value);
    
    if (dlt_count > 0 && dlt_list) {
        pcap_free_datalinks(dlt_list);
    }
    
    pcap_close(pcap);
    unlink("./dummy_file");
    
    return 0;
}