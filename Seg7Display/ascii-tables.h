/**
 * @file   ascii-tables.h
 * @date   October, 2015
 * @brief  Set of various ASCII => 7Segment LED display.
 *
 * This header file was created for use with the Seg7Display Arduino library.
 *
 * License: ???
 *
 *(C) Copyright 2015 Altium LLC
 */

 
#ifndef ASCII_TABLES_H
#define ASCII_TABLES_H

/* 7-Segment decoder tables

           <=A=>        segment A = 0b10000000 = 0x80u (for LSB first SPI bus transmission
           ^     ^      segment B = 0b01000000 = 0x40u
           F     B      segment C = 0b00100000 = 0x20u
           v<=G=>v      segment D = 0b00010000 = 0x10u
           ^     ^      segment E = 0b00001000 = 0x08u
           E     C      segment F = 0b00000100 = 0x04u
           v     v      segment G = 0b00000010 = 0x02u
            <=D=>       seg. d.p. = 0b00000001 = 0x01u
                  d.p.
*/

/// Define special symbols
#define SYM_A	0X80
#define SYM_B	0X40
#define SYM_C	0X20
#define SYM_D	0X10
#define SYM_E	0X08
#define SYM_F	0X04
#define SYM_G	0X02
#define SYM_DP	0X01

/*! \def SYM_A
 *  \brief SYM_A code for segment A.
 *
 *  \def SYM_B
 *  \brief SYM_B code for segment B.
 *
 *  \def SYM_C
 *  \brief SYM_C code for segment C.
 *
 *  \def SYM_D
 *  \brief SYM_D code for segment D.
 *
 *  \def SYM_E
 *  \brief SYM_E code for segment E.
 *
 *  \def SYM_F
 *  \brief SYM_F code for segment F.
 *
 *  \def SYM_G
 *  \brief SYM_G code for segment G.
 *
 *  \def SYM_DP
 *  \brief SYM_DP code for segment the decimal point.
 */

#define SYM_BAR_L	(SYM_E | SYM_F)			// Left bar
#define SYM_BAR_R	(SYM_B | SYM_C)			// Right bar
#define SYM_BAR_RT	(SYM_BAR_R | SYM_A)		// Right bar w roof
#define SYM_BAR_LT	(SYM_BAR_L | SYM_A)		// Left bar w roof
#define SYM_BAR_RB	(SYM_BAR_R | SYM_D)		// Right bar w floor
#define SYM_BAR_LB	(SYM_BAR_L | SYM_D)		// Left bar w floor

/*! \def SYM_BAR_L
 *  \brief SYM_BAR_L code for segment E and F.
 *
 *  \def SYM_BAR_R
 *  \brief SYM_BAR_R code for segment B and C.
 *
 *  \def SYM_BAR_RT
 *  \brief SYM_BAR_RT code for segment A, B and C.
 *
 *  \def SYM_BAR_LT
 *  \brief SYM_BAR_LT code for segment A, E and F.
 *
 *  \def SYM_BAR_RB
 *  \brief SYM_BAR_RB Code for segment B, C and D.
 *
 *  \def SYM_BAR_LB
 *  \brief SYM_BAR_LB Code for segment D, E and F.
 *
 */

/*! SPECIAL_CHARS is an array of special characters that can be displayed on the 7SEG display.
 *  Element zero (0) should never be used since zero is string termination in the String object we use.
 */
