/**
 * @file   Seg7Display.h
 * @date   October, 2015
 * @brief  Library for using a 7 segments LED display.
 *
 * ![CCimage]: https://i.creativecommons.org/l/by/4.0/88x31.png  "Creative Commons License"
 * [creativecommons]:  http://creativecommons.org/licenses/by/4.0/  "Creative Commons Attribution 4.0 International License"
 *
 *! \mainpage Arduino Seg7Display library
 *
 * \section intro_sec Introduction
 *
 * Small library to be used with one or more 7 segments LED displays.
 * This library has been tested with the SPI 7-SEG 4DIGIT DISPLAY ARDUINO SHIELD.
 * This is a open source hardware project that exists in CircuitMaker.
 * For more info, see links below: 
 *
 * \note http://blog.circuitmaker.com/#Blogs/streamlining-design-to-manufacturing-part2
 * \note https://workspace.circuitmaker.com/Projects/7A26611E-AC67-4CEF-B859-E88B29AB8FBC
 *
 * \subsection step1 License
 * ![Creative Commons License](https://i.creativecommons.org/l/by/4.0/88x31.png "Creative Commons License") This work is licensed under [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/)
 *
 * \subsection step2 Using the Seg7Display library
 *  Usage: In order to start using the Seg7Display library you need to...
 * 
 *		1. Create a Seg7Display object.
 *		
 *		2. Call begin with two parameters.
 *			param1: The SPI SS (SlaveSelect) pin number. Arduino default is 10.
 *			param2: A pointer to a ASCII => 7SEG table.
 *					At the moment there are three different tables defined:
 *						ASCII_NUM_TAB, ASCII_HEX_TAB and ASCII_FULL_TAB.
 *						
 *					You can easily define your own decode tables if you feel the need
 *					and there is more info about this in ascii-tables.h.
 *					
 *		3. Set the number of 7 segment displays you are using.
 *			The example from CircuitMaker (see links above) uses 2 rows of 4 7Seg LED's
 *			making a total of 8 display numbers.
 * 
 * \subsection step3 Example 1
 *  Below is a small Arduino example of how to use this library.
 *  
 *		#include <SPI.h>
 *		#include <Seg7Display.h>
 *		
 *		Seg7Display seg;						// Create our display object.
 *		
 *		void setup() {							// Standard setup function.
 *			seg.begin( 10, ASCII_FULL_TAB );	// Set SS and ASCII decode table
 *			seg.setSegmentsSize(8);				// Number of display segments
 *			seg.writeSegments("Octopart");		// Display our first message...
 *		}
 *
 * \subsection step4 Example 2
 *  Below is a slightly larger Arduino example of how to use this library.
 *
 *		#include <SPI.h>
 *		#include <Seg7Display.h>
 *		
 *		Seg7Display seg;
 *		
 *		unsigned int loopDelay = 1000;
 *		uint8_t show = 0;
 *		
 *		char frame[] = {0x0c,0x01,0x01,0x0b,0x0e,0x04,0x04,0x0d,0x00};
 *		char signs[] = {0x0b,0x0c,0x0b,0x0c,0x00};
 *		unsigned long t1;
 *		
 *		void setup() {
 *		  seg.begin( 10, ASCII_FULL_TAB );
 *		  seg.setSegmentsArraySize(8);
 *		
 *		  seg.writeSegments( frame );
 *		  t1 = millis();
 *		  delay( 1000 );
 *		  t1 = millis();
 *		}
 *		
 *		void loop() {
 *		  seg.refresh();
 *		  
 *		  unsigned long t2 = millis();
 *		  if( t2-t1 > loopDelay ) {
 *		    t1 = t2;
 *		    switch( show++ ) {
 *		
 *		      case 0:
 *		        seg.writeSegments("Octopart");
 *		        loopDelay = 1000;
 *		      break;
 *		      
 *		      case 1:
 *		        seg.stopBlink();
 *		        seg.scrollUpperEx("Hello ",300,1);
 *		        seg.writeLower(" Crn");
 *		        loopDelay = 5000;
 *		      break;
 *		      
 *		      case 2:
 *		        seg.stopScroll( DISPLAY_UPPER );
 *		        seg.writeUpper("____");
 *		        seg.scrollLowerEx("WUorld ",200,0);
 *		        loopDelay = 5000;
 *		      break;
 *		      
 *		      case 3:
 *		        seg.stopScroll( DISPLAY_LOWER );
 *		        seg.writeSegments("Octopart");
 *		        seg.setBlink(0xFF,800,400);
 *		        loopDelay = 5000;
 *		      break;
 *		      
 *		      case 4:
 *		        seg.stopBlink();
 *		        seg.writeUpper("1234");
 *		        seg.setDecimalPoints(0x20);
 *		        seg.setBlink(0x10,400,100);
 *		        seg.scrollLowerEx(signs,300,1);
 *		        loopDelay = 5000;
 *		      break;
 *		      
 *		      default:
 *		        seg.stopBlink();
 *		        seg.stopScroll( DISPLAY_UPPER | DISPLAY_LOWER );
 *		        seg.setDecimalPoints(0x00);
 *		        show=0;
 *		        loopDelay = 500;
 *		    };
 *		  }
 *		}
 */
 
