#ifndef _ATARI_EM_H_
#define _ATARI_EM_H_

#ifndef NULL
# define NULL 0
#endif

/*
 * Addressing modes
 */

/* No mode */
#define MODE_NUL 0

/* Accumulator */
#define MODE_ACC 1

/* Absolute */
#define MODE_ABS 2

/* Absolute, X-indexed */
#define MODE_ABX 3

/* Absolute, Y-indexed */
#define MODE_ABY 4

/* Immediate */
#define MODE_IMM 5

/* Implied */
#define MODE_IMP 6

/* Indirect */
#define MODE_IND 7

/* Indirect, X-indexed */
#define MODE_INX 8

/* Indirect,Y-indexed */
#define MODE_INY 9

/* Relative */
#define MODE_REL 10

/* Zero page */
#define MODE_ZPG 11

/* Zero page, X-indexed */
#define MODE_ZPX 12

/* Zero page,Y-indexed */
#define MODE_ZPY 13

typedef unsigned char byte;
typedef unsigned short word;

typedef unsigned int useconds_t;

#endif
