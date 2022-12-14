#include "mu_headers.h"

char token_default_comment_chars[] = ";";
char *token_default_operators[TOKEN_DEFAULT_OPERATORS_COUNT] =
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

// Initialise the Tokenizer
Tokenizer *tokenizer_init(int buffer_size, char *comment_chars, char **operators, int operator_count)
{
	Tokenizer *tok = (Tokenizer*) malloc(sizeof(Tokenizer));
	tok->buffer_size = buffer_size;
	tok->buffer = (char*) calloc(buffer_size, sizeof(char));
	tok->comment_chars = comment_chars ? comment_chars : token_default_comment_chars;

	if (operators) // Currently unused code path
	{
		tok->operators = operators;
		tok->operator_count = operator_count;
	}
	else
	{
		tok->operators = token_default_operators;
		tok->operator_count = operator_count;
	}

	tok->num_operator_chars_read = 0;
	tok->filename[0] = 0;
	tok->file_buffer = NULL;
	tok->filesize = 0;
	tok->cur_file_pos = 0;
	tok->cur_file_line = 0;
	tok->last_line_pos = 0;
	tok->buffer_is_next_token = false;
	tok->at_end_of_line = false;
	tok->at_eof = false;
	tok->negative_separate_from_number = false;
	tok->case_sensitive = true;
	tok->last_token_was_quoted_string = false;

	return tok;
}

void free_tokenizer(Tokenizer *tok)
{
	if(tok->file_buffer)
	{
		free(tok->file_buffer);
	}
	free(tok->buffer);
	free(tok);
}

// Opens file and reads into buffer
bool mu_open_file(Tokenizer *tok, const char* filename)
{
	// If a file is currently in the buffer, don't open another one
	if(tok->file_buffer)
	{
		return false;
	}

	strcpy(tok->filename, filename);
	FILE *file = fopen(filename, "rb");
	if(!file)
	{
		debug_print("Tokenizer: File %s not found", filename);
		return false;
	}

	// Get the file size
	fseek(file, 0, SEEK_END);
	tok->filesize = ftell(file);
	if(!tok->filesize)
	{
		return false;
	}

	// Store the entire file in the file buffer
	tok->file_buffer = (char*) malloc(sizeof(char) * tok->filesize);
	fseek(file, 0, SEEK_SET);
	fread(tok->file_buffer, tok->filesize, 1, file);
	fclose(file);

	tok->cur_file_pos = 0;
	tok->cur_file_line = 0;
	tok->last_line_pos = 0;
	tok->buffer_is_next_token = false;
	tok->at_end_of_line = false;
	tok->at_eof = false;

	return true;
}

bool mu_close_file(Tokenizer *tok)
{
	if(!tok->file_buffer)
	{
		return false;
	}

	free(tok->file_buffer);
	debug_print("Freed block %p from Tokenizer", tok->file_buffer);
	tok->filesize = 0;
	tok->filename[0] = 0;
	tok->file_buffer = NULL;
	tok->cur_file_pos = 0;
	tok->cur_file_line = 0;
	tok->last_line_pos = 0;
	tok->buffer_is_next_token = false;
	tok->at_end_of_line = false;
	tok->at_eof = false;

	return true;
}

bool mu_check_eof(Tokenizer *tok)
{
	if(tok->cur_file_pos >= tok->filesize)
	{
		tok->at_eof = true;
		return true;
	}
	return false;
}

bool mu_check_newline(Tokenizer *tok, char c)
{
	if(c == '\n')
	{
		tok->at_end_of_line = true;
		tok->cur_file_line++;
		tok->last_line_pos = tok->cur_file_pos;
		return true;
	}
	return false;
}

