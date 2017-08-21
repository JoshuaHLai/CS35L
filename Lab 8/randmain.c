#include "randcpuid.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <dlfcn.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
		perror (argv[1]);
      else
		valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  void* libsw;
  void* libhw;
  char* error = NULL;

/* Implement code for dynamic linking using dlopen() and dlsym() */
  if (rdrand_supported ())
    {
      /*Following what was described as each of the variables given.
      This block will work with the hardware.*/
      libhw = dlopen("randlibhw.so", RTLD_NOW);
      libsw = NULL;
      if (libhw == NULL)
      {
      	fprintf(stderr, "Cannot open library.");
      	exit(1);
      }

      initialize = dlsym(libhw, "hardware_rand64_init");
      error = dlerror();
      if (error)
      {
      	fprintf(stderr, "Cannot initialize hardware");
      	exit(1);
      }

      initialize();

      rand64 = dlsym(libhw, "hardware_rand64");
      if (error)
      {
      	fprintf(stderr, "Cannot locate hardware_rand64 function");
      	exit(1);
      }

      finalize = dlsym(libhw, "hardware_rand64_fini");
      error = dlerror();
      if (error)
      {
      	fprintf(stderr, "Cannot finalize hardware");
      	exit(1);
      }

      finalize();
    }
  else
    {
      /*Following what was described as each of the variables given.
      This block of code will work with the software.*/
      libsw = dlopen("randlibsw.so", RTLD_NOW);
      libhw = NULL;
      if (libsw == NULL)
      {
      	fprintf(stderr, "Did not load library corrrectly.");
      	exit(1);
      }

      initialize = dlsym(libsw, "software_rand64_init");
      error = dlerror();
      if (error)
      {
      	fprintf(stderr, "Cannot initialize software");
      	exit(1);
      }

      rand64 = dlsym(libsw, "software_rand64");
      error = dlerror();
      if (error)
      {
      	fprintf(stderr, "Cannot locate softwareware_rand64 function");
      	exit(1);
      }

      finalize = dlsym(libsw, "software_rand64_fini");
      error = dlerror();
      if (error)
      {
      	fprintf(stderr, "Cannot finalize software");
      	exit(1);
      }
    }

  if (libsw == NULL)
  {
  	char hardware_output[3] = {'H', 'W', '\0'};
  	printf("%s\n", hardware_output);
  }
  else
  {
  	char software_output[3] = {'S', 'W', '\0'};
  	printf("%s\n", software_output);
  }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      long long new_outbytes = outbytes;
      /* if (fwrite(&x, 1, outbytes, stdout) != outbytes) */
      if (!printf ("%llu\n", x))
	  {
        output_errno = errno;
        break;
      }
      //printf ("%llu\n", x);
      nbytes -= new_outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }

  /* Use dlclose() in order to clean up before exiting */
  if (libsw == NULL)
  {
  	dlclose(libhw);
  }
  else
  {
  	dlclose(libsw);
  }

  return 0;
}
