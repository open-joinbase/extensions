#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/// Columns, is the array of the bytes-represented columns. That is, you split a line string into an array of column values in the string's bytes form. JoinBase later will convert the column to its own type data for storage.
///
/// Contract:
/// 1. bytes is the array of byte array for every column, lens is the uint32_t array of the len of every column in bytes. The order of arrays is important. The order must be same to the column order specified in the schema. NOTE: we use byte array rather than null-terminated C string.
/// 2. The size_rows field of `Columns` indicates the length of rows in the `Columns`. The total length of columns is `size_rows` multiples `size_columns` which is provided in the parameters of API call. For size_rows > 1, you have multiple rows put into the `Columns`.
/// 3. status_code: if status_code < 0, then the value is interpreted as the error code, the log will print this error code in positive number for debugging; if status_code==0, the udm function return normally and the return will be checked and used.
/// 4. ext_ptr1 and ext_ptr1 are pointers which could be used to keep some malloc-ed pointers and freed in the udm_free when necessary
/// 5. it is your responsibilty to provide a valid Columns representation. The violation will cause the undefined behavior(UB).
typedef struct Columns
{
    uint8_t **bytes;
    int32_t *lens_or_typs;
    void *ext_ptr1;
    void *ext_ptr2;
    uint32_t size_rows;
    int8_t status_code;
} Columns;

/// @brief the method to free all kinds of resources allocated in your custom mapping function. The signature of this function must be same to this one in this example to make invocation successfully.
/// @param cols seen in the above Columns struct spec
void udm_free(Columns *cols)
{
    free((void *)cols->bytes);
    free((void *)cols->lens_or_typs);
}

/// @brief the example UDM: test_msg_to_cols. The name of output dynamic library is not important. But the name of this function is important and must be same within the table attribute custom_mapping.
/// @param line UTF-8 byte array for one line
/// @param size_line the size of UTF-8 byte array for that line
/// @param size_columns the expected column size in one cols. In fact, you should know the size_columns before your coding. This parameter is used for checking when needed.
/// @return
Columns udm_test(const char *line, uint32_t size_line, uint32_t size_columns)
{
    // NOTE we know the size_columns is 2 before coding this test_msg_to_cols
    Columns cols;
    // cols.status_code = 0;
    // cols.ext_code = 0;
    char **bytes = malloc(2 * sizeof(char *));
    int32_t *lens = malloc(2 * sizeof(int32_t));
    char *p = strchr(line, ' ');
    size_t len0 = p - line;
    size_t len1 = size_line - 1 - len0;
    lens[0] = len0;
    lens[1] = len1;
    bytes[0] = (char *)line;
    bytes[1] = p + 1;
    cols.bytes = (uint8_t **)bytes;
    cols.lens_or_typs = lens;
    cols.size_rows = 1;
    return cols;
}
