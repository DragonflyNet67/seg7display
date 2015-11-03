/**
 * @file   Seg7Display.cpp
 * @date   October, 2015
 * @brief  Library for using a 7 segments LED display.
 *
 * @license
* ![Creative Commons License](https://i.creativecommons.org/l/by/4.0/88x31.png "Creative Commons License") This work is licensed under [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/)
 *
 */

#include "SPI.h"
#include <Seg7Display.h>

// Standard constructor
Seg7Display::Seg7Display()
{
	for(uint8_t i=0; i<8; i++ ) {
		m_disp.upLo[i] 			= ' ';
		m_blink.nextToggle[i]   = 0;
		m_blink.isOn[i] 		= 1;
	}
	m_disp.next  	= NULL;
	m_blink.next 	= NULL;
	m_ascii_table 	= NULL;
	
	m_segmentSize = 1;
	m_scrollUpper.delay = m_scrollLower.delay = 0;
	m_dps = 0;
	m_slaveSelectPin = 10;
}

// begin sets the SS pin and what ASCII 2 7SEG table to use.
uint8_t Seg7Display::begin(uint8_t pin, const unsigned char *table)
{
	if( pin>10) {
		return ERROR_CODE_INVALID_SS_PIN;
	}
	
	// Set the SlaveSelect pin
	m_slaveSelectPin = pin;
	// Set the ASCII 2 7SEG display table 
	m_ascii_table = table;
	
	// Setup the SPI
	pinMode(m_slaveSelectPin, OUTPUT);
	SPI.setDataMode( SPI_MODE0 );
	SPI.setBitOrder(LSBFIRST);
	SPI.begin();
	
	return ALL_OK;
}

/// setSegmentsSize sets the number of display segments available.
uint8_t Seg7Display::setSegmentsArraySize(uint8_t size)
{
	// There must be at least one 7SEG display 
	if( size > 0) {
		m_segmentSize = size; 
		return ALL_OK;
	}

	return ERROR_CODE_TO_FEW_SEGMENTS;
}

 // writeSegments writes a String to both upper and lower displays.
void Seg7Display::writeSegments(String txt)
{
	helperWrite(txt, m_disp.upLo, 8);
}

// Write a string to the upper segments
void Seg7Display::writeUpper(String txt)
{
	helperWrite(txt, m_disp.upper, 4);
}

// Write a string to the lower segments
void Seg7Display::writeLower(String txt)
{
	helperWrite(txt, m_disp.lower, 4);
}

// writeSegment writes one character to one display segment.
uint8_t Seg7Display::writeOneSegment(uint8_t seg, char ch)
{
	// If selected segment higher than available segments, then return ERROR_CODE_OUT_OF_RANGE.
	if( (seg>m_segmentSize) || (seg==0) ) {
		return ERROR_CODE_OUT_OF_RANGE;
	}
	
	/* If the current display buffer is to small, then pad with spaces ' ' until
	 * we can add the ch parameter at the right String position.
	 */
	m_disp.upLo[seg-1] = ch;
	 
	// Make a refresh to display the character(s) we just added to the String buffer.
	refresh();
	
	return ALL_OK;
}

/* readSegment reads one character from the display buffer.
 * If the seg parameter is outside the text buffer, then we return ERROR_CODE_OUT_OF_RANGE.
 * The ch parameter [out] will then contain '\0'.
 */
uint8_t Seg7Display::readOneSegment(uint8_t seg, char& ch)
{
	if( seg > 8 ) {
		ch = '\0';
		return ERROR_CODE_OUT_OF_RANGE;
	}
	
	ch = m_disp.upLo[seg];
	return ALL_OK;
}

// refresh can be used to refresh the 7SEG displays.
void Seg7Display::refresh()
{
	uint8_t dp = 0x80;						// Helper variable for decimal point write.
	unsigned long thisTime = millis();		// What time is it now?
	
	// Check if we are scrolling the upper row.
	if( m_scrollUpper.delay ) {
		helperScroll(m_scrollUpper, &m_disp.upper[0]);
	}
	
	// Check if we are scrolling the lower row.
	if( m_scrollLower.delay ) {
		helperScroll(m_scrollLower, &m_disp.lower[0]);
	}
	
	// Loop through all the digits in our array.
	for( uint8_t i=0; i<m_segmentSize; i++)
	{
	  // Check if any of the digits are in blink mode
	  if( m_blink.nextToggle[i] && (m_blink.nextToggle[i]<thisTime)) {
		  
		  // Yes, we are blinking digit i, so we just toggle it (on/off) ...
		  m_blink.isOn[i] = m_blink.isOn[i]?0:1;
		  
		  // ... and set the next toggle interval.
		  if( m_blink.isOn[i] ) {
			  m_blink.nextToggle[i] = m_blink.on[i] + thisTime;
		  } else {
			  m_blink.nextToggle[i] = m_blink.off[i] + thisTime;
		  }
	  }
	  
	  // m_blink.isOn[i] is always 1 when not blinking, and 0 or 1 when in blinking mode.
	  if( m_blink.isOn[i] ) {
		  /** spi_packet:  bits 8-15 = 7SEG code for character to print on the display
		   *               bits 0-7  = code for which 7SEG to update. 0x80 = upper left, 0x01 = lower right.
		   */
		  word spi_packet = (unsigned word)asciiTo7seg(m_disp.upLo[i])<<8 | 0x80>>i;
		  
		  // Add any active decimal points to the display.
		  spi_packet |= (m_dps & dp)? 0x0100:0x0000;
		  
		  // Transfer two bytes over SPI.
		  digitalWrite(m_slaveSelectPin, LOW);
		  SPI.transfer16(spi_packet);
		  digitalWrite(m_slaveSelectPin, HIGH);
	  } else {
		  // Transfer a zero to digit i.
		  digitalWrite(m_slaveSelectPin, LOW);
		  SPI.transfer16(0x0000 + (0x80>>i));
		  digitalWrite(m_slaveSelectPin, HIGH);
	  }
	  
	  dp = dp>>1;			// Check if next decimal point is on or not.
	}
}

