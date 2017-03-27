#include "hilevel.h"


pcb_t pcb[ 50  ], *current = NULL;
int priority[ 50 ];
uint32_t count=0;
uint32_t icurrent = 0;
uint32_t pidNum = 0;
uint32_t timer =0;
uint32_t active_pids[50];

uint16_t fb[600][800];

int currLine = 5;
int currCol = 5;

extern void     main_console();
extern uint32_t tos_console(); 

extern uint32_t tos_shared();
uint32_t* sharred_current = (uint32_t*) (tos_shared);

extern char font[128][8];

void scheduler( ctx_t* ctx  ) {
  bool changed=false;
  int32_t next = 0;
  int hiPriority = priority[0];

  for (uint32_t i = 1; i < count; i++) {
    if (priority[i] > hiPriority) {
        hiPriority = priority[i];
        next = i;
        changed=true;
    }
  }

  for (uint32_t i = 0; i < count; i++) priority[i] +=1;

  priority[next] = 0;

  memcpy( &pcb[ icurrent  ].ctx, ctx, sizeof( ctx_t  )  );
  memcpy( ctx, &pcb[ next ].ctx, sizeof( ctx_t  )  );
  current = &pcb[ next ];
  icurrent = next;

  if(changed) {
    char* x;
    uint32_t num = pcb[ icurrent].pid %10;
    x[0] = '0' +  num;
    num = (pcb[ icurrent].pid - num)/10;
    x[1] = '0' + num;

    PL011_putc( UART0, '\n', true ); 
    PL011_putc( UART0, x[1], true ); 
    PL011_putc( UART0, x[0], true ); 
    PL011_putc( UART0, ':', true ); 
    PL011_putc( UART0, ' ', true ); 
  }
	
  return;
}


void printZero() {

PL011_putc( UART0, '\n', true ); 
PL011_putc( UART0, '0', true ); 
PL011_putc( UART0, '0', true ); 
PL011_putc( UART0, '$', true ); 
PL011_putc( UART0, ' ', true ); 

return;
}

void change_toConsole ( ctx_t* ctx) {
printZero();
return;

}

void drawLetter(char c) {
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    PL011_putc( UART0, c, true ); 
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
    if(currCol > 785) {
        currCol = 5;
        currLine += 17;
    }
  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x0000;
        }
    }
}

void deleteLetter() {
  for( int i=0; i<16; i++) {
      for(int j =0; j<16; j++) {
          fb[currLine+i][currCol+j] = 0x9CEA;
        }
    }
    if(currCol <= 16 && currLine > 17) {
        currCol = 785;
        currLine -= 17;
    } else if( currCol > 5) {
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
        drawLetter(c[i]);
    }
}




void hilevel_handler_rst(  ctx_t* ctx ) {

  SYSCONF->CLCD      = 0x2CAC;     // per per Table 4.3 of datasheet
  LCD->LCDTiming0    = 0x1313A4C4; // per per Table 4.3 of datasheet
  LCD->LCDTiming1    = 0x0505F657; // per per Table 4.3 of datasheet
  LCD->LCDTiming2    = 0x071F1800; // per per Table 4.3 of datasheet

  LCD->LCDUPBASE     = ( uint32_t )( &fb );

  LCD->LCDControl    = 0x00000020; // select TFT   display type
  LCD->LCDControl   |= 0x00000008; // select 16BPP display mode
  LCD->LCDControl   |= 0x00000800; // power-on LCD controller
  LCD->LCDControl   |= 0x00000001; // enable   LCD controller

  /* Configure the mechanism for interrupt handling by
   *
   * - configuring then enabling PS/2 controllers st. an interrupt is
   *   raised every time a byte is subsequently received,
   * - configuring GIC st. the selected interrupts are forwarded to the 
   *   processor via the IRQ interrupt signal, then
   * - enabling IRQ interrupts.
   */

  PS20->CR           = 0x00000010; // enable PS/2    (Rx) interrupt
  PS20->CR          |= 0x00000004; // enable PS/2 (Tx+Rx)
  PS21->CR           = 0x00000010; // enable PS/2    (Rx) interrupt
  PS21->CR          |= 0x00000004; // enable PS/2 (Tx+Rx)

  uint8_t ack;

        PL050_putc( PS20, 0xF4 );  // transmit PS/2 enable command
  ack = PL050_getc( PS20       );  // receive  PS/2 acknowledgement
        PL050_putc( PS21, 0xF4 );  // transmit PS/2 enable command
  ack = PL050_getc( PS21       );  // receive  PS/2 acknowledgement

  /* Configure the mechanism for interrupt handling by
  *
  * - configuring timer st. it raises a (periodic) interrupt for each 
  *   timer tick,
  * - configuring GIC st. the selected interrupts are forwarded to the 
  *   processor via the IRQ interrupt signal, then
  * - enabling IRQ interrupts.
  */
  
  TIMER0->Timer1Load  = 0x00030000; // select period = 2^20 ticks ~= 1 sec
  TIMER0->Timer1Ctrl  = 0x00000002; // select 32-bit   timer
  TIMER0->Timer1Ctrl |= 0x00000040; // select periodic timer
  TIMER0->Timer1Ctrl |= 0x00000020; // enable          timer interrupt
  TIMER0->Timer1Ctrl |= 0x00000080; // enable          timer
  
  GICC0->PMR          = 0x000000F0; // unmask all            interrupts
  GICD0->ISENABLER1  |= 0x00000010; // enable timer          interrupt
  GICD0->ISENABLER1 |= 0x00300000; // enable PS2          interrupts
  GICC0->CTLR         = 0x00000001; // enable GIC interface
  GICD0->CTLR         = 0x00000001; // enable GIC distributor
  
  memset( &pcb[ 0  ], 0, sizeof( pcb_t  )  );
  pcb[ 0  ].pid      = 0;
  pcb[ 0  ].ctx.cpsr = 0x50;
  pcb[ 0  ].ctx.pc   = ( uint32_t  )( &main_console  );
  pcb[ 0  ].ctx.sp   = ( uint32_t  )( &tos_console   );

  priority[0] = 1;
  
  int_enable_irq(); //current = &pcb[ 0  ]; memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );
  current = &pcb[ 0  ]; 
  memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );

  active_pids[0] =0;

  count=1;

  PL011_putc( UART0, '\n', true ); 
  for(uint32_t i=0; i<20; i++) PL011_putc( UART0, '~', true ); 


  // Write example red/green/blue test pattern into the frame buffer.

  for( int i = 0; i < 600; i++ ) {
    for( int j = 0; j < 800; j++ ) {
      fb[ i ][ j ] = 0x9CEA;
    }
  }

  for( int i=0; i<16; i++) {
      for(int j =0; j<12; j++) {
          fb[currLine+i][currCol+j] = 0x0000;
        }
    }
  icurrent =0;

  return;
}

