#include "hilevel.h"
#include <stdio.h>

pcb_t pcb[ 1 ], *current = NULL;

void scheduler() {

}

extern void main_P1();
extern uint32_t tos_P1();

void hilevel_handler_rst( ctx_t* ctx ) {
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
  pcb[ 0  ].pid      = 1;
  pcb[ 0  ].ctx.cpsr = 0x50;
  pcb[ 0  ].ctx.pc   = ( uint32_t  )( &main_P1  );
  pcb[ 0  ].ctx.sp   = ( uint32_t  )( &tos_P1   );
  current = &pcb[ 0  ]; memcpy( ctx, &current->ctx, sizeof( ctx_t  )  );

  int_enable_irq();
  
  while( 1 ) {
    for( int i = 0; i < 0x20000000; i++  ) {
   	  asm volatile( "nop"  );
    }
	asm volatile( "svc 0" );
	//PL011_putc( UART0, 'F', true   );  
  }

  return;
}

void hilevel_handler_irq() {
  // Step 2: read  the interrupt identifier so we know the source.
	uint32_t id = GICC0->IAR; // Step 4: handle the interrupt, then clear (or reset) the source.  
  if( id == GIC_SOURCE_TIMER0 ) {
    PL011_putc( UART0, 'T', true ); TIMER0->Timer1IntClr = 0x01;
  }

  if( id == GIC_SOURCE_UART0  ) {
    uint8_t x = PL011_getc( UART0, true  );

    PL011_putc( UART0, 'K',                      true  ); 
    PL011_putc( UART0, '<',                      true  ); 
    PL011_putc( UART0, itox( ( x >> 4  ) & 0xF  ), true  ); 
    PL011_putc( UART0, itox( ( x >> 0  ) & 0xF  ), true  ); 
    PL011_putc( UART0, '>',                      true  ); 

    UART0->ICR = 0x10;
  
  }

  // Step 5: write the interrupt identifier to signal we're done.

  GICC0->EOIR = id;

  return;
}


void hilevel_handler_svc() {
  /* Each time execution reaches this point, we are tasked with handling
   * a supervisor call (aka. software interrupt, i.e., a trap or system 
   * call).
   */

  PL011_putc( UART0, 'F', true  );  

  return;
}
