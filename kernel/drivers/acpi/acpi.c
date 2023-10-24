#include <stddef.h>
#include <stdbool.h>

#include <lib/printf.h>
#include <lib/check.h>
#include <lib/string.h>

#include <boot/limine/limine.h>

#include <drivers/acpi/acpi.h>

volatile struct limine_rsdp_request rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};
struct rsdp_t* rsdp = NULL;

struct xsdt_t {
    struct sdt_t sdt;
    uint64_t sdt_pointers[];
} __attribute__((packed));

struct xsdt_t* xsdt = NULL;

static uint8_t rsdp_calculate_checksum(const void *data, size_t length) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint8_t checksum = 0;
    
    for (size_t i = 0; i < length; i++) {
        checksum += bytes[i];
    }
    
    return checksum;
}

static bool is_table_valid(struct sdt_t* table_header) {
    unsigned char sum = 0;
 
    for (uint32_t i = 0; i < table_header->length; i++) {
        sum += ((char *)table_header)[i];
    }
 
    return sum == 0;
}

void* find_tables(const char signature[4]) {
    for (uint64_t x = 0; x < (xsdt->sdt.length - sizeof(xsdt->sdt)) / 8; x++) {
        struct sdt_t* table = (struct sdt_t*)xsdt->sdt_pointers[x];
        if (strncmp((const char*)&table->signature, signature, 4)) {return (void*)table;}
    }
    return NULL;
}

int acpi_init(void) {
    CHECK((rsdp_req.response == NULL));
    rsdp = (struct rsdp_t*)rsdp_req.response->address;

    uint8_t rsdp_checksum = rsdp_calculate_checksum(rsdp, sizeof(struct rsdp_t));
    CHECK(rsdp_checksum != 0);

    uint8_t ext_checksum = rsdp_calculate_checksum(&rsdp->length, sizeof(struct rsdp_t) - offsetof(struct rsdp_t, length));
    CHECK(ext_checksum != 0);

    print("ACPI OEM ID: %s", rsdp->OEM_ID);

    CHECK(rsdp->revision != 2);
    print("ACPI Version: %u", rsdp->revision);

    xsdt = (struct xsdt_t*)rsdp->xsdt;
    CHECK(is_table_valid(&xsdt->sdt) == false);
    print_text("Valid XSDT table found");

    for (uint64_t x = 0; x < (xsdt->sdt.length - sizeof(xsdt->sdt)) / 8; x++) {
        struct sdt_t* table = (struct sdt_t*)xsdt->sdt_pointers[x];
        CHECK(is_table_valid(table) == false);
        char table_type[5];
        memcpy(&table_type, &table->signature, 4);
        table_type[4] = '\0';
        print("Valid %s table found", table_type);
    }
    
    return 0;
}
