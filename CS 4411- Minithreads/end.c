/* 
 * Function to mark the start of the area of memory in 
 * which a running minithread may be preempted.
 *
 * YOU SHOULD NOT [NEED TO] MODIFY THIS FILE.
*/

#include <setjmp.h>
#include "defs.h"

#ifdef WINCE
unsigned int end(void) {  
#elif defined(_M_IX86)
unsigned long end(void) {  
#elif defined(_M_AMD64)
unsigned __int64 end(void) {  
#else
#error "Arch not supported?"
#endif
  jmp_buf buf;

  setjmp(buf);
#ifdef WINCE
  /* for ARM processor PC is in position 9 in jmp_buf */
  return buf[10];
#else
#if defined(_M_IX86)
  /* for x86 processor PC is in position 6 in jmp_buf */
  return buf[5];
#elif defined(_M_AMD64)
  /* except on x86_64 */
  return buf[5].Part[0];
#else
#error "Arch not supported?"
#endif
#endif
}
