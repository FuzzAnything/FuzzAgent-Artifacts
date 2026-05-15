#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstddef>
#include "magic.h"

namespace harness_008 {
/*
 * Fuzzing harness for libmagic library - ELF Core Dump Parsing
 * Targets: dophn_core function in readelf.c via magic_buffer -> file_buffer -> file_tryelf -> dophn_core
 * Goal: Target the critical coverage gap with 0 hits and 641 undiscovered accumulated branches in ELF core parsing
 * Strategy: Generate synthetic ELF core dump files with PT_NOTE program header segments and various note types
 * Differentiated from previous harnesses: This is a new semantic domain (ELF core dump parsing) not covered by previous harnesses
 * ELF core dump requirements:
 * - ELF header with e_type = ET_CORE (4)
 * - Program headers with p_type = PT_NOTE (4)
 * - Note sections with various note types (NT_PRSTATUS, NT_AUXV, NT_FILE, NT_PRPSINFO, etc.)
 */


// Include libmagic header

// ELF constants from readelf.h
#define EI_NIDENT 16
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ET_CORE 4
#define PT_NOTE 4

// Note types for ET_CORE
#define NT_PRSTATUS 1
#define NT_PRFPREG 2
#define NT_PRPSINFO 3
#define NT_PRXREG 4
#define NT_TASKSTRUCT 4
#define NT_PLATFORM 5
#define NT_AUXV 6

// ELF class
#define ELFCLASS32 1
#define ELFCLASS64 2

// ELF data encoding
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

// Simple ELF structures for core dump generation
struct Elf32_Ehdr {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf64_Ehdr {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf32_Phdr {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};

struct Elf64_Phdr {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

struct Elf32_Nhdr {
    uint32_t n_namesz;
    uint32_t n_descsz;
    uint32_t n_type;
};

struct Elf64_Nhdr {
    uint32_t n_namesz;
    uint32_t n_descsz;
    uint32_t n_type;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need substantial data for ELF structures
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open
    int flags = 0;
    
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Determine ELF core dump generation strategy
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume remaining bytes to build ELF core dump
    std::vector<uint8_t> elf_buffer;
    
