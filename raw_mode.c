#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h> 
#include <errno.h>
#include "raw_mode.h"


extern struct termios orig_termios;

void enableRawMode() 
// enabling raw mode instead of canonical/cooked mode

{
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die ("tcgetattr");
	// keeping the original terminal attributes

	atexit(disableRawMode);
	// registering disableRawMode() function to be called
	// automatically when the program exits 

	struct termios raw = orig_termios;
	// making a copy of the original termios attributes
	// struct to the raw struct

	// tcgetattr(STDIN_FILENO, &raw);
	// reading the current attributes into a struct
	
	raw.c_iflag &= ~(BRKINT |ICRNL | INPCK | ISTRIP | IXON);
	// IXON flag does ctrl + s (XOFF) which pauses transmission
	// ctrl + q (XON) which resumes the transmission
	// ICRNL stops the translation of carriage return and new line 
	// BRKINT flags a break condition which causes SIGINT
	// signal to be sent to the program
	// INPCK flag enables parity checking (not modern)
	// ISTRIP causes the 8th bit of each byte to be stripped
	// automatically setting it to 0

	raw.c_oflag &= ~(OPOST);
	// OPOST flag responsible is for the post processing of the output

	raw.c_cflag |= ~(CS8);
	// CS8 is a bit mask with multiple bits and not a flag
	// sets the char size to 8 bits per byte (not modern)


	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN |ISIG);
	// turning off the ECHO feature, so that each
	// typed key is not printed to the terminal
	// ICANON local flag ensures we read line byte by
	// byte and not line by line 
	// ISIG flag will stop the ctrl + c (SIGINT) and
	// ctrl + z (SIGTSTP) signal from going to the 
	// current process which might suspend the terminal
	// IEXTEN flag will fix ctrl + v issue and ctrl + o
	// issue for macos

	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	// c_cc filed indexes [VMIN] & [VTIME] and cc stands for control
	// characters, an array of bytes that control various terminal
	// settings 

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
	// passing the modified struct to write the new
	// terminal attributes back out
}

void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
	{
		die("tcsetattr");
	}
	//disabling raw mode when the program exits
}


void die (const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	// perror() looks at the global errno variable and prints the 
	// error message for that. Also prints the string to it.
	exit(1);
}