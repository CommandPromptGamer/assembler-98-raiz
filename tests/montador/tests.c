
#include <criterion/criterion.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "montador.h"

Test(section_suite, test_getSection_multiple_sections_with_newlines) {
    
    char file_t[] = 
    "       omadmosasmodao masdoas asdaosd \n"
    " msoadamodsad modso msomdam asodmasdo am\n"
    "section .text\n"
    "  12 32 42 02 932 19\n"
    " 23 239 021 023 923 91\n"
    " Some random text here\n"
    " Another line in .text section\n"
    "section .data\n"
    " 0x00 0x01 0x02 0x03 0x04\n"
    " 0x05 0x06 0x07 0x08 0x09\n"
    " More data section content\n"
    "section .rodata\n"
    " \"Hello, world!\"\n"
    " \"Another string constant\"\n";

    FILE *f = fopen("test", "w");
    fprintf(f, "%s", file_t);
    cr_assert(f != NULL);
    fclose(f);

    f = fopen("test", "r");
    cr_assert(f != NULL);

    section_t *section_data = getSection(f, ".data");  
    char expected_data_content[] = 
    " 0x00 0x01 0x02 0x03 0x04\n"
    " 0x05 0x06 0x07 0x08 0x09\n";

    cr_assert_str_eq(expected_data_content,
    section_data->section_content,
    "Expected .data content: %s,\nGot: %s\n", expected_data_content, section_data->section_content);

    cr_assert_eq(strlen(expected_data_content),
    section_data->section_size,
    "Expected .data size: %lu,\nGot: %lu\n", strlen(expected_data_content), section_data->section_size );


    section_t *section_text = getSection(f, ".text");  
    char expected_text_content[] = 
    "  12 32 42 02 932 19\n"
    " 23 239 021 023 923 91\n"
    " Some random text here\n";

    cr_assert_str_eq(expected_text_content,
    section_text->section_content,
    "Expected .text content: %s,\nGot: %s\n", expected_text_content, section_text->section_content);

    cr_assert_eq(strlen(expected_text_content),
    section_text->section_size,
    "Expected .text size: %lu,\nGot: %lu\n", strlen(expected_text_content), section_text->section_size );

    cr_assert_str_eq(section_text->section_title, ".text");
    cr_assert_eq(section_text->head, 0);
    cr_assert_eq(section_text->tokens, NULL);

    section_t *section_rodata = getSection(f, ".rodata");  
    char expected_rodata_content[] = 
    " \"Hello, world!\"\n"
    " \"Another string constant\"\n";

    cr_assert_str_eq(expected_rodata_content,
    section_rodata->section_content,
    "Expected .rodata content: %s,\nGot: %s\n", expected_rodata_content, section_rodata->section_content);

    cr_assert_eq(strlen(expected_rodata_content),
    section_rodata->section_size,
    "Expected .rodata size: %lu,\nGot: %lu\n", strlen(expected_rodata_content), section_rodata->section_size );

    remove("test");
    fclose(f);
    free(section_text->section_content);
    free(section_text);
    free(section_data->section_content);
    free(section_data);
    free(section_rodata->section_content);
    free(section_rodata);
}