    switch (strategy) {
        case 0: {
            // Strategy 0: Simple 32-bit ELF core with minimal PT_NOTE
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(256, 1024);
            elf_buffer.resize(elf_size);
            
            // Fill with random data first
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            if (random_data.size() < elf_size) {
                // Not enough data, use what we have
                elf_size = random_data.size();
                elf_buffer.resize(elf_size);
            }
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Try to make it look like ELF
            if (elf_size >= EI_NIDENT + sizeof(uint16_t) * 2) {
                // Set ELF magic
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
                
                // Set ELF class (32-bit)
                elf_buffer[4] = ELFCLASS32;
                
                // Set data encoding (little endian)
                elf_buffer[5] = ELFDATA2LSB;
                
                // Set ELF version (1)
                elf_buffer[6] = 1;
                
                // Set e_type to ET_CORE (4) at offset 16 (after e_ident)
                if (elf_size >= 18) {
                    // Little endian: ET_CORE = 4
                    elf_buffer[16] = 4;
                    elf_buffer[17] = 0;
                }
                
                // Set e_phnum (number of program headers) - at least 1 for PT_NOTE
                if (elf_size >= 44) {
                    elf_buffer[44] = 1;  // e_phnum LSB
                    elf_buffer[45] = 0;  // e_phnum MSB
                }
                
                // Try to add a PT_NOTE program header if there's enough space
                // Program header offset would be at e_phoff (offset 28)
                if (elf_size >= 32) {
                    // Set e_phoff to point somewhere reasonable
                    uint32_t phoff = 64;  // After ELF header
                    memcpy(&elf_buffer[28], &phoff, 4);
                    
                    // Write a PT_NOTE program header
                    if (elf_size >= phoff + sizeof(Elf32_Phdr)) {
                        Elf32_Phdr phdr;
                        phdr.p_type = PT_NOTE;
                        phdr.p_offset = phoff + sizeof(Elf32_Phdr);  // After program header
                        phdr.p_vaddr = 0;
                        phdr.p_paddr = 0;
                        phdr.p_filesz = 128;  // Size of note data
                        phdr.p_memsz = 128;
                        phdr.p_flags = 4;  // Readable
                        phdr.p_align = 4;
                        
                        memcpy(&elf_buffer[phoff], &phdr, sizeof(phdr));
                        
                        // Write some note data
                        if (elf_size >= phdr.p_offset + 32) {
                            Elf32_Nhdr nhdr;
                            nhdr.n_namesz = 4;  // Size of "CORE" + null
                            nhdr.n_descsz = 16; // Some description data
                            nhdr.n_type = NT_PRSTATUS;
                            
                            memcpy(&elf_buffer[phdr.p_offset], &nhdr, sizeof(nhdr));
                            
                            // Write note name "CORE"
                            if (elf_size >= phdr.p_offset + sizeof(nhdr) + 4) {
                                elf_buffer[phdr.p_offset + sizeof(nhdr)] = 'C';
                                elf_buffer[phdr.p_offset + sizeof(nhdr) + 1] = 'O';
                                elf_buffer[phdr.p_offset + sizeof(nhdr) + 2] = 'R';
                                elf_buffer[phdr.p_offset + sizeof(nhdr) + 3] = 'E';
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: 64-bit ELF core with multiple PT_NOTE segments
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(512, 2048);
            elf_buffer.resize(elf_size);
            
            // Fill with random data
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            if (random_data.size() < elf_size) {
                elf_size = random_data.size();
                elf_buffer.resize(elf_size);
            }
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Try to make it look like 64-bit ELF
            if (elf_size >= EI_NIDENT + sizeof(uint16_t) * 2) {
                // Set ELF magic
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
                
                // Set ELF class (64-bit)
                elf_buffer[4] = ELFCLASS64;
                
                // Set data encoding
                elf_buffer[5] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
                
                // Set ELF version
                elf_buffer[6] = 1;
                
                // Set e_type to ET_CORE
                if (elf_size >= 18) {
                    elf_buffer[16] = ET_CORE;
                    elf_buffer[17] = 0;
                }
                
                // Set multiple program headers
                uint8_t num_phdrs = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
                if (elf_size >= 56) {
                    elf_buffer[56] = num_phdrs;  // e_phnum LSB (offset 56 in 64-bit)
                    elf_buffer[57] = 0;          // e_phnum MSB
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Malformed/edge case ELF core
            // Create data that might trigger error paths in dophn_core
            size_t elf_size = fdp.ConsumeIntegralInRange<size_t>(64, 512);
            elf_buffer.resize(elf_size);
            
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(elf_size);
            if (random_data.size() < elf_size) {
                elf_size = random_data.size();
                elf_buffer.resize(elf_size);
            }
            memcpy(elf_buffer.data(), random_data.data(), elf_size);
            
            // Set ELF magic but potentially corrupt other fields
            if (elf_size >= 4) {
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
            }
            
            // Randomly set some fields to trigger different code paths
            if (elf_size >= 16) {
                // Random e_type - might be ET_CORE or something else
                elf_buffer[16] = fdp.ConsumeIntegral<uint8_t>();
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Direct consumption as ELF-like buffer
            // Let libmagic's ELF parser handle whatever we give it
            elf_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            
            // Ensure it has ELF magic if there's enough data
            if (elf_buffer.size() >= 4) {
                elf_buffer[0] = ELFMAG0;
                elf_buffer[1] = ELFMAG1;
                elf_buffer[2] = ELFMAG2;
                elf_buffer[3] = ELFMAG3;
            }
            break;
        }
    }
    
    // Test the generated ELF core dump with magic_buffer
    if (!elf_buffer.empty()) {
        const char* result = magic_buffer(magic, elf_buffer.data(), elf_buffer.size());
        // Result can be NULL - we're testing the parsing path, not the output
        (void)result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_008