void hilevel_handler_irq( ctx_t* ctx ) {
  // Step 2: read  the interrupt identifier so we know the source.

  uint32_t id = GICC0->IAR;


  if     ( id == GIC_SOURCE_PS20 ) {
    int x = PL050_getc( PS20 );
    char c = decode(x);
    if(c == '#') deleteLetter();
    else if(c != '~') drawLetter(c);
    //for (int i=0; i<30; i++) {
    //    drawLetter(c, i*17+40, 40);
    //    c++;
    //}
    PL011_putc( UART0, '<',                      true ); 
    PL011_putc( UART0, itox( ( x >> 4 ) & 0xF ), true ); 
    PL011_putc( UART0, itox( ( x >> 0 ) & 0xF ), true ); 
    PL011_putc( UART0, '>',                      true ); 
  }
  else if( id == GIC_SOURCE_PS21 ) {
    uint8_t x = PL050_getc( PS21 );

    //PL011_putc( UART0, '1',                      true );  
    //PL011_putc( UART0, '<',                      true ); 
    //PL011_putc( UART0, itox( ( x >> 4 ) & 0xF ), true ); 
    //PL011_putc( UART0, itox( ( x >> 0 ) & 0xF ), true ); 
    //PL011_putc( UART0, '>',                      true ); 
  }

  // Step 4: handle the interrupt, then clear (or reset) the source.
  if( id == GIC_SOURCE_TIMER0 ) {
    TIMER0->Timer1IntClr = 0x01;
    timer++;
  }
  scheduler(ctx);

  // Step 5: write the interrupt identifier to signal we're done.

  GICC0->EOIR = id;

  return;
}

extern uint32_t _heap_start;
void* tos_userProgram = &_heap_start;

void do_Exec (ctx_t* ctx ) {
  void* (*prog) = ( void* )( ctx->gpr[ 0 ] );
  char** (stra) = (char**) (ctx->gpr[1]);
  uint32_t argc = 0;
  while(stra[argc] != NULL) argc++;
  char* argv[argc];
  for(int i=0; i<3; i++)argv[i] = stra[i];

  pidNum++;
  active_pids[count] = pidNum;
  memset( &pcb[ count  ], 0, sizeof( pcb_t  )  );
  pcb[ count  ].pid      = pidNum;
  pcb[ count  ].ctx.cpsr = 0x50;
  pcb[ count  ].ctx.pc   = ( uint32_t  )( prog  );
  pcb[ count  ].ctx.sp   = ( uint32_t  )( tos_userProgram );    
  pcb[ count  ].ctx.gpr[0]   = ( uint32_t  )( argc );    
  pcb[ count  ].ctx.gpr[1]   = ( uint32_t  )( argv );    
  count++;

  priority[count] = 2;
  return;
}


void do_Exit (ctx_t* ctx ) {
  for (uint32_t i = icurrent; i<count; i++) {
    //pcb[i].pid = i;
    memcpy( &pcb[ i  ].pid, &pcb[ i+1 ].pid, sizeof( ctx_t  )  ); 
    priority[i] = priority[i+1]; 
    active_pids[i] = active_pids[i+1];
  } 

  count--;
  icurrent = 0;
  memcpy( ctx, &pcb[ icurrent  ].ctx, sizeof( ctx_t  )  );
  current = &pcb [ icurrent ];
  PL011_putc( UART0, 'H', true );

  return;
}