const unsigned char SPECIAL_CHARS[32] = 
 {
	0X00,				// 0x00 should never be used in a String object.
	SYM_A,				// 0x01 Segment A
	SYM_B,				// 0x02 Segment B
	SYM_C,				// 0x03 Segment C
	SYM_D,				// 0x04 Segment D
	SYM_E,				// 0x05 Segment E
	SYM_F,				// 0x06 Segment F
	SYM_G,				// 0x07 Segment G
	SYM_DP,				// 0x08 Segment decimal point
 	SYM_BAR_L,			// 0x09 Left bar
 	SYM_BAR_R,			// 0x0A Right bar
 	SYM_BAR_RT,			// 0x0B Right bar w roof
 	SYM_BAR_LT,			// 0x0C Left bar w roof
 	SYM_BAR_RB,			// 0x0D Right bar w floor
 	SYM_BAR_LB,			// 0x0E Left bar w floor
	0X00,				// 0x0F blank...
	0X00,				// 0x10 blank...
	0X00,				// 0x11 blank...
	0X00,				// 0x12 blank...
	0X00,				// 0x13 blank...
	0X00,				// 0x14 blank...
	0X00,				// 0x15 blank...
	0X00,				// 0x16 blank...
	0X00,				// 0x17 blank...
	0X00,				// 0x18 blank...
	0X00,				// 0x19 blank...
	0X00,				// 0x1A blank...
	0X00,				// 0x1B blank...
	0X00,				// 0x1C blank...
	0X00,				// 0x1D blank...
	0X00,				// 0x1E blank...
	0X00				// 0x1F blank...
};

/*! ASCII to SEVEN-SEGMENT implementation tables
 *
 * The format for a ASCII to 7SEG table is as follows.
 * byte 0: First location in the ASCII table
 * byte 1: Last location in the ASCII table
 * byte 2 to n: The 7SEG encoding for that specific ASCII character.
 *
 * For example, see the tables ASCII_NUM_TAB, ASCII_NUM_TAB and ASCII_NUM_TAB below.
 */

/// ASCII table for numbers only.
const unsigned char ASCII_NUM_TAB[] =
 {
	0x30,		// 0x30 == 48 == '0' == First character in the table.
	0x39,		// 0x39 == 57 == '9' == Last character in the table.
    0xFC,       // '0'
    0x60,       // '1'
    0xDA,       // '2'
    0xF2,       // '3'
    0x66,       // '4'
    0xB6,       // '5'
    0xBE,       // '6'
    0xE0,       // '7'
    0xFE,       // '8'
    0xE6        // '9'
};

/// ASCII table for HEX numbers only. 
const unsigned char ASCII_HEX_TAB[] =
 {
	0x30,		// 0x30 == 48 == '0' == First character in the table.
	0x46,		// 0x46 == 70 == 'F' == Last character in the table.
    0xFC,       // '0'
    0x60,       // '1'
    0xDA,       // '2'
    0xF2,       // '3'
    0x66,       // '4'
    0xB6,       // '5'
    0xBE,       // '6'
    0xE0,       // '7'
    0xFE,       // '8'
    0xE6,       // '9'
    0x00,       // ':', No seven-segment implementation for colon
    0x00,       // ';', No seven-segment implementation for semi-colon 
    0x00,       // '<', No seven-segment implementation for less-than sign
    0x00,       // '=', Equal sign
    0x00,       // '>', No seven-segment implementation for greater-than sign
    0x00,       // '?', Question mark
    0x00,       // '@', No seven-segment implementation for commercial at-sign 
    0xEE,       // 'A'
    0x3E,       // 'B', Actually displayed as 'b'
    0x9C,       // 'C'               
    0x7A,       // 'D', Actually displayed as 'd'
    0x9E,       // 'E'
    0x8E        // 'F'
};

/** ASCII table for printable characters [' '..'z'].
 * Note that many of these characters will be displayed as space ' ' since there is
 * no good way to display them on a 7 segments LED display.
 */
