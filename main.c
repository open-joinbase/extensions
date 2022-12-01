#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/// Row, is the array of the bytes-represented columns. That is, you split a line string into an array of column values in the string's bytes form. JoinBase later will convert the column to its own type data for storage.
///
/// Contract:
/// 1. bytes is the array of byte array for every column, lens is the uint32_t array of the len of every column in bytes. Their size are provided by `size_columns`, which is provided in the parameters of API call. The order of arrays is important. The order must be same to the column order specified in the schema. NOTE: we use byte array rather than null-terminated C string.
/// 2. status_code: if status_code < 0, then the value is interpreted as the error code, the log will print this error code in positive number for debugging; if status_code==0, the arrays of bytes and lens are hinted as malloc-ed memories and they will be free-ed by JoinBase; if status_code==1, the arrays of bytes and lens are hinted as UDM function managed memory, JoinBase will not free them. NOTE: status_code==1 is for zero-copying udms, to use this status_code if and only if you understand it.
/// 3. it is your responsibilty to provide correct row representation. The violation will cause the undefined behavior(UB).
typedef struct Row
{
    const uint8_t **bytes;
    const uint32_t *lens;
    int32_t status_code;
} Row;

/// @brief the example UDM: test_msg_to_row. The name of the function must be same within the table attribute custom_mapping
/// @param line UTF-8 byte array for one line
/// @param size_line the size of UTF-8 byte array for that line
/// @param size_columns the expected column size in one row. In fact, you should know the size_columns before your coding. This parameter is used for checking when needed.
/// @return
Row test_msg_to_row(const char *line, uint32_t size_line, uint32_t size_columns)
{
    // NOTE we know the size_columns is 2 before coding this test_msg_to_row
    Row row;
    row.status_code = 0;
    char **bytes = malloc(2 * sizeof(char *));
    uint32_t *lens = malloc(2 * sizeof(uint32_t));
    char *p = strchr(line, ' ');
    size_t len0 = p - line;
    size_t len1 = size_line - 1 - len0;
    lens[0] = len0;
    lens[1] = len1;
    char *bs0 = malloc(len0);
    char *bs1 = malloc(len1);
    memcpy(bs0, line, len0);
    memcpy(bs1, p + 1, len1);
    bytes[0] = bs0;
    bytes[1] = bs1;
    row.bytes = (const uint8_t **)bytes;
    row.lens = lens;
    return row;
}
