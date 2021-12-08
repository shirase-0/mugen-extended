#include "mu_headers.h"

FILE *p_log_file = NULL;

void mu_log_message(char *str,...)
{
	char message[255]; 			// Extend length if required

	va_list ap;					// Pointer to argument list
	va_start(ap, str);			// Parses string for variables
	vsprintf(message, str, ap);	// Converts symbols to actual numbers
	va_end(ap);					// Garbage collection

#ifdef _XBOX
  p_log_file = fopen("d:\\log.txt","a+");
#else
  p_log_file = fopen("log.txt","a+");
#endif

	fprintf(p_log_file, message);
	fprintf(p_log_file, "\n");
	fclose(p_log_file);

	printf(message);
	printf("\n");  
}

void init_log_file()
{
	p_log_file = fopen("log.txt","w");
	fclose(p_log_file);
	mu_log_message("Logging: Init Successful");
}