#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "magic.h"

namespace harness_025 {
/*
 * Fuzzing harness for libmagic library - ELF File Parsing Internals
 * Targets: dophn_core(), donote() in readelf.c (0 hits, 1226+ undiscovered branches)
 * MODULE: readelf.c (ELF parsing module) - Critical coverage gap
 * STRATEGY: Target ELF file format fuzzing with diverse ELF file structures
 *           (executables, shared libraries, core dumps, object files) to reach
 *           uncovered ELF parsing logic
 * DIFFERENTIATION: Focus on deep ELF parsing internals rather than high-level API calls;
 *           create semantic diversity from existing 25 harnesses by targeting
 *           ELF-specific parsing paths, particularly PT_NOTE sections that trigger
 *           dophn_core() and donote() functions
 * API SEQUENCE: magic_open() -> magic_buffer() with ELF file data -> magic_close()
 * Goal: Reach untouched ELF parsing functions with 0% coverage, particularly
 *       dophn_core() and donote() in readelf.c with 1226+ undiscovered branches
 */


// Include libmagic header

// ELF Constants
#define EI_NIDENT 16
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define EM_NONE 0
#define EM_386 3
#define EM_X86_64 62
#define EM_ARM 40
#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7

// ELF Structures
typedef struct {
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
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

// Note header structure
typedef struct {
    uint32_t n_namesz;
    uint32_t n_descsz;
    uint32_t n_type;
} Elf64_Nhdr;

// Helper function to generate ELF header
std::vector<uint8_t> generate_elf_header(FuzzedDataProvider& fdp, bool is_64bit, bool is_core) {
    std::vector<uint8_t> header;
    
    if (is_64bit) {
        header.resize(sizeof(Elf64_Ehdr));
        Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(header.data());
        
        // ELF identification
        ehdr->e_ident[0] = 0x7f;
        ehdr->e_ident[1] = 'E';
        ehdr->e_ident[2] = 'L';
        ehdr->e_ident[3] = 'F';
        ehdr->e_ident[4] = is_64bit ? ELFCLASS64 : ELFCLASS32;
        ehdr->e_ident[5] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
        ehdr->e_ident[6] = 1; // EV_CURRENT
        ehdr->e_ident[7] = 0; // ELFOSABI_NONE
        
        // Fill remaining ident bytes
        for (int i = 8; i < EI_NIDENT; i++) {
            ehdr->e_ident[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // ELF type
        if (is_core) {
            ehdr->e_type = ET_CORE;
        } else {
            uint8_t elf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (elf_type) {
                case 0: ehdr->e_type = ET_NONE; break;
                case 1: ehdr->e_type = ET_REL; break;
                case 2: ehdr->e_type = ET_EXEC; break;
                case 3: ehdr->e_type = ET_DYN; break;
            }
        }
        
        // Machine type
        uint8_t machine = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (machine) {
            case 0: ehdr->e_machine = EM_NONE; break;
            case 1: ehdr->e_machine = EM_386; break;
            case 2: ehdr->e_machine = EM_X86_64; break;
            case 3: ehdr->e_machine = EM_ARM; break;
        }
        
        ehdr->e_version = 1; // EV_CURRENT
        ehdr->e_entry = fdp.ConsumeIntegral<uint64_t>();
        ehdr->e_phoff = sizeof(Elf64_Ehdr); // Program header table offset
        ehdr->e_shoff = 0; // Section header table offset (optional)
        ehdr->e_flags = fdp.ConsumeIntegral<uint32_t>();
        ehdr->e_ehsize = sizeof(Elf64_Ehdr);
        ehdr->e_phentsize = sizeof(Elf64_Phdr);
        ehdr->e_phnum = fdp.ConsumeIntegralInRange<uint16_t>(0, 10); // Number of program headers
        ehdr->e_shentsize = 0; // Section header entry size
        ehdr->e_shnum = 0; // Number of section headers
        ehdr->e_shstrndx = 0; // Section header string table index
    } else {
        // 32-bit ELF header (simplified)
        header.resize(52); // Size of Elf32_Ehdr
        // Minimal 32-bit ELF header
        header[0] = 0x7f;
        header[1] = 'E';
        header[2] = 'L';
        header[3] = 'F';
        header[4] = ELFCLASS32;
        header[5] = ELFDATA2LSB;
        header[6] = 1;
    }
    
    return header;
}

// Helper function to generate program headers with PT_NOTE sections
std::vector<uint8_t> generate_program_headers(FuzzedDataProvider& fdp, bool is_64bit, uint16_t num_headers, bool include_note_sections) {
    std::vector<uint8_t> phdrs;
    
    size_t phdr_size = is_64bit ? sizeof(Elf64_Phdr) : 32; // 32-bit phdr size
    phdrs.resize(num_headers * phdr_size);
    
    uint8_t* ptr = phdrs.data();
    uint64_t current_offset = 128; // Start after ELF header
    
    for (uint16_t i = 0; i < num_headers; i++) {
        if (is_64bit) {
            Elf64_Phdr* phdr = reinterpret_cast<Elf64_Phdr*>(ptr);
            
            // Determine program header type
            if (include_note_sections && (i == 0 || fdp.ConsumeBool())) {
                phdr->p_type = PT_NOTE; // Critical for dophn_core()
            } else {
                uint8_t ptype = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (ptype) {
                    case 0: phdr->p_type = PT_NULL; break;
                    case 1: phdr->p_type = PT_LOAD; break;
                    case 2: phdr->p_type = PT_DYNAMIC; break;
                    case 3: phdr->p_type = PT_INTERP; break;
                    case 4: phdr->p_type = PT_NOTE; break;
                    case 5: phdr->p_type = PT_SHLIB; break;
                    case 6: phdr->p_type = PT_PHDR; break;
                    case 7: phdr->p_type = PT_TLS; break;
                }
            }
            
            phdr->p_flags = fdp.ConsumeIntegral<uint32_t>();
            phdr->p_offset = current_offset;
            phdr->p_vaddr = fdp.ConsumeIntegral<uint64_t>();
            phdr->p_paddr = fdp.ConsumeIntegral<uint64_t>();
            
            // For PT_NOTE sections, generate reasonable sizes
            if (phdr->p_type == PT_NOTE) {
                phdr->p_filesz = fdp.ConsumeIntegralInRange<uint64_t>(16, 1024);
                phdr->p_memsz = phdr->p_filesz;
            } else {
                phdr->p_filesz = fdp.ConsumeIntegral<uint64_t>();
                phdr->p_memsz = fdp.ConsumeIntegral<uint64_t>();
            }
            
            phdr->p_align = fdp.ConsumeIntegral<uint64_t>();
            
            current_offset += phdr->p_filesz;
            ptr += sizeof(Elf64_Phdr);
        } else {
            // Simplified 32-bit program header
            // p_type (4 bytes)
            uint32_t p_type = include_note_sections ? PT_NOTE : PT_LOAD;
            memcpy(ptr, &p_type, 4);
            ptr += 4;
            
            // p_offset (4 bytes)
            memcpy(ptr, &current_offset, 4);
            ptr += 4;
            
            // p_vaddr, p_paddr, p_filesz, p_memsz, p_flags, p_align
            for (int j = 0; j < 6; j++) {
                uint32_t val = fdp.ConsumeIntegral<uint32_t>();
                memcpy(ptr, &val, 4);
                ptr += 4;
            }
            
            current_offset += 256; // Simple increment
        }
    }
    
    return phdrs;
}

// Helper function to generate PT_NOTE section data (triggers donote())
std::vector<uint8_t> generate_note_section(FuzzedDataProvider& fdp, bool is_64bit) {
    std::vector<uint8_t> note_data;
    
    // Generate 1-4 note entries
    size_t num_notes = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    for (size_t i = 0; i < num_notes && fdp.remaining_bytes() > 16; i++) {
        // Note header
        Elf64_Nhdr nhdr;
        nhdr.n_namesz = fdp.ConsumeIntegralInRange<uint32_t>(1, 32);
        nhdr.n_descsz = fdp.ConsumeIntegralInRange<uint32_t>(0, 256);
        nhdr.n_type = fdp.ConsumeIntegral<uint32_t>();
        
        // Add note header to data
        size_t current_size = note_data.size();
        note_data.resize(current_size + sizeof(Elf64_Nhdr));
        memcpy(note_data.data() + current_size, &nhdr, sizeof(Elf64_Nhdr));
        
        // Add name (padded to 4-byte alignment)
        if (nhdr.n_namesz > 0) {
            size_t name_size = std::min(nhdr.n_namesz, static_cast<uint32_t>(fdp.remaining_bytes()));
            std::vector<uint8_t> name_data = fdp.ConsumeBytes<uint8_t>(name_size);
            note_data.insert(note_data.end(), name_data.begin(), name_data.end());
            
            // Padding
            size_t padding = (4 - (name_size % 4)) % 4;
            for (size_t p = 0; p < padding && fdp.remaining_bytes() > 0; p++) {
                note_data.push_back(0);
            }
        }
        
        // Add description (padded to 4-byte alignment)
        if (nhdr.n_descsz > 0) {
            size_t desc_size = std::min(nhdr.n_descsz, static_cast<uint32_t>(fdp.remaining_bytes()));
            std::vector<uint8_t> desc_data = fdp.ConsumeBytes<uint8_t>(desc_size);
            note_data.insert(note_data.end(), desc_data.begin(), desc_data.end());
            
            // Padding
            size_t padding = (4 - (desc_size % 4)) % 4;
            for (size_t p = 0; p < padding && fdp.remaining_bytes() > 0; p++) {
                note_data.push_back(0);
            }
        }
    }
    
    return note_data;
}

// Helper function to generate ELF file with various structures
std::vector<uint8_t> generate_elf_file(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> elf_data;
    
    // Determine ELF characteristics
    bool is_64bit = fdp.ConsumeBool();
    bool is_core = fdp.ConsumeBool();
    bool include_notes = fdp.ConsumeBool();
    
    // Generate ELF header
    std::vector<uint8_t> header = generate_elf_header(fdp, is_64bit, is_core);
    elf_data.insert(elf_data.end(), header.begin(), header.end());
    
    // Get number of program headers from header (if we can parse it)
    uint16_t num_phdrs = 0;
    if (is_64bit && header.size() >= sizeof(Elf64_Ehdr)) {
        Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(header.data());
        num_phdrs = ehdr->e_phnum;
    } else {
        num_phdrs = fdp.ConsumeIntegralInRange<uint16_t>(0, 5);
    }
    
    // Generate program headers
    if (num_phdrs > 0) {
        std::vector<uint8_t> phdrs = generate_program_headers(fdp, is_64bit, num_phdrs, include_notes);
        elf_data.insert(elf_data.end(), phdrs.begin(), phdrs.end());
    }
    
    // Generate section data for each program header
    // For simplicity, we'll generate some generic section data
    size_t remaining_bytes = fdp.remaining_bytes();
    size_t section_data_size = std::min(remaining_bytes, static_cast<size_t>(1024));
    
    if (section_data_size > 0) {
        std::vector<uint8_t> section_data = fdp.ConsumeBytes<uint8_t>(section_data_size);
        elf_data.insert(elf_data.end(), section_data.begin(), section_data.end());
        
        // If we have note sections, add them
        if (include_notes && fdp.remaining_bytes() > 0) {
            std::vector<uint8_t> note_data = generate_note_section(fdp, is_64bit);
            elf_data.insert(elf_data.end(), note_data.begin(), note_data.end());
        }
    }
    
    return elf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic ELF generation
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on ELF-related flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
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
    
    // Generate ELF file data with various structures to trigger dophn_core() and donote()
    // This is the critical part that targets the uncovered ELF parsing functions
    std::vector<uint8_t> elf_data = generate_elf_file(fdp);
    
    if (!elf_data.empty()) {
        // Use magic_buffer to process the ELF data
        // This will trigger the ELF parsing code in readelf.c, including
        // dophn_core() for core files and donote() for PT_NOTE sections
        const char* result = magic_buffer(magic, elf_data.data(), elf_data.size());
        // Result can be NULL for malformed ELF files, that's OK for fuzzing
        (void)result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_025
