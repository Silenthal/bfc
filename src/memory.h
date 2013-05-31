#ifndef BF_MEMORY_H
#define BF_MEMORY_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void    bf_mem_inc();
void    bf_mem_dec();
void    bf_mem_left();
void    bf_mem_right();
uint8_t bf_mem_read();
void    bf_mem_write(uint8_t data);
void    bf_mem_clear();

#ifdef __cplusplus
}
#endif

#endif