#ifndef Seg7Display_h
#define Seg7Display_h

#include "Arduino.h"

/// Some different ASCII tables that can be used with the library.
#include "ascii-tables.h"		

/// Defines of return codes. Should be fairly self explaining.
/*! \def ALL_OK
 *  \brief ALL_OK is used to signal no error occurred.
 * 
 *  \def ERROR_CODE_INVALID_SPI_MODE
 *  \brief return value when an invalid SPI mode is set. Not implemented yet.
 * 
 *  \def ERROR_CODE_INVALID_SS_PIN
 *  \brief return value when an invalid SS (SlaveSelect) pin is set. Used by method begin.
 * 
 *  \def ERROR_CODE_TO_FEW_SEGMENTS
 *  \brief return value when segment size is set to aero (0) in method setSegmentsArraySize.
 * 
 *  \def ERROR_CODE_OUT_OF_RANGE
 *  \brief return value from methods writeOneSegment and readOneSegment when reading/writing outside defined segments.
 */
#define ALL_OK								0
#define ERROR_CODE_INVALID_SPI_MODE			1
#define ERROR_CODE_INVALID_SS_PIN			2
#define ERROR_CODE_TO_FEW_SEGMENTS			3
#define ERROR_CODE_OUT_OF_RANGE				4
 
/*! \def DISPLAY_UPPER
 *  \brief defined number for upper display.
 * 
 *  \def DISPLAY_LOWER
 *  \brief defined number for lower display.
 */
#define DISPLAY_UPPER						0X01
#define DISPLAY_LOWER						0X02


/**
 * \struct blinks
 *
 * A display structure containing blink information for two 4 digit 7 SEG displays. 
 */
typedef struct blinks {
	unsigned long	on[8];			/*!< Containing time in milliseconds that blink is on. */
	unsigned long	off[8];			/*!< Containing time in milliseconds that blink is off. */
	unsigned long	nextToggle[8];	/*!< Containing time in milliseconds for next toggle. */
	uint8_t			isOn[8];		/*!< True if the digit is on. */
	void*			next;			/*!< Pointer to next 7SEG array. Not implemented yet. */
}blink_t;							/*!< typedef for structure blinks */

/**
 * \struct displays
 *
 * A display type containing text arrays for two 4 digit 7 SEG displays.
 * This is a union of upLo[8] and two 4 char arrays (upper and lower).
 *
 * disp_t also contains a next pointer to support an array of 2*4 digit displays.
 *
 */
typedef struct displays {
	union {
		char	upLo[8];				/*!< The eight digits for the upper and lower displays. */
		struct {
			char	upper[4];			/*!< The four digits for the upper display. */
			char 	lower[4];			/*!< The four digits for the lower display. */
		};
	};
	void*	next;						/*!< Pointer to the next 2*4 display. Not implemented yet. */
}disp_t;								/*!< typedef for structure displays */

