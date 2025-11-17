/* Include section */
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h> 
#include "raw_mode.h"

/* Macro section */
#define CTRL_KEY(k) ((k) & 0x1f)

/* Prototypes section */
char editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows();

/* Global Variables section */
struct editorConfig {
	struct termios orig_termios;
};

struct editorConfig E;


/* Init section */
int main() 
{
	enableRawMode();

	while (1)
	{
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return 0;
}


char editorReadKey()
{
	int nread;
	char c;
	while((nread = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}

/* Input */

void editorProcessKeypress()
{
	char c = editorReadKey();

	switch(c)
	{
	case CTRL_KEY('q'):
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
		break;
	}
}

/* output */

void editorDrawRows()
{
	int y;
	for (y = 0; y < 24; y++)
	{
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void editorRefreshScreen()
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}