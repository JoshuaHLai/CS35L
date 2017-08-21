#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Constructor as described in spec and documentation */
/* Initialize the software rand64 implementation.  */
__attribute__((constructor)) extern void software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Destructor as described in spec and documentation */
/* Finalize the software rand64 implementation.  */
__attribute__((destructor)) extern void software_rand64_fini (void)
{
  fclose (urandstream);
}