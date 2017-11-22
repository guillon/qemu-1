#pragma once

#include <stdint.h>
#include <stdio.h>

/* This API is used by instrumentation framework */

#ifdef __cplusplus
extern "C" {
#endif

/* opaque object to represent a translated block. */
#ifdef __cplusplus
class translation_block;
#else
typedef struct translation_block translation_block;
#endif

enum architecture
{
    ARCHITECTURE_X86_64,
    ARCHITECTURE_UNKNOWN
};

/* initialize or close plugin */
/* @out is stream for plugin output */
void plugin_init(FILE* out, enum architecture arch);
void plugin_close(void);

/* get or create a block starting at @pc, with @code of a given @size in bytes.
 * @binary_file is file which contains this block.
 * @binary_file_load_address is where @binary_file_path was loaded in address
 * space. This is used to match debug information with actual pc executed.
 */
translation_block* get_translation_block(uint64_t pc, const uint8_t* code,
                                         size_t size,
                                         const char* binary_file_path,
                                         uint64_t binary_file_load_address);

/* block @b is executed
 *
 * @potential_callee_return_address is where execution should return after
 * calling a function. This is used to track function calls. On x86_64, it is
 * located on top of the stack right after a call. Potential means that it does
 * not have to be correct, a framework can simply always return the good memory
 * location, and if a call is done, it will be detected.
 */
void event_block_executed(translation_block* b,
                          uint64_t potential_callee_return_address);

/* cpus are stopped (end of program) */
void event_cpus_stopped(void);

#ifdef __cplusplus
}
#endif
