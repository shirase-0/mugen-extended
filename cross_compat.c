#include "mu_headers.h"

// PLEASE NOTE
// No longer in use, this file should only be included in the project once I have a better understanding
// of how to implement the cross-compatible features I need

// Note to self: This is probably highly inefficient, so it should be replaced with a more efficient
// version if it's going to be used a lot
int strcmp_case_insensitive(const char *s1, const char *s2)
{
	int i = 0;
	char *s1_temp = malloc(sizeof(char) * strlen(s1));
	char *s2_temp = malloc(sizeof(char) * strlen(s2));
	int comparison = -1;

	while(s1[i])
	{
		s1_temp[i] = (char)tolower(s1[i]);
		i++;
	}

	i = 0;
	while(s2[i])
	{
		s2_temp[i] = (char)tolower(s2[i]);
		i++;
	}

	comparison = strcmp(s1_temp, s2_temp);
	free(s1_temp);
	free(s2_temp);

	return comparison;
}