#include "render.h"

uint16_t fb[600][800];
int currLine = 5;
int currCol = 5;
int currLineKE = 5;
int currColKE = 410;
extern char font[128][8];
int cursor[2] = {300, 300};
int underCursor[8][8];
bool initCursor = false;

void drawCursor(int x, int y) {
    int n = 0;
    int m = 0;
    if(initCursor) {
        for(int i=cursor[0]; i<cursor[0]+8; i++) {
            for(int j=cursor[1]; j<cursor[1]+8; j++) {
                fb[i][j] = underCursor[n][m];
                n++;
            }
            m++;
        }
    }
    initCursor = true;
    n = m = 0;
    cursor[0] = x;
    cursor[1] = y;
    for(int i=x; i<x+8; i++) {
        for(int j=y; j<y+8; j++) {
            underCursor[n][m] = fb[i][j];
            n++;
            fb[i][j] = 0x000F;
        }
        m++;
    }
}

void upBuffer(int type) {
    if(type ==0) {
        for(int i=0; i<480; i++) {
            for(int j=0; j<400; j++) {
                fb[ i ][ j ] = 0x9CEA;
            }
        }
        currLine=5;
        currCol=5;
    } else {
      for(int i=0; i<480; i++) {
          for(int j=410; j<800; j++) {
              fb[ i ][ j ] = 0x9CEA;
          }
      }
      currLineKE=5;
      currColKE=410;
    }

}

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


  for( int i = 0; i < 480; i++ ) {
    for( int j = 400; j < 405; j++ ) {
      fb[ i ][ j ] = 0x0000;
    }
  }

  for( int i = 480; i < 485; i++ ) {
    for( int j = 0; j < 800; j++ ) {
      fb[ i ][ j ] = 0x0000;
    }
  }

}


void carriageReturn(int type) {
  if(type ==0) {
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
    } else {
      currColKE = 410;
      currLineKE += 17;
    }

 return;
}

void drawLetter(char c, int type) {
  if (type == 0) {
     if(currLine > 460) upBuffer(type);
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
           currLine+=17;
       }
     for( int i=0; i<16; i++) {
         for(int j =0; j<12; j++) {
             fb[currLine+i][currCol+j] = 0x0000;
           }
       }
    } else {
     if(currLineKE > 460) upBuffer(type);
     for( int i=0; i<16; i++) {
         for(int j =0; j<12; j++) {
             fb[currLineKE+i][currColKE+j] = 0x9CEA;
           }
       }
       char* chr = font[c];
       for(uint8_t i=0; i<8; i++) {
           for(uint8_t j=0; j<8; j++) {
               if(chr[j] & (1<<i)) {
                   for(int m=0; m<2; m++) {
                       for(int n=0; n<2; n++) {
                           fb[2*j+currLineKE+m][2*i+currColKE+n] = 0x0000;
                       }
                   }
               }
           }
       }
       currColKE += 16;
       if(currColKE > 785) {
           currColKE = 410;
       }
    }

}

int deleteLetter(int consoleBuffer, int type) {
  for( int i=0; i<16; i++) {
      for(int j =0; j<16; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    if(currCol <= 16 && currLine > 17 && consoleBuffer > 0) {
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
  return consoleBuffer;
}

void drawString(char* c, int n, int type) {
    for(int i=0; i<n; i++) {
        if(c[i] =='\n') carriageReturn(type);
        else drawLetter(c[i], type);
    }
}





