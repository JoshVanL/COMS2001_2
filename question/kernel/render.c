#include "render.h"

uint16_t fb[600][800];
int currLine = 5;
int currCol = 5;
extern char font[128][8];


void renderInit() {
  SYSCONF->CLCD      = 0x2CAC;     // per per Table 4.3 of datasheet
  LCD->LCDTiming0    = 0x1313A4C4; // per per Table 4.3 of datasheet
  LCD->LCDTiming1    = 0x0505F657; // per per Table 4.3 of datasheet
  LCD->LCDTiming2    = 0x071F1800; // per per Table 4.3 of datasheet

  LCD->LCDUPBASE     = ( uint32_t )( &fb );

  LCD->LCDControl    = 0x00000020; // select TFT   display type
  LCD->LCDControl   |= 0x00000008; // select 16BPP display mode
  LCD->LCDControl   |= 0x00000800; // power-on LCD controller
  LCD->LCDControl   |= 0x00000001; // enable   LCD controller

  PS20->CR           = 0x00000010; // enable PS/2    (Rx) interrupt
  PS20->CR          |= 0x00000004; // enable PS/2 (Tx+Rx)
  PS21->CR           = 0x00000010; // enable PS/2    (Rx) interrupt
  PS21->CR          |= 0x00000004; // enable PS/2 (Tx+Rx)

  uint8_t ack;

  PL050_putc( PS20, 0xF4 );  // transmit PS/2 enable command
  ack = PL050_getc( PS20       );  // receive  PS/2 acknowledgement
  PL050_putc( PS21, 0xF4 );  // transmit PS/2 enable command
  ack = PL050_getc( PS21       );  // receive  PS/2 acknowledgement

  for( int i = 0; i < 600; i++ ) {
    for( int j = 0; j < 800; j++ ) {
      fb[ i ][ j ] = 0x9CEA;
    }
  }

  for( int i = 0; i < 460; i++ ) {
    for( int j = 400; j < 405; j++ ) {
      fb[ i ][ j ] = 0x0000;
    }
  }
  
  for( int i = 460; i < 465; i++ ) {
    for( int j = 0; j < 800; j++ ) {
      fb[ i ][ j ] = 0x0000;
    }
  }


  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x0000;
        }
    }
}


void carriageReturn() {
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    currCol = 5;
    currLine += 17;
    for( int i=0; i<16; i++) {
        for(int j =0; j<12; j++) {
            fb[currLine+i][currCol+j] = 0x0000;
          }
      }
 return;
}

void drawLetter(char c) {
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    char* chr = font[c];
    for(uint8_t i=0; i<8; i++) {
        for(uint8_t j=0; j<8; j++) {
            if(chr[j] & (1<<i)) {
                for(int m=0; m<2; m++) {
                    for(int n=0; n<2; n++) {
                        fb[2*j+currLine+m][2*i+currCol+n] = 0x0000;
                    }
                }
            }
        }
    }
    currCol += 16;
    if(currCol > 385) {
        currCol = 5;
        currLine += 17;
    }
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x0000;
        }
    }
}

void deleteLetter(int consoleBuffer) {
  for( int i=0; i<16; i++) {
      for(int j =0; j<16; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    if(currCol <= 16 && currLine > 17 && consoleBuffer > 0) {
        consoleBuffer--;
        currCol = 385;
        currLine -= 17;
    } else if( currCol > 5 && consoleBuffer > 0) {
        consoleBuffer--;
        currCol -= 16;
    }
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x0000;
        }
    }
}

void drawString(char* c, int n) {
    for(int i=0; i<n; i++) {
        if(c[i] =='\n') carriageReturn();
        else drawLetter(c[i]);
    }
}