const unsigned char ASCII_FULL_TAB[] =
 {
	0x20,		// 0x20 == 32  == ' ' == First character in the table.
	0x7A,		// 0x7A == 122 == 'z' == Last character in the table.
    0x00,       // ' '
    0x41,       // '!', 
    0x44,       // '"', Double quote
    0x00,       // '#', Pound sign
    0x00,       // '$', No seven-segment implementation for dollar sign
    0x00,       // '%', No seven-segment implementation for percent sign
    0x00,       // '&', No seven-segment implementation for ampersand
    0x40,       // ''', Single quote
    0x9C,       // '(', Same as '['
    0xF0,       // ')', Same as ']'
    0x00,       // '*', No seven-segment implementation for asterix
    0x00,       // '+', No seven-segment implementation for plus sign
    0x01,       // ',', Same as '.'
    0x02,       // '-', Minus sign
    0x01,       // '.', 
    0x00,       // '/', No seven-segment implementation for slash
    0xFC,       // '0'
    0x60,       // '1'
    0xDA,       // '2'
    0xF2,       // '3'
    0x66,       // '4'
    0xB6,       // '5'
    0xBE,       // '6'               
    0xE0,       // '7'
    0xFE,       // '8'
    0xE6,       // '9'
    0x00,       // ':', No seven-segment implementation for colon
    0x00,       // ';', No seven-segment implementation for semi-colon 
    0x00,       // '<', No seven-segment implementation for less-than sign
    0x12,       // '=', Equal sign
    0x00,       // '>', No seven-segment implementation for greater-than sign
    0xCA,       // '?', Question mark
    0x00,       // '@', No seven-segment implementation for commercial at-sign 
    0xEE,       // 'A'
    0x3E,       // 'B', Actually displayed as 'b'
    0x9C,       // 'C'               
    0x7A,       // 'D', Actually displayed as 'd'
    0x9E,       // 'E'
    0x8E,       // 'F'
    0xBC,       // 'G', Actually displayed as 'g'
    0x6E,       // 'H'   
    0x60,       // 'I', Same as '1'
    0x78,       // 'J'                   
    0x00,       // 'K', No seven-segment implementation         
    0x1C,       // 'L'                                     
    0x1C,       // 'M', No seven-segment implementation
    0x2A,       // 'N', Actually displayed as 'n'     
    0xFC,       // 'O', Same as '0'                     
    0xCE,       // 'P'                                   
    0x00,       // 'Q', No seven-segment implementation       
    0x0A,       // 'R', Actually displayed as 'r'                 
    0xB6,       // 'S', Same as '5'                         
    0x1E,       // 'T', Actually displayed as 't'             
    0x7C,       // 'U'                                   
    0x00,       // 'V', No seven-segment implementation         
    0x3C,       // 'W', No seven-segment implementation - used to the left of U to make LU style "W"        
    0x00,       // 'X', No seven-segment implementation       
    0x76,       // 'Y'                                   
    0x00,       // 'Z', No seven-segment implementation     
    0x00,       // '['                             
    0x00,       // '\', No seven-segment implementation 
    0x00,       // ']'                               
    0x00,       // '^', No seven-segment implementation   
    0x10,       // '_', Underscore                       
    0x00,       // '`', No seven-segment implementation for reverse quote
    0xFA,       // 'a'                                     
    0x3E,       // 'b'                                     
    0x1A,       // 'c'                                   
    0x7A,       // 'd'                                 
    0xDE,       // 'e'                           
    0x8E,       // 'f', Actually displayed as 'F' 
    0xF6,       // 'g'                         
    0x2E,       // 'h'                                 
    0x20,       // 'i'                                           
    0x78,       // 'j', Actually displayed as 'J'             
    0x00,       // 'k', No seven-segment implementation           
    0x1C,       // 'l', Actually displayed as 'L'           
    0x0A,       // 'm', No seven-segment implementation - used to the left of n to make rn style 'm'      
    0x2A,       // 'n'                               
    0x3A,       // 'o'                             
    0xCE,       // 'p', Actually displayed as 'P'       
    0xEA,       // 'q',
    0x0A,       // 'r'                                 
    0xB6,       // 's', Actually displayed as 'S'         
    0x1E,       // 't'                   
    0x38,       // 'u'
    0x00,       // 'v', No seven-segment implementation
    0x00,       // 'w', No seven-segment implementation
    0x00,       // 'x', No seven-segment implementation
    0x76,       // 'y', Actually displayed as 'Y'             
    0x00        // 'z', No seven-segment implementation
};

#endif // ASCII_TABLES_H
