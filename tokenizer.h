#ifndef __MUTOKENIZER__
#define __MUTOKENIZER__

Tokenizer *tokenizer_init(int buffer_size, char *comment_chars, char **operators, int operator_count);
void free_tokenizer(Tokenizer *tokenizer);

bool mu_open_file(Tokenizer *tokenizer, const char* filename);
bool mu_close_file(Tokenizer *tokenizer);

const char *get_token(Tokenizer *tokenizer);
bool get_token_check(Tokenizer *tokenizer, char *dest_string, int max_length);
bool check_token_consume(Tokenizer *tokenizer, const char *string_to_look_for); // Default: true
bool check_token_no_consume(Tokenizer *tokenizer, const char *string_to_look_for);
bool check_token_is_number(Tokenizer *tokenizer);
bool check_token_is_quoted_string(Tokenizer *tokenizer);

int mu_get_int(Tokenizer *tokenizer);
float mu_get_float(Tokenizer *tokenizer);

// These might not be needed, as they're simple getters and setters
bool at_end_of_line();
bool at_end_of_file();

void set_is_case_sensitive(bool b);
void set_return_negative_separately_from_number(bool b);

const char *mu_get_file_name();
int get_line_number();
int get_colummn_number();

#endif