/**
 * \struct scroll
 *
 * A scroll type containing time information for scrolling one 4 digit display left or right.
 * text in scroll_t is a String object containing the text to be scrolled for that display.
 *
 */
typedef struct scroll {
	unsigned long		time;		/*!< The time (milliseconds) when the scroll text was updated last time. */
	String				text;		/*!< The text to scroll. */
	unsigned long		delay;		/*!< The scroll delay time in milliseconds. */
	uint8_t				toLeft;		/*!< True if the text scrolls from right to left. */
	uint8_t				marker;		/*!< Marks which digit we are printing on the current 7SEG display segment. */
}scroll_t;							/*!< typedef for structure scroll */
		
/**
 * \class Seg7Display
 *
 * \brief Library for using a 7 segments LED display.
 *
 * Small library to be used with one or more 7 segments LED displays.
 * This library has been tested with the SPI 7-SEG 4DIGIT DISPLAY ARDUINO SHIELD.
 * This is a open source hardware project that exists in CircuitMaker.
 *
 * \version 1.0 $
 *
 * \date 2015/10/07 $
 *
 * Created on:  2015/10/07
 *
 */
class Seg7Display
{
	public:	
		//! Seg7Display default constructor.
					Seg7Display();
					
		//! Sets the SS pin and what ASCII 2 7SEG table to use.
		/*!
		  \param [in] pin the SS (SlaveSelect) pin number.
		  \param [in] table pointer to a ASCII 2 7SEG decode table.
		  \return Returns ALL_OK on success. 
		  \sa ascii-tables.h for availabe decode tables.
		  \sa ALL_OK for error codes.
	    */
		uint8_t		begin(uint8_t pin, const unsigned char *table);
		
		//! Sets the number of display segments available.
		/*!
		  \param [in] size is the number of 7SEG digits to use.
		  \return Returns ALL_OK on success. 
		  \sa ALL_OK for error codes.
	    */
		uint8_t 	setSegmentsArraySize(uint8_t size);
		
		//! writes a String to the display.
		/*!
		  \param [in] txt is the String object to be displayed
	    */
		void	 	writeSegments(String txt);

		//! writes a String to the display.
		/*!
		  \param [in] txt is the String object to be displayed
	    */
		void 		writeUpper(String txt);

		//! writes a String to the display.
		/*!
		  \param [in] txt is the String object to be displayed
	    */
		void 		writeLower(String txt);
		
		//! writes one character to one display segment.
		/*!
		  \param [in] seg is the display digit segment to write to. First segment is 1.
		  \param [in] ch is the character to display on the 7SEG digit.
		  \return Returns ALL_OK on success. 
		  \sa ALL_OK for error codes.
	    */
		uint8_t 	writeOneSegment(uint8_t seg, char ch);
		
		//! Reads one character from the display buffer.
		/*!
		  \param [in] seg is the number of 7SEG digits to read.
		  \param [out] ch contains the read character, or '\0' on error code ERROR_CODE_OUT_OF_RANGE.
		  \return If the seg parameter is outside the text buffer, then we return ERROR_CODE_OUT_OF_RANGE. Otherwise ALL_OK.
	    */
		uint8_t 	readOneSegment(uint8_t seg, char& ch);
		
		//! updates all digits on the display.
		/*! This function must be called regularly for the text to be displayed on the 7 SEG display()s).
		 */
		void		refresh();
		
		//! scrolls a text in the lower display.
		/*!
		  \param [in] str is the text to be scrolled on the lower display.
		  \param [in] t is the scroll delay time in milliseconds.
		  \param [in] left is true (not 0) for left scroll. Otherwise we scroll to the right.
	    */
		void		scrollLowerEx(String str, unsigned int t, uint8_t left);
		
		//! scrolls a text in the lower display.
		/*!
		  \param [in] t is the scroll delay time in milliseconds.
		  \param [in] left is true (not 0) for left scroll. Otherwise we scroll to the right.
	    */
		void 		scrollLower(unsigned int t, uint8_t left);

