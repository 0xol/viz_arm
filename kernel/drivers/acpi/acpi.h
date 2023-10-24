#ifndef DRIVERS_ACPI_ACPI_H
#define DRIVERS_ACPI_ACPI_H

#include <stdint.h>

struct rsdp_t {
    int8_t signature[8];
    uint8_t checksum;
    int8_t OEM_ID[6];
    uint8_t revision;
    uint32_t reserved_1;//32 bit address, but kernel is 64 bit only
    uint32_t length;
    uint64_t xsdt;
    uint8_t extended_checksum;
    int8_t reserved_2[3];
} __attribute__((packed));

struct sdt_t {
    int8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    int8_t OEM_ID[6];
    int8_t OEM_table_ID[8];
    uint32_t OEM_revision;
    uint32_t creator_ID;
    uint32_t creator_revision;
} __attribute__((packed));

int acpi_init(void);

#endif