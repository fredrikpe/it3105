#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Print the string in reverse, and reverse lower case and capital letters.
// But only 10 chars max.
unsigned char* reverse(char *string)
{
	// Make buffer
	unsigned char *mem = malloc(10);
	int i, lastChar;
	
	// Make a work copy of the string
	memcpy(mem, string, strlen(string));
	
	// Search for the first non null char
	for(i = 10; i >= 0; i--)
	{
		// Strings are terminated by null
		// Find the first character
		if(mem[i] != 0)
		{
			lastChar = i;
			break;
		}
	}
	
	// Swap lower case <-> capital letters
	for(i = 0; i <= lastChar; i++)
	{
		// Handle lower case
		if(mem[i] >= 'a') {
			mem[i] -= 'a' - 'A';
		} else {
			// Handle upper case
			if(mem[i] >= 'A') {
				mem[i] += 'a' - 'A';
			}
		}
	}
	
	
	// Make a return array
	unsigned char *returnString = malloc(lastChar);
	
	// Reverse the string, and store it in the return array
	for(i = lastChar; i >= 0; i--)
		returnString[i] = mem[i];
	
	// Free the memory buffer
	free(mem);
	
	// Return the modified string
	return returnString;
}

int main(int argc, char *argv[])
{
	int i;
	// Iterate over all the arguments
	for(i = argc-1; i > 0; i--) {
		char *returnString = reverse(argv[i]);
		// Print the new string
		printf("%s\n", returnString);
		
		// Free the returned string
		free(returnString);
	}
	return 0;
}
