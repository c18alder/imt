#include "dev/uart0.h"

#if !NETSTACK_CONF_WITH_IPV4
/* In case of IPv4: putchar() is defined by the SLIP driver */

#if MSP430_COOJA_PUTCHAR
volatile char* cooja_debug_ptr;
#endif /* ! MSP430_COOJA_PUTCHAR */

int
putchar(int c)
{
#if !MSP430_COOJA_PUTCHAR
  uart0_writeb((char)c);
#else /* ! MSP430_COOJA_PUTCHAR */
  static char buf[2] = {0};
  buf[0] = (char)c;
  do {cooja_debug_ptr = buf;}while(0);
#endif /* ! MSP430_COOJA_PUTCHAR */
  return c;
}
#endif /* ! NETSTACK_CONF_WITH_IPV4 */
