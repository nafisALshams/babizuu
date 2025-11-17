#include <stdio.h>
#include <unistd.h>
#include <termios.h> 

void enableRawMode();

int main() 
{
	enableRawMode();
	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	// reading 1 byte from the std inp into the var c
	// when 'q' is read the program exits

	return 0;
}

void enableRawMode() 
// enabling raw mode instead of canonical/cooked mode

{
	struct termios raw;

	tcgetattr(STDIN_FILENO, &raw);
	// reading the current attributes into a struct

	raw.c_lflag &= ~(ECHO);
	// turning off the ECHO feature, so that each
	// typed key is not printed to the terminal

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	// passing the modified struct to write the new
	// terminal attributes back out
}