		//! scrolls a text in the upper display.
		/*!
		  \param [in] str is the text to be scrolled on the upper display.
		  \param [in] t is the scroll delay time in milliseconds.
		  \param [in] left is true (not 0) for left scroll. Otherwise we scroll to the right.
	    */
		void		scrollUpperEx(String str, unsigned int t, uint8_t left);

		//! scrolls a text in the lower display.
		/*!
		  \param [in] t is the scroll delay time in milliseconds.
		  \param [in] left is true (not 0) for left scroll. Otherwise we scroll to the right.
	    */
		void 		scrollUpper(unsigned int t, uint8_t left);

		//! Function to set one or more decimal points.
		/*!
		 * Upper display: First (leftmost) digit == 0x80, second == 0x40, third == 0x20, fourth (last) == 0x10.
		 * Lower display: First (leftmost) digit == 0x08, second == 0x04, third == 0x02, fourth (last) == 0x01.
	    */
		void		setDecimalPoints(uint8_t points);
		
		//! Set setBlink interval for one or more digits.
		/*!
		  \param [in] digit is an or (|) combination of one or more display digits.
		  \param [in] on is time in milliseconds that the digits is on.
		  \param [in] off is time in milliseconds that the digits are off.
		 
		  \note
		  Upper display: First (leftmost) digit == 0x80, second == 0x40, third == 0x20, fourth (last) == 0x10.
		  Lower display: First (leftmost) digit == 0x08, second == 0x04, third == 0x02, fourth (last) == 0x01.
	    */
		void		setBlink(uint8_t digit, unsigned int on, unsigned int off);
		
		//! Stop blinking all digits.
		/*!
		 * 
	    */
		void stopBlink();
		
		//! Stop scrolling any or both of the displays.
		/*!
		  \param [in] displays can be DISPLAY_UPPER, DISPLAY_LOWER or both (DISPLAY_UPPER | DISPLAY_LOWER).
	    */
		void stopScroll( uint8_t displays);

	private:	/// Stuff private to the class. Don't touch!
		/// SPI slave select pin.
		int					m_slaveSelectPin;
		
		/// Number of 7 segment LED's. Default value is 1.
		uint8_t				m_segmentSize;
		
		/// Pointer to text to be displayed. This is implemented as a String object.
		disp_t				m_disp;
		
		/// Pointer to used ASCII table. Set this pointer in the begin() method.
		const unsigned char	*m_ascii_table;
		
		/// member variables to keep track of time when text is scrolling on the upper display.
		scroll_t			m_scrollUpper;

		/// member variables to keep track of time when text is scrolling on the lower display.
		scroll_t			m_scrollLower;
		
		/// member variable containing information about any decimal point to be set in a 2*4 digit display.
		/// 0x01 == lower left, 0x08 == lower right, 0x10 == upper left, 0x80 == upper right.
		/// Example: 0x23 would light up the two right most points in the lower display and the second right point in the upper display.
		uint8_t				m_dps;
		
		/// member variable containing information about blink interval for a 2*4 digit display.
		blink_t				m_blink;
		
		/// Helper method to decode ASCII tables.
		uint8_t 			asciiTo7seg(char ch);				

		/// Helper writer method to write to upper, lower or both displays.
		void 				helperWrite(String& txt, char* buf, uint8_t len);

		//! Function to check if there is scrolling text to display.
		/*!
		  \param [in] scroll is the scroll object.
		  \param [in] disp is the display object.
		 * \sa scrollUpper and scrollLower for how to set up scrolling text.
	    */
		void 				helperScroll(scroll_t& scroll, char *disp);

		//! Function to set up scrolling text for the upper or lower display.
		/*!
		  \param [in] str is the string to scroll.
		  \param [in] scroll is the scroll object.
		  \param [in] disp is the display object.
		  \param [in] t is the scroll delay time in milliseconds.
		  \param [in] left is true (not 0) for left scroll. Otherwise we scroll to the right.
		 * \sa scrollUpper and scrollLower for how to set up scrolling text.
	    */
		void 				helperSetupScroll(String& str, scroll_t& scroll, char *disp, unsigned int t, uint8_t left);
};

#endif // Seg7Display_h