// Call this function to set up scrolling text for the upper display.
void Seg7Display::scrollUpperEx(String str, unsigned int t, uint8_t left)
{
	scrollHelper(str, m_scrollUpper, m_disp.upper, t, left);
}

// Call this function to set up scrolling text for the upper display.
void Seg7Display::scrollUpper(unsigned int t, uint8_t left)
{
	String str = m_disp.upper;
	str.remove(4);
	scrollUpperEx( str, t, left);
}

// Call this function to set up scrolling text for the lower display.
void Seg7Display::scrollLowerEx(String str, unsigned int t, uint8_t left)
{
	scrollHelper(str, m_scrollLower, m_disp.lower, t, left);
}

// Call this function to set up scrolling text for the lower display.
void Seg7Display::scrollLower(unsigned int t, uint8_t left)
{
	String str = m_disp.lower;
	str.remove(4);
	scrollLowerEx( m_disp.lower, t, left);
}

// Sets the m_bps member variable to the digits with decimal point set.
void Seg7Display::setDecimalPoints(uint8_t points)
{
	m_dps = points;
}

// Set what digits should blink and the time interval.
void Seg7Display::setBlink(uint8_t digit, unsigned int on, unsigned int off)
{
	uint8_t i = 0;
	millis();
	delay(100);
	unsigned long t = millis();
	for( uint8_t x = 0x80; x; x = x>>1) {
		if( x & digit ) {
			m_blink.on[i]			= on;
			m_blink.off[i]			= off;
			m_blink.nextToggle[i]	= t;
			m_blink.isOn[i]			= 0;
		}
		i++;
	}
}

// Stop blinking one or more of the 7SEG digits.
void Seg7Display::stopBlink()
{
	for(uint8_t i=0; i<8; i++) {
		m_blink.nextToggle[i]	= 0;
		m_blink.isOn[i]			= 1;
	}
}

// Stop scrolling display array.
void Seg7Display::stopScroll( uint8_t displays) 
{
	if( displays & DISPLAY_LOWER ) {
		m_scrollLower.delay = 0;
	} 
	if( displays & DISPLAY_UPPER ) {
		m_scrollUpper.delay = 0;
	}
}


//  =========================================================================
//  Private member methods.
//
//  =========================================================================

// Helper method to decode ASCII tables.
uint8_t Seg7Display::asciiTo7seg(char ch)
{
	uint8_t start = *m_ascii_table;
	uint8_t end   = *(m_ascii_table+1);
	if( (ch>start) && (ch<=end))  {
		return *(m_ascii_table + (ch-(start-2)));
	}
	if( ch>=0 && ch<32 ) { // Ok, get on of our special characters
		return SPECIAL_CHARS[ch];
	}

	return 0;  // The character was outside the ASCII table used.
}

// Call this function to set up scrolling text for the upper display.
void Seg7Display::scrollHelper(String& str, scroll_t& scroll, char *disp, unsigned int t, uint8_t left)
{
	for(uint8_t x=0; x<4; x++) {
		*(disp+x) = ' ';
	}
	scroll.text = str;
	scroll.delay = t;
	scroll.time = millis();
	scroll.toLeft = left;
	scroll.marker = left?0:str.length()-1;
}

/// Helper function write text to any of the display buffers. 
void Seg7Display::helperWrite(String& txt, char* buf, uint8_t len)
{
	uint8_t x;
	if( txt.length()>len) {
		txt.remove(len);
	}
	for( x=0; x<txt.length(); x++) {
		*(buf+x) = txt.charAt(x);
	}
	while( x++ < len) {
		*(buf+x) = ' ';
	}
}

// Function that check what digits to display where when we are in scroll mode.
void Seg7Display::helperScroll(scroll_t& scroll, char *disp)
{
	uint8_t x;		// Helper loop variable.
	
	// This is a private method and we have already made sure that we are in scroll mode
	//   for this array of 7 SEG digit display. 
	// If scroll.delay != 0, then we are in scroll mode. So check this before calling this method.
	if( (scroll.time + scroll.delay) < millis() ) {
		if( scroll.toLeft ) {
			for(x=0; x<3; x++) {
				*(disp+x) = *(disp+x+1);
			}
			*(disp+x) = scroll.text.charAt(scroll.marker);
			scroll.marker = (scroll.text.length()==(scroll.marker+1))?0:scroll.marker+1;
		} else {
			for(x=3; x>0; x--) {
				*(disp+x) = *(disp+x-1);
			}
			*(disp+x) = scroll.text.charAt(scroll.marker);
			scroll.marker = (scroll.marker==0)?scroll.text.length()-1:scroll.marker-1;
		}
		scroll.time = millis();
	}
}