void do_Kill( ctx_t* ctx ) {
  uint32_t P;
  bool valid = false;
  uint32_t pid = ctx->gpr[ 0 ];
  for(uint32_t i = 1; i <count; i++) {
    if (pid == pcb[ i ].pid) {       
      valid = true;
      P = i;
      break;
    }
  }
  if(valid) {
    for (uint32_t i = P; i<count; i++) {
      memcpy( &pcb[ i  ].pid, &pcb[ i+1 ].pid, sizeof( ctx_t  )  );
      memcpy( &pcb[ i  ].ctx, &pcb[ i+1 ].ctx, sizeof( ctx_t  )  );
      priority[i] = priority[i+1];
      active_pids[i] = active_pids[i+1];
    }
    count--;
    icurrent = 0;
    PL011_putc( UART0, 'H', true );
  } else {
    PL011_putc( UART0, 'E', true );
  }
  return;
}

void do_KillAll (ctx_t* ctx) {
  for(uint32_t i=count-1; i>0; i--) {
      ctx->gpr[ 0 ] = active_pids[i];
      do_Kill( ctx);
  }
  PL011_putc( UART0, 'A', true );

  return;
}

    

bool flag_share = false;
uint32_t share_loc[20] = {0};

void hilevel_handler_svc( ctx_t* ctx, uint32_t id ) {
  /* Each time execution reaches this point, we are tasked with handling
   * a supervisor call (aka. software interrupt, i.e., a trap or system 
   * call).
   */

  switch( id  ) {
    case 0x00 : { // 0x00 => yield()
      scheduler( ctx  );
      break;
    }
    case 0x01 : { // 0x01 => write( fd, x, n  )
     int   fd = ( int    )( ctx->gpr[ 0  ]  );
     char*  x = ( char*  )( ctx->gpr[ 1  ]  );
     int    n = ( int    )( ctx->gpr[ 2  ]  );

   
     if (fd == 1) { //write to console
       for( int i = 0; i < n; i++  ) {
  	     PL011_putc( UART0, *x++, true  );
       }
       ctx->gpr[ 0  ] = n;
     }   
     break;
    }
    case 0x02 : { //Read()
      break;
    }
    case 0x03 : { //Fork()
      PL011_putc( UART0, 'F', true );
      tos_userProgram += 0x00010000; 
      break;
    }
    case 0x04 : {  //Exit()
      PL011_putc( UART0, ' ', true );
      PL011_putc( UART0, 'X', true );
      do_Exit( ctx );
      break;
    }     
    case 0x05 : { //EXEC()
      if (&ctx->gpr[0] != NULL) {
        PL011_putc( UART0, 'E', true );
        do_Exec(ctx);
      }
      break;
    }
    case 0x06 : { //KILL()
      PL011_putc( UART0, 'K', true );
      do_Kill(ctx);
      break;
    }
    case 0x07 : { //Share init
      sharred_current += 0x00010000;
      share_loc[0] +=1;
      share_loc[share_loc[0]] = (uint32_t) sharred_current;
      ctx->gpr[0] = share_loc[0];
      break;
    }
    case 0x08 : { //Share() Read-Write
      priority[icurrent] = 20;
      int   fd = ( int    )( ctx->gpr[ 0  ]  );
      uint32_t pnt = ( uint32_t  )( ctx->gpr[ 1  ]  );
      uint32_t*  x = ( uint32_t*  )( ctx->gpr[ 2  ]  );
      uint32_t  n = ( uint32_t  )( ctx->gpr[ 3  ]  );

      uint32_t* curr = (uint32_t*) (share_loc[pnt]);
    
      if (fd == 0) {
         memcpy(&curr[0], x,  n*sizeof(int));
         priority[icurrent] = 0;
      }
      if (fd == 1) {
        memcpy(x, &curr[0], n*sizeof(int));
        priority[icurrent] = 0;
      }
      flag_share = false;
      priority[icurrent] = 0;
      break;
    }
    case 0x09 : { //Semaphore
      if (flag_share) {
          ctx->gpr[0] = 1;
          if (priority[icurrent] >1) priority[icurrent]-=2;
      }
      else {
          flag_share = true;
          ctx->gpr[0] = 0;
          priority[icurrent] += 10;
     }
      break;
    }
    case 0x10 : { //Return timer val
      ctx->gpr[0] = timer;
      break;
    }
    case 0x11 : { //Console has made command
      change_toConsole(ctx);
      break;
    }
    case 0x12 : { //Console has made command
      PL011_putc( UART0, 'K', true );
      do_KillAll(ctx);
      break;
    }
    case 0x13 : { //Get processes count
      ctx->gpr[0] = count;
      break;
    }
    case 0x14 : { //Get processes pid

      int n = ctx->gpr[0];
      ctx->gpr[0] = active_pids[n];

      break;
    }
    case 0x15 : { //Set current processs priority

      int n = ctx->gpr[0];
      priority[icurrent] = n;
      break;
    }
    default   : { // 0x?? => unknown/unsupported
      PL011_putc( UART0, 'E', true );
      PL011_putc( UART0, 'R', true );
      break;
    }
  
  }

  return;
}