const char *get_token(Tokenizer *tok)
{
	if(!tok->buffer_is_next_token)
	{
		if(tok->at_eof || !tok->file_buffer)
		{
			return NULL;
		}

		tok->last_token_was_quoted_string = false;
		tok->at_end_of_line = false;
		bool have_hit_second_whitespace = false;
		char *buffer_temp = tok->buffer;

		// Iterate over file buffer
		// buffer_temp shrinks throughout this process, hence the check for < buffer_size
		while(!tok->at_eof && (buffer_temp - tok->buffer < tok->buffer_size))
		{
			char c = tok->file_buffer[tok->cur_file_pos++];

			if(mu_check_eof(tok))
			{
				break;
			}

			if(c == '\"')
			{
				if(buffer_temp > tok->buffer)
				{
					tok->cur_file_pos--;
				}
				else
				{
					while(true)
					{
						c = tok->file_buffer[tok->cur_file_pos++];

						if(mu_check_eof(tok))
						{
							break;
						}

						if(c == '\"')
						{
							while(true)
							{
								c = tok->file_buffer[tok->cur_file_pos++];

								if(mu_check_eof(tok))
								{
									break;
								}

								mu_check_newline(tok, c);

								if(!isspace(c))
								{
									tok->cur_file_pos--;
									break;
								}
							}
							tok->last_token_was_quoted_string = true;
							break;
						}
						// Advance the buffer using pointer magic
						// CHECK: Replace this with strstr?
						// Probably no need, this method is likely faster
						*buffer_temp = c;
						buffer_temp++;
					}
				}
				break;
			}

			// Skip chars after comment chars till the end of the line
			bool hit_comment_char = false;
			int comment_char_count = tok->comment_chars ? strlen(tok->comment_chars) : 0;
			for(int a = 0; a < comment_char_count; a++)
			{
				if(c == tok->comment_chars[a])
				{
					hit_comment_char = true;
					while(true)
					{
						c = tok->file_buffer[tok->cur_file_pos++];

						if(mu_check_eof(tok))
						{
							break;
						}

						if(mu_check_newline(tok, c))
						{
							break;
						}
					}
				}
			}

			if(hit_comment_char)
			{
				if(buffer_temp > tok->buffer) // Not sure how this is possible, experiment with removing this check?
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

				// Check for negative number
				if((c == '-') && !tok->negative_separate_from_number)
				{
					char next_c = tok->file_buffer[tok->cur_file_pos]; // isn't this just the same char?
					if(isdigit(next_c) || (next_c == '.'))
					{
						negative_number = true;
					}
				}

				if(!negative_number)
				{
					// TODO: check what goes into tok->operators at runtime
					// For every operator
					for(int a = 0; a < tok->operator_count; a++)
					{
						bool prev_chars_matched = (strlen(tok->operators[a]) > tok->num_operator_chars_read);
						// TODO: Figure out what this does at runtime
						for(int b = 0; prev_chars_matched && (b < tok->num_operator_chars_read); b++)
						{
							if(tok->buffer[b] != (tok->operators[a])[b])
							{
								prev_chars_matched = false;
							}
						}

						if(prev_chars_matched && (c == (tok->operators[a])[tok->num_operator_chars_read]))
						{
							hit_operator = true;

							// If there is something already in the buffer, back up and return what is already there
							if(!tok->num_operator_chars_read && (buffer_temp > tok->buffer))
							{
								// Rewind file pointer by one
								tok->cur_file_pos--;
								break_to_return_token = true;
							}
							else
							{
								// Pointer magic 2.0, check earlier in the function for more details
								*buffer_temp = c;
								buffer_temp++;
								tok->num_operator_chars_read++;
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
					if(!tok->num_operator_chars_read && !have_hit_second_whitespace)
					{
						// Pointer magic 3.0 (see above for details)
						*buffer_temp = c;
						buffer_temp++;
					}
					else
					{
						// Rewind file pointer by one
						tok->cur_file_pos--;
						break;
					}
				}
			}
			else
			{
				mu_check_newline(tok, c);
				have_hit_second_whitespace = (buffer_temp > tok->buffer);
			}
		}
		*buffer_temp = 0;
		tok->num_operator_chars_read = 0;
	}
	tok->buffer_is_next_token = false;

	//mu_log_message("%s", tok->buffer);
	return tok->buffer;
}

bool get_token_check(Tokenizer *tok, char *dest_str, int max_len)
{
	if(!get_token(tok))
	{
		return false;
	}
	strncpy(dest_str, tok->buffer, max_len > tok->buffer_size ? tok->buffer_size : max_len);

	return true;
}

bool check_token(Tokenizer *tok, const char *str_to_look_for, bool consume)
{
	if(!tok->buffer_is_next_token)
	{
		if(!get_token(tok))
		{
			return false;
		}
	}

	// I've switched to using system-specific implementations of strcmp for case-insensitive comparisons
	// If I end up figuring out a way to patch this to use my own function, I should probably do that for
	// true cross-compatibility
	// TODO: Replace this with my own function, or make this implementation more readable
#ifdef _WIN32 // If we're on Windows
	bool result = tok->case_sensitive ? (strcmp(str_to_look_for, tok->buffer) == 0) : (stricmp(str_to_look_for, tok->buffer) == 0);
#else // If we're on a POSIX-compliant system (UNTESTED)
	bool result = tok->case_sensitive ? (strcmp(str_to_look_for, tok->buffer) == 0) : (strcasecmp(str_to_look_for, tok->buffer) == 0);
#endif

	if(consume)
	{
		tok->buffer_is_next_token = !result;
	}
	else
	{
		tok->buffer_is_next_token = true;
	}

	return result;
}

float mu_get_float(Tokenizer *tok)
{
	if(!tok->buffer_is_next_token)
	{
		if(!get_token(tok))
		{
			return 0.0f;
		}
	}

	tok->buffer_is_next_token = false;
	return (float) atof(tok->buffer);
}

int mu_get_int(Tokenizer *tok)
{
	if(!tok->buffer_is_next_token)
	{
		if(!get_token(tok))
		{
			return 0;
		}
	}

	tok->buffer_is_next_token = false;
	return atoi(tok->buffer);
}

bool is_token_number(Tokenizer *tok)
{
	if(!tok->buffer_is_next_token)
	{
		if(!get_token(tok))
		{
			return false;
		}
	}

	tok->buffer_is_next_token = true;

	if(tok->last_token_was_quoted_string)
	{
		return false;
	}

	int buffer_len = strlen(tok->buffer);
	char *c = tok->buffer;

	for(int a = 0; a < buffer_len; a++)
	{
		if(((*c < '0') || (*c > '9')) &&
			(*c != '.') &&
			!(!tok->negative_separate_from_number && (a == 0) && (*c == '-') && (buffer_len > 1)))
		{
			return false;
		}
		c++;
	}

	return (buffer_len > 0);
}

bool is_token_quoted_str(Tokenizer *tok)
{
	if(!tok->buffer_is_next_token)
	{
		if(!get_token(tok))
		{
			return false;
		}
	}

	tok->buffer_is_next_token = true;
	return tok->last_token_was_quoted_string;
}