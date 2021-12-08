#include "mu_headers.h"

#define TOKEN_DEFAULT_OPERATORS_COUNT 23
char token_default_comment_chars[] = ";";
char * token_default_operators[TOKEN_DEFAULT_OPERATORS_COUNT] =
{
	"~",
    "!",
    "+",
    "-",
    "=",
    "!=",
    "[",
    "]",
    "(",
    ")",
    "/",
    "$",
    ",",
    "<",
    ">",
    "<=",
    ">=",
    "&",
    "|",
    "^",
    "&&",
    "||",
    "*"
};

// Initialises the Tokenizer
Tokenizer *tokenizer_init(int buffer_size, char *comment_chars, char **operators, int operator_count)
{
	Tokenizer *tokenizer = (Tokenizer*) malloc(sizeof(Tokenizer));
	tokenizer->buffer_size = buffer_size;
	tokenizer->buffer = (char *) malloc(sizeof(char) * buffer_size);
	memset(tokenizer->buffer, 0, sizeof(char) * buffer_size);
	tokenizer->comment_chars = comment_chars ? comment_chars : token_default_comment_chars;

	if (operators)
	{
		tokenizer->operators = operators;
		tokenizer->operator_count = operator_count;
	}
	else
	{
		tokenizer->operators = token_default_operators;
		tokenizer->operator_count = operator_count;
	}

	tokenizer->num_operator_chars_read = 0;

	tokenizer->filename[0] = 0;
	tokenizer->file_buffer = NULL;
	tokenizer->file_size = 0;
	tokenizer->current_file_pos = 0;
	tokenizer->current_file_line = 0;
	tokenizer->last_line_pos = 0;

	tokenizer->buffer_is_next_token = false;
	tokenizer->at_end_of_line = false;
	tokenizer->at_end_of_file = false;

	tokenizer->return_negative_separately_from_number = false;
	tokenizer->is_case_sensitive = true;

	tokenizer->last_token_was_quoted_string = false;

	return tokenizer;
}

void free_tokenizer(Tokenizer *tokenizer)
{
	if(tokenizer->file_buffer)
	{
		free(tokenizer->file_buffer);
	}
	free(tokenizer->buffer);
	free(tokenizer);
}

// Opens file and reads into buffer
// Note to self: when modifying the value(s) stored in a pointer, there's no need to return the pointer, as the value has been changed in memory
bool mu_open_file(Tokenizer *tokenizer, const char* filename)
{
	if(tokenizer->file_buffer)
	{
		return false;
	}

	strcpy(tokenizer->filename, filename);

	FILE *file = fopen(filename, "rb");
	if(!file)
	{
		mu_log_message("Tokenizer: File %s not found", filename);
		return false;
	}

	fseek(file, 0, SEEK_END);
	tokenizer->file_size = ftell(file);
	if(!tokenizer->file_size)
	{
		return false;
	}
	tokenizer->file_buffer = (char *) malloc(sizeof(char) * tokenizer->file_size);

	fseek(file, 0, SEEK_SET);
	fread(tokenizer->file_buffer, tokenizer->file_size, 1, file);

	fclose(file);

	tokenizer->current_file_pos = 0;
	tokenizer->current_file_line = 0;
	tokenizer->last_line_pos = 0;

	tokenizer->buffer_is_next_token = false;
	tokenizer->at_end_of_line = false;
	tokenizer->at_end_of_file = false;

	return true;
}

bool mu_close_file(Tokenizer *tokenizer)
{
	if(!tokenizer->file_buffer)
	{
		return false;
	}

	free(tokenizer->file_buffer);
	mu_log_message("Freed block %p from Tokenizer", tokenizer->file_buffer);
	tokenizer->file_size = 0;
	tokenizer->filename[0] = 0;
	tokenizer->file_buffer = NULL;
	tokenizer->current_file_pos = 0;
	tokenizer->current_file_line = 0;
	tokenizer->last_line_pos = 0;

	tokenizer->buffer_is_next_token = false;
	tokenizer->at_end_of_line = false;
	tokenizer->at_end_of_file = false;

	return true;
}

