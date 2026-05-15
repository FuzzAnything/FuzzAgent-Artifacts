#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "magic.h"

namespace harness_015 {
/*
 * Fuzzing harness for libmagic library - ELF Core File Parsing
 * Targets: dophn_core (641 blocked branches), donote (591 blocked branches), 
 *          do_core_note (145 blocked branches), do_auxv_note (135 blocked branches),
 *          file_tryelf (1051 blocked branches, 27.8% coverage)
 * Focus: ELF core file parsing with PT_NOTE program header segments containing various NOTE types
 * Goal: Generate synthetic ELF core files (ELFCLASS32/ELFCLASS64) with PT_NOTE segments
 *        containing NT_AUXV, NT_FILE, NT_PRSTATUS, NT_PRPSINFO, etc.
 * Edge cases: corrupted but parsable ELF headers, varying endianness (little/big),
 *        different architectures (x86, ARM, etc.), malformed note sections
 * Semantic differentiation: Existing harnesses test general file detection;
 *        this harness exclusively targets ELF core file parsing with synthetic
 *        ELF structure generation
 * Strategy: Use FuzzedDataProvider to generate valid and malformed ELF core structures,
 *        test through magic_buffer API with MAGIC_CONTINUE and MAGIC_MIME flags
 * Expected coverage gain: Target the 641+ blocked branches in dophn_core and 
 *        hundreds more in related ELF functions
 */


// Include libmagic header

// ELF structure definitions (from readelf.h)
#define EI_NIDENT 16
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
#define ET_CORE 4
#define PT_NOTE 4
#define NT_PRSTATUS 1
#define NT_PRFPREG 2
#define NT_PRPSINFO 3
#define NT_TASKSTRUCT 4
#define NT_PLATFORM 5
#define NT_AUXV 6
#define NT_FILE 0x46494c45  // "FILE" in hex

// Simplified ELF structures for generation
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

typedef struct {
    uint32_t n_namesz;
    uint32_t n_descsz;
    uint32_t n_type;
} Elf64_Nhdr;

// Generate ELF header with configurable parameters
void generate_elf_header(FuzzedDataProvider& fdp, Elf64_Ehdr& header, 
                         uint8_t elf_class, uint8_t endianness, bool generate_valid) {
    // Initialize ident array
    memset(header.e_ident, 0, EI_NIDENT);
    header.e_ident[0] = 0x7F;
    header.e_ident[1] = 'E';
    header.e_ident[2] = 'L';
    header.e_ident[3] = 'F';
    header.e_ident[4] = elf_class;  // ELFCLASS32 or ELFCLASS64
    header.e_ident[5] = endianness; // ELFDATA2LSB or ELFDATA2MSB
    header.e_ident[6] = 1;  // Version
    
    if (generate_valid && fdp.ConsumeBool()) {
        header.e_type = ET_CORE;
    } else {
        header.e_type = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Machine type - common architectures
    uint8_t machine_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    switch (machine_choice) {
        case 0: header.e_machine = 3; break;   // EM_386
        case 1: header.e_machine = 62; break;  // EM_X86_64
        case 2: header.e_machine = 40; break;  // EM_ARM
        case 3: header.e_machine = 183; break; // EM_AARCH64
        case 4: header.e_machine = 8; break;   // EM_MIPS
        case 5: header.e_machine = fdp.ConsumeIntegral<uint16_t>(); break; // Random
    }
    
    header.e_version = 1;
    header.e_entry = 0;
    
    // Program header offset - sometimes corrupted
    if (generate_valid && fdp.ConsumeBool()) {
        header.e_phoff = sizeof(Elf64_Ehdr);
    } else {
        header.e_phoff = fdp.ConsumeIntegral<uint64_t>();
    }
    
    header.e_shoff = 0;  // No section headers for core files
    header.e_flags = 0;
    header.e_ehsize = sizeof(Elf64_Ehdr);
    
    // Program header size
    if (generate_valid && fdp.ConsumeBool()) {
        header.e_phentsize = sizeof(Elf64_Phdr);
    } else {
        header.e_phentsize = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Number of program headers
    if (generate_valid && fdp.ConsumeBool()) {
        header.e_phnum = 1;  // At least one PT_NOTE
    } else {
        header.e_phnum = fdp.ConsumeIntegral<uint16_t>();
    }
    
    header.e_shentsize = 0;
    header.e_shnum = 0;
    header.e_shstrndx = 0;
}

// Generate PT_NOTE program header
void generate_note_phdr(FuzzedDataProvider& fdp, Elf64_Phdr& phdr, 
                        size_t note_offset, size_t note_size, bool generate_valid) {
    if (generate_valid && fdp.ConsumeBool()) {
        phdr.p_type = PT_NOTE;
    } else {
        phdr.p_type = fdp.ConsumeIntegral<uint32_t>();
    }
    
    phdr.p_flags = 4;  // Read-only
    phdr.p_offset = note_offset;
    phdr.p_vaddr = 0;
    phdr.p_paddr = 0;
    phdr.p_filesz = note_size;
    phdr.p_memsz = note_size;
    phdr.p_align = 4;
}

// Generate note header with specific type
void generate_note(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer,
                   uint32_t note_type, const std::string& name, bool generate_valid) {
    Elf64_Nhdr nhdr;
    
    if (generate_valid && fdp.ConsumeBool()) {
        nhdr.n_namesz = name.size() + 1;  // Include null terminator
        nhdr.n_descsz = fdp.ConsumeIntegralInRange<uint32_t>(4, 256);
        nhdr.n_type = note_type;
    } else {
        nhdr.n_namesz = fdp.ConsumeIntegral<uint32_t>();
        nhdr.n_descsz = fdp.ConsumeIntegral<uint32_t>();
        nhdr.n_type = fdp.ConsumeIntegral<uint32_t>();
    }
    
    // Write note header
    size_t offset = buffer.size();
    buffer.resize(offset + sizeof(Elf64_Nhdr));
    memcpy(buffer.data() + offset, &nhdr, sizeof(Elf64_Nhdr));
    
    // Write note name (padded to 4-byte alignment)
    offset += sizeof(Elf64_Nhdr);
    size_t name_len = nhdr.n_namesz;
    buffer.resize(offset + name_len);
    if (name_len > 0) {
        size_t copy_len = std::min(name.size(), name_len - 1);
        if (copy_len > 0) {
            memcpy(buffer.data() + offset, name.c_str(), copy_len);
        }
        // Null terminate if space
        if (copy_len < name_len) {
            buffer[offset + copy_len] = '\0';
        }
    }
    
    // Align to 4 bytes
    offset += name_len;
    size_t padding = (4 - (offset % 4)) % 4;
    buffer.resize(offset + padding);
    offset += padding;
    
    // Write note descriptor (random data for fuzzing)
    size_t desc_len = nhdr.n_descsz;
    buffer.resize(offset + desc_len);
    std::vector<uint8_t> desc_data = fdp.ConsumeBytes<uint8_t>(desc_len);
    if (desc_data.size() < desc_len) {
        // Pad with zeros if not enough data
        memset(buffer.data() + offset + desc_data.size(), 0, desc_len - desc_data.size());
    }
    if (!desc_data.empty()) {
        memcpy(buffer.data() + offset, desc_data.data(), desc_data.size());
    }
    
    // Align to 4 bytes
    offset += desc_len;
    padding = (4 - (offset % 4)) % 4;
    buffer.resize(offset + padding);
}

// Generate ELF core file with various note types
std::vector<uint8_t> generate_elf_core(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> elf_data;
    
    // Determine ELF parameters from fuzzer input
    uint8_t elf_class = fdp.ConsumeBool() ? ELFCLASS32 : ELFCLASS64;
    uint8_t endianness = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
    bool generate_valid = fdp.ConsumeBool();
    
    // Generate ELF header
    Elf64_Ehdr header;
    generate_elf_header(fdp, header, elf_class, endianness, generate_valid);
    
    // Write ELF header
    size_t header_size = sizeof(Elf64_Ehdr);
    elf_data.resize(header_size);
    memcpy(elf_data.data(), &header, header_size);
    
    // Generate note section
    std::vector<uint8_t> note_section;
    
    // Determine which note types to include
    uint8_t note_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // NT_PRSTATUS - process status
    if (note_mask & 0x01) {
        generate_note(fdp, note_section, NT_PRSTATUS, "CORE", generate_valid);
    }
    
    // NT_PRPSINFO - process info
    if (note_mask & 0x02) {
        generate_note(fdp, note_section, NT_PRPSINFO, "CORE", generate_valid);
    }
    
    // NT_AUXV - auxiliary vector
    if (note_mask & 0x04) {
        generate_note(fdp, note_section, NT_AUXV, "CORE", generate_valid);
    }
    
    // NT_FILE - mapped files
    if (note_mask & 0x08) {
        generate_note(fdp, note_section, NT_FILE, "CORE", generate_valid);
    }
    
    // Random note type
    if (note_mask & 0x10) {
        uint32_t random_type = fdp.ConsumeIntegral<uint32_t>();
        std::string name = fdp.ConsumeRandomLengthString(16);
        generate_note(fdp, note_section, random_type, name, generate_valid);
    }
    
    // Add more notes if space allows
    while (fdp.remaining_bytes() > 100 && note_section.size() < 4096) {
        uint32_t type = fdp.ConsumeIntegral<uint32_t>();
        std::string name = fdp.ConsumeRandomLengthString(8);
        generate_note(fdp, note_section, type, name, false); // Allow malformed
    }
    
    // Generate program header pointing to note section
    size_t phdr_offset = elf_data.size();
    Elf64_Phdr phdr;
    generate_note_phdr(fdp, phdr, header_size, note_section.size(), generate_valid);
    
    elf_data.resize(phdr_offset + sizeof(Elf64_Phdr));
    memcpy(elf_data.data() + phdr_offset, &phdr, sizeof(Elf64_Phdr));
    
    // Append note section
    size_t note_offset = elf_data.size();
    elf_data.resize(note_offset + note_section.size());
    if (!note_section.empty()) {
        memcpy(elf_data.data() + note_offset, note_section.data(), note_section.size());
    }
    
    return elf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 64) {
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
        return 0;
    }
    
    // Try to load magic database
    const char* magic_path = NULL;
    if (magic_load(magic, magic_path) == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Generate ELF core file from remaining fuzzer input
    std::vector<uint8_t> elf_core = generate_elf_core(fdp);
    
    if (!elf_core.empty()) {
        // Test with magic_buffer
        const char* result = magic_buffer(magic, elf_core.data(), elf_core.size());
        (void)result; // Result can be NULL, that's OK
        
        // Also test with different flags
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            // Try with MIME flag
            magic_setflags(magic, MAGIC_MIME_TYPE);
            result = magic_buffer(magic, elf_core.data(), elf_core.size());
            (void)result;
            
            // Try with CONTINUE flag
            magic_setflags(magic, MAGIC_CONTINUE);
            result = magic_buffer(magic, elf_core.data(), elf_core.size());
            (void)result;
        }
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_015
