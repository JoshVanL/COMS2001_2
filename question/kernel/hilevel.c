#include "hilevel.h"


pcb_t pcb[ 4  ], *current = NULL;
uint32_t count=0;
uint32_t icurrent = 0;


void scheduler( ctx_t* ctx  ) {
  if  (count > 0) { 
    if (icurrent+1 > count) {
      memcpy( &pcb[ icurrent  ].ctx, ctx, sizeof( ctx_t  )  );
      memcpy( ctx, &pcb[ 0 ].ctx, sizeof( ctx_t  )  );
      current = &pcb[ 0  ];
      icurrent =0;
    } else {
      memcpy( &pcb[ icurrent  ].ctx, ctx, sizeof( ctx_t  )  );
      memcpy( ctx, &pcb[ icurrent+1 ].ctx, sizeof( ctx_t  )  );
      current = &pcb[ icurrent+1  ];
      icurrent++;
    }
  }
	//else if ( current == &pcb[ 2  ]  ) {
    //memcpy( &pcb[ 2  ].ctx, ctx, sizeof( ctx_t  )  );
    //memcpy( ctx, &pcb[ 3  ].ctx, sizeof( ctx_t  )  );
    //current = &pcb[ 3  ];
  
	//}
	//else if ( current == &pcb[ 3  ]  ) {
    //memcpy( &pcb[ 3  ].ctx, ctx, sizeof( ctx_t  )  );
    //memcpy( ctx, &pcb[ 0  ].ctx, sizeof( ctx_t  )  );
    //current = &pcb[ 0  ];
  
	//}
	
  return;
}

extern void     main_P3();
extern void     main_P4();
extern void     main_P5();
extern void     main_console();
extern uint32_t tos_console(); 




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
  

  int_enable_irq();

  icurrent =0;
  //current = &pcb[ 0  ]; memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );
  current = &pcb[ 0  ]; 
  memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );
  return;
}

void hilevel_handler_irq( ctx_t* ctx ) {
  // Step 2: read  the interrupt identifier so we know the source.

  uint32_t id = GICC0->IAR;

  // Step 4: handle the interrupt, then clear (or reset) the source.
  if( id == GIC_SOURCE_TIMER0 ) {
    PL011_putc( UART0, '\n', true ); 
    PL011_putc( UART0, 'T', true ); 
    PL011_putc( UART0, '\n', true ); 
    TIMER0->Timer1IntClr = 0x01;
	scheduler(ctx);
  }

  // Step 5: write the interrupt identifier to signal we're done.

  GICC0->EOIR = id;

  return;
}

extern uint32_t _heap_start;
void* tos_userProgram = &_heap_start;

void do_Exec (ctx_t* ctx ) {
  void* prog = ( void* )( ctx ->gpr[ 0 ] );
  count++;
  memset( &pcb[ count  ], 0, sizeof( pcb_t  )  );
  pcb[ count  ].pid      = count;
  pcb[ count  ].ctx.cpsr = 0x50;
  pcb[ count  ].ctx.pc   = ( uint32_t  )( prog  );
  pcb[ count  ].ctx.sp   = ( uint32_t  )( tos_userProgram );    
  memcpy( &pcb[ icurrent  ].ctx, ctx, sizeof( ctx_t  )  );
  memcpy( ctx, &pcb[ count  ].ctx, sizeof( ctx_t  )  );
  current = &pcb[ count  ];
  icurrent = count;
  return;
}


void do_Exit (ctx_t* ctx ) {
  for (uint32_t i = icurrent; i<count; i++) {
    memcpy( &pcb[ i  ].pid, &pcb[ i+1 ].ctx, sizeof( ctx_t  )  );
    memcpy( &pcb[ i  ].ctx, &pcb[ i+1 ].ctx, sizeof( ctx_t  )  );
  }
  count--;
  icurrent = 0;
  memcpy( ctx, &pcb[ icurrent  ].ctx, sizeof( ctx_t  )  );
  current = &pcb [ icurrent ];
  PL011_putc( UART0, 'H', true );
  PL011_putc( UART0, '\n', true );
  return;
}

void do_Kill(pid_t pid) {
  for(uint32_t i = 1; i <=count; i++) {
    if (pid == pcb[ i ].pid) {
        PL011_putc( UART0, 'P', true );
        icurrent = i;
        current = &pcb[ i ];
        do_Exit(&pcb[i].ctx);
        break;
    }
  }
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
   
     for( int i = 0; i < n; i++  ) {
  	   PL011_putc( UART0, *x++, true  );
    }

     ctx->gpr[ 0  ] = n;
     break;
    }
    case 0x03 : { //Fork()
      PL011_putc( UART0, 'F', true );
      PL011_putc( UART0, '\n', true );
      //tos_userProgram = ( void* ) (malloc( 0x00001000) );
      tos_userProgram = &tos_userProgram + 0x00010000; 
      break;
    }
    case 0x04 : {  //Exit()
      PL011_putc( UART0, 'X', true );
      PL011_putc( UART0, '\n', true );
      do_Exit( ctx );
      break;
    }     
    case 0x05 : { //EXEC()
      PL011_putc( UART0, 'E', true );
      PL011_putc( UART0, '\n', true );
      do_Exec(ctx);
      break;
    }
    case 0x06 : { //KILL()
      PL011_putc( UART0, 'K', true );
      PL011_putc( UART0, '\n', true );
      do_Kill(ctx->gpr[0]);
      break;
    }
    default   : { // 0x?? => unknown/unsupported
      PL011_putc( UART0, 'E', true );
      PL011_putc( UART0, 'R', true );
      PL011_putc( UART0, '\n', true );
      break;
    }
  
  }

  return;

}