const char *get_token(Tokenizer *tokenizer)
{
	//mu_log_message("Start of get_token; tokenizer->buffer: %s", tokenizer->buffer);
	if(!tokenizer->buffer_is_next_token)
	{
		if(tokenizer->at_end_of_file || !tokenizer->file_buffer)
		{
			return NULL;
		}

		tokenizer->last_token_was_quoted_string = false;
		tokenizer->at_end_of_line = false;
		bool have_hit_second_whitespace = false;
		char *buffer_temp = tokenizer->buffer;
		//mu_log_message("After buffer_temp declaration; tokenizer->buffer: %s", tokenizer->buffer);

		// Iterate over file_buffer
		while(!tokenizer->at_end_of_file && (buffer_temp - tokenizer->buffer < tokenizer->buffer_size))
		{
			char c = tokenizer->file_buffer[tokenizer->current_file_pos++];
			//mu_log_message("After assignment from file_buffer; c: %c, tokenizer->buffer: %s", c, tokenizer->buffer);

			if(tokenizer->current_file_pos >= tokenizer->file_size)
			{
				tokenizer->at_end_of_file = true;
				break;
			}

			// Read in quoted strings literally with no processing
			if(c == '\"')
			{
				// If there is something already in the buffer, back up and return what is already there
				if(buffer_temp > tokenizer->buffer)
				{
					tokenizer->current_file_pos--;
				}
				else
				{
					while(true)
					{
						c = tokenizer->file_buffer[tokenizer->current_file_pos++];

						if(tokenizer->current_file_pos >= tokenizer->file_size)
						{
							tokenizer->at_end_of_file = true;
							break;
						}

						if(c == '\"')
						{
							// Check for end of line/EOF
							while(true)
							{
								c = tokenizer->file_buffer[tokenizer->current_file_pos++];

								if(tokenizer->current_file_pos >= tokenizer->file_size)
								{
									tokenizer->at_end_of_file = true;
									break;
								}

								if(c == '\n')
								{
									tokenizer->at_end_of_line = true;
									tokenizer->current_file_line++;
									tokenizer->last_line_pos = tokenizer->current_file_pos;
								}

								if(!isspace(c))
								{
									// Rewind file pointer by one
									tokenizer->current_file_pos--;
									break;
								}
							}
							tokenizer->last_token_was_quoted_string = true;
							break;
						}
						// I don't understand this section
						*buffer_temp = c;
						buffer_temp++;
					}
				}
				break;
			}

			// Skip chars after comment chars till the end of the line
			bool hit_comment_char = false;
			int comment_char_count = tokenizer->comment_chars ? strlen(tokenizer->comment_chars) : 0;
			for(int a = 0; a < comment_char_count; a++)
			{
				if(c == tokenizer->comment_chars[a])
				{
					hit_comment_char = true;
					while(true)
					{
						c = tokenizer->file_buffer[tokenizer->current_file_pos++];
						//mu_log_message("%c", c);
						if(tokenizer->current_file_pos >= tokenizer->file_size)
						{
							tokenizer->at_end_of_file = true;
							break;
						}

						if(c == '\n')
						{
							tokenizer->current_file_line++;
							tokenizer->last_line_pos = tokenizer->current_file_pos;
							tokenizer->at_end_of_line = true;
							break;
						}
					}
				}
			}

			if(hit_comment_char)
			{
				if(buffer_temp > tokenizer->buffer)
				{
					break;
				}
			}
			else if(!isspace(c))
			{
				// Check for operators and return them as separate strings from other things, even if no whitespace between
				bool hit_operator = false;
				bool break_to_return_token = false;
				bool negative_number = false;

				if((c == '-') && !tokenizer->return_negative_separately_from_number)
				{
					char next_c = tokenizer->file_buffer[tokenizer->current_file_pos];
					if(isdigit(next_c) || (next_c == '.'))
					{
						negative_number = true;
					}
				}

				if(!negative_number)
				{
					// For every operator
					for(int a = 0; a < tokenizer->operator_count; a++)
					{
						bool prev_chars_matched = (strlen(tokenizer->operators[a]) > tokenizer->num_operator_chars_read);
						for(int b = 0; prev_chars_matched && (b < tokenizer->num_operator_chars_read); b++)
						{
							if(tokenizer->buffer[b] != (tokenizer->operators[a])[b])
							{
								prev_chars_matched = false;
							}
						}

						if(prev_chars_matched && (c == (tokenizer->operators[a])[tokenizer->num_operator_chars_read]))
						{
							hit_operator = true;
							// If there is something already in the buffer, back up and return what is already there
							if(!tokenizer->num_operator_chars_read && (buffer_temp > tokenizer->buffer))
							{
								//mu_log_message("Guess where our problem is!!!!!!!!!!!");
								// Rewind file pointer by one
								tokenizer->current_file_pos--;
								break_to_return_token = true;
							}
							else
							{
								// I still don't understand this bit
								*buffer_temp = c;
								buffer_temp++;
								tokenizer->num_operator_chars_read++;
							}
							break;
						}
					}
				}

				if(break_to_return_token)
				{
					break;
				}

				if(!hit_operator)
				{
					// Check whitespace after token to see if end of line/EOF bits should be set
					if(!tokenizer->num_operator_chars_read && !have_hit_second_whitespace)
					{
						// Seriously what does this do .-.
						*buffer_temp = c;
						buffer_temp++;
					}
					else
					{
						// Rewind file pointer by one
						tokenizer->current_file_pos--;
						break;
					}
				}
			}
			else
			{
				if(c == '\n')
				{
					tokenizer->at_end_of_line = true;
					tokenizer->current_file_line++;
					tokenizer->last_line_pos = tokenizer->current_file_pos;
				}
				have_hit_second_whitespace = (buffer_temp > tokenizer->buffer);
			}
		}
		*buffer_temp = 0;
		tokenizer->num_operator_chars_read = 0;
	}
	tokenizer->buffer_is_next_token = false;

	//mu_log_message("CHECK: tokenizer->buffer = %s", tokenizer->buffer);

	// if(strcmp(tokenizer->buffer, "170") == 0)
	// {
	// 	mu_log_message("CHECK: tokenizer->buffer = %s", tokenizer->buffer);
	// }
	
	return tokenizer->buffer;
}

bool get_token_check(Tokenizer *tokenizer, char *dest_string, int max_length)
{
	if(!get_token(tokenizer))
	{
		return false;
	}
	strncpy(dest_string, tokenizer->buffer, max_length > tokenizer->buffer_size ? tokenizer->buffer_size : max_length);

	return true;
}

bool check_token_consume(Tokenizer *tokenizer, const char *string_to_look_for)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return false;
		}
	}

	// This has switched to using system-specific implementations of strcmp for case-insensitive comparisons
	// If I end up figuring out a way to patch this to use my own function, I should probably do that for
	// true cross-compatibility
#ifdef _WIN32 // If we're on windows
	bool result = tokenizer->is_case_sensitive ? (strcmp(string_to_look_for, tokenizer->buffer) == 0) : (stricmp(string_to_look_for, tokenizer->buffer) == 0);
#else // If we're on a POSIX-compliant system (UNTESTED)
	bool result = tokenizer->is_case_sensitive ? (strcmp(string_to_look_for, tokenizer->buffer) == 0) : (strcasecmp(string_to_look_for, tokenizer->buffer) == 0);
#endif
	tokenizer->buffer_is_next_token = !result;
	return result;
}

bool check_token_no_consume(Tokenizer *tokenizer, const char *string_to_look_for)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return false;
		}
	}

	// This has switched to using system-specific implementations of strcmp for case-insensitive comparisons
	// If I end up figuring out a way to patch this to use my own function, I should probably do that for
	// true cross-compatibility
#ifdef _WIN32 // If we're on windows
	bool result = tokenizer->is_case_sensitive ? (strcmp(string_to_look_for, tokenizer->buffer) == 0) : (stricmp(string_to_look_for, tokenizer->buffer) == 0);
#else // if we're on a POSIX-compliant system (UNTESTED)
	bool result = tokenizer->is_case_sensitive ? (strcmp(string_to_look_for, tokenizer->buffer) == 0) : (strcasecmp(string_to_look_for, tokenizer->buffer) == 0);
#endif
	tokenizer->buffer_is_next_token = true;
	return result;
}

float mu_get_float(Tokenizer *tokenizer)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return 0.0f;
		}
	}

	tokenizer->buffer_is_next_token = false;
	return (float) atof(tokenizer->buffer);
}

int mu_get_int(Tokenizer *tokenizer)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return 0;
		}
	}

	tokenizer->buffer_is_next_token = false;
	return atoi(tokenizer->buffer);
}

bool check_token_is_number(Tokenizer *tokenizer)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return false;
		}
	}

	tokenizer->buffer_is_next_token = true;

	if(tokenizer->last_token_was_quoted_string)
	{
		return false;
	}

	int buffer_len = strlen(tokenizer->buffer);
	char *c = tokenizer->buffer;
	//mu_log_message("check_token_is_number; Current value of tokenizer->buffer: %s", c);

	for(int a = 0; a < buffer_len; a++)
	{
		if(((*c < '0') || (*c > '9')) && (*c != '.') && !(!tokenizer->return_negative_separately_from_number && (a == 0) && (*c == '-') && (buffer_len > 1)))
		{
			return false;
		}
		c++;
	}

	return (buffer_len > 0);
}

bool check_token_is_quoted_string(Tokenizer *tokenizer)
{
	if(!tokenizer->buffer_is_next_token)
	{
		if(!get_token(tokenizer))
		{
			return false;
		}
	}

	tokenizer->buffer_is_next_token = true;
	return tokenizer->last_token_was_quoted_string;
}