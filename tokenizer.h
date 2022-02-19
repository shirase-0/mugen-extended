#ifndef __MUTOKENIZER__
#define __MUTOKENIZER__

Tokenizer *tokenizer_init(int buffer_size, char *comment_chars, char **operators, int operator_count);
void free_tokenizer(Tokenizer *tok);

bool mu_open_file(Tokenizer *tok, const char* filename);
bool mu_close_file(Tokenizer *tok);

bool mu_check_eof(Tokenizer *tok);
bool mu_check_newline(Tokenizer *tok, char c);

const char *get_token(Tokenizer *tok);
bool get_token_check(Tokenizer *tok, char *dest_str, int max_len);

float mu_get_float(Tokenizer *tok); 
int mu_get_int(Tokenizer *tok);

// When this function is tested, remove check_token_consume and check_token_no_consume
bool check_token(Tokenizer *tok, const char *str_to_look_for, bool consume); 
//bool check_token_consume(Tokenizer *tokenizer, const char *string_to_look_for); // Default: true
//bool check_token_no_consume(Tokenizer *tokenizer, const char *string_to_look_for);
bool is_token_number(Tokenizer *tok);
bool is_token_quoted_str(Tokenizer *tok);


// Currently unused
void set_is_case_sensitive(bool b);
void set_return_negative_separately_from_number(bool b);

const char *mu_get_file_name();
int get_line_number();
int get_colummn_number();

#endif