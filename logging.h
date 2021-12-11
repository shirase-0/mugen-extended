#ifndef __MULOGGING__H
#define __MULOGGING__H

// This allows me the flexibility of removing all debug messages for actual releases, while still allowing me to
// run my own debug message function when I do want to see debug messages
#ifdef DEBUG
#define debug_print(msg, ...) do { if (DEBUG) mu_log_message(msg, ##__VA_ARGS__); } while (0)
#else
#define debug_print(msg, ...) {}
#endif

void mu_log_message(char *str,...);
void init_log_file();

#endif