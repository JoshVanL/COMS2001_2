#include "hilevel.h"


pcb_t pcb[ 50  ], *current = NULL;
int priority[ 20 ];
uint32_t count=0;
uint32_t icurrent = 0;
uint32_t pidNum = 0;

extern void     main_console();
extern uint32_t tos_console(); 

extern uint32_t tos_shared();
void* sharred_current = (void*) (tos_shared);

void scheduler( ctx_t* ctx  ) {
  int32_t next = 0;
  int hiPriority = priority[0];

  for (uint32_t i = 1; i <= count; i++) {
     if (priority[i] > hiPriority) {
        hiPriority = priority[i];
        next = i;
      } else {
        priority[i] += 3;
      }
  }

  priority[next] -= 2;
  memcpy( &pcb[ icurrent  ].ctx, ctx, sizeof( ctx_t  )  );
  memcpy( ctx, &pcb[ next ].ctx, sizeof( ctx_t  )  );
  current = &pcb[ next ];
  icurrent = next;

  char x = (pcb[ icurrent].pid + '0');
  PL011_putc( UART0, '\n', true ); 
  PL011_putc( UART0, x, true ); 
  PL011_putc( UART0, ':', true ); 
  PL011_putc( UART0, ' ', true ); 
	
  return;
}




void hilevel_handler_rst(  ctx_t* ctx ) {
  /* Configure the mechanism for interrupt handling by
  *
  * - configuring timer st. it raises a (periodic) interrupt for each 
  *   timer tick,
  * - configuring GIC st. the selected interrupts are forwarded to the 
  *   processor via the IRQ interrupt signal, then
  * - enabling IRQ interrupts.
  */
  
  TIMER0->Timer1Load  = 0x00100000; // select period = 2^20 ticks ~= 1 sec
  TIMER0->Timer1Ctrl  = 0x00000002; // select 32-bit   timer
  TIMER0->Timer1Ctrl |= 0x00000040; // select periodic timer
  TIMER0->Timer1Ctrl |= 0x00000020; // enable          timer interrupt
  TIMER0->Timer1Ctrl |= 0x00000080; // enable          timer
  
  GICC0->PMR          = 0x000000F0; // unmask all            interrupts
  GICD0->ISENABLER1  |= 0x00000010; // enable timer          interrupt
  GICC0->CTLR         = 0x00000001; // enable GIC interface
  GICD0->CTLR         = 0x00000001; // enable GIC distributor
  
  memset( &pcb[ 0  ], 0, sizeof( pcb_t  )  );
  pcb[ 0  ].pid      = 0;
  pcb[ 0  ].ctx.cpsr = 0x50;
  pcb[ 0  ].ctx.pc   = ( uint32_t  )( &main_console  );
  pcb[ 0  ].ctx.sp   = ( uint32_t  )( &tos_console   );

  priority[0] = 1;
  
int_enable_irq(); icurrent =0; //current = &pcb[ 0  ]; memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );
  current = &pcb[ 0  ]; 
  memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );
  return;
}
void hilevel_handler_irq( ctx_t* ctx ) {
  // Step 2: read  the interrupt identifier so we know the source.

  uint32_t id = GICC0->IAR;

  // Step 4: handle the interrupt, then clear (or reset) the source.
  if( id == GIC_SOURCE_TIMER0 ) {
    TIMER0->Timer1IntClr = 0x01;
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
  count++;
  pidNum++;
  memset( &pcb[ count  ], 0, sizeof( pcb_t  )  );
  pcb[ count  ].pid      = pidNum;
  pcb[ count  ].ctx.cpsr = 0x50;
  pcb[ count  ].ctx.pc   = ( uint32_t  )( prog  );
  pcb[ count  ].ctx.sp   = ( uint32_t  )( tos_userProgram );    

  priority[count] = 2;
  

  //scheduler(ctx);
  return;
}


void do_Exit (ctx_t* ctx ) {
  for (uint32_t i = icurrent; i<count; i++) {
    //pcb[i].pid = i;
    memcpy( &pcb[ i  ].pid, &pcb[ i+1 ].pid, sizeof( ctx_t  )  ); memcpy( &pcb[ i  ].ctx, &pcb[ i+1 ].ctx, sizeof( ctx_t  )  ); priority[i] = priority[i+1]; } count--;
  icurrent = 0;
  memcpy( ctx, &pcb[ icurrent  ].ctx, sizeof( ctx_t  )  );
  current = &pcb [ icurrent ];
  PL011_putc( UART0, 'H', true );
  return;
}

void do_Kill( ctx_t* ctx ) {
  uint32_t P;
  uint32_t pid = ctx->gpr[ 0 ];
  for(uint32_t i = 1; i <=count; i++) {
    if (pid == pcb[ i ].pid) {       
      P = i;
      break;
    }
  }
  for (uint32_t i = P; i<count; i++) {
    memcpy( &pcb[ i  ].pid, &pcb[ i+1 ].pid, sizeof( ctx_t  )  );
    memcpy( &pcb[ i  ].ctx, &pcb[ i+1 ].ctx, sizeof( ctx_t  )  );
    priority[i] = priority[i+1];
  }
  count--;
  icurrent = 0;
  PL011_putc( UART0, 'H', true );
  return;
}

    

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
     else if (fd == 3) { //write to shared memory
        sharred_current = (&tos_shared); 
         for( int i =0; i < n; i++) {
            memset( sharred_current++, *x++, sizeof(char)); 
        }
     }

     break;
    }
    case 0x02 : { //Read()
      int   fd = ( int    )( ctx->gpr[ 0  ]  );
      char*  x = ( char*  )( ctx->gpr[ 1  ]  );
      int    n = ( int    )( ctx->gpr[ 2  ]  );
       
      sharred_current = (&tos_shared); 

      if (fd == 3) {
       for( int i =0; i < n; i++) {
         memcpy(x++, sharred_current++, sizeof(char)); 
       }
      }
      break;
    }
    case 0x03 : { //Fork()
      PL011_putc( UART0, 'F', true );
      tos_userProgram = &tos_userProgram + 0x00010000; 
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
    default   : { // 0x?? => unknown/unsupported
      PL011_putc( UART0, 'E', true );
      PL011_putc( UART0, 'R', true );
      break;
    }
  
  }

  return;

}
