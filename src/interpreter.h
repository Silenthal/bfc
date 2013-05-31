#ifndef BF_INTERPRET_H
#define BF_INTERPRET_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

bool        bf_setup(const char* program);
bool        bf_setup_with_file(const char* filename);
bool        bf_loop();
void        bf_free_resources();
const char* bf_get_last_error();

#ifdef __cplusplus
}
#endif

#endif