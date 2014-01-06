C-v:: move to next screen
M-x:: move to previous screen
C-l:: move screen to center of cursor

C-p:: previous line
C-n:: next line
C-b:: backward
C-f:: forward

M-f:: Move forward a word
M-b:: Move backword a word

C-a:: Move to beginning of line
C-e:: Move to end of line

M-a:: Move to beginning of sentence
M-e:: Move to end of sentence

M-<:: Move to beginning of text
M->:: Move to end of text

C-u <count> <Command>:: Repeat <Command> <count> times
e.g C-u 8 C-e
C-u 8 * -> To print 8 '*'

C-g:: Stop a command

C-x 1:: Get rid of all other windows

C-x generally refers to windows, and buffers

C-d:: Delete character after cursor
M-d:: Kill next word after cursor

C-k:: Kill from cursor position till end of line
M-k:: Kill from cursor position till end of sentence

Visual mode in Emacs
C-<Spc> <move cursor till text you want to select> C-w:: To kill that block of text

kill:: can be reinserted
delete:: cannot be reinserted

Reinsertion of killed is called 'yanking'.

C-y:: yanking
C-y then M-y:: yank previous stuff that was yanked

C-/:: undo

C-x C-f:: find a file
C-x C-s:: save a file
C-x C-s <name>:: Save to name
C-x s:: save some buffers

C-x C-b:: see open buffers
C-x b:: switch between open buffers
C-x b <buffer name>:: switch to buffer <buffer name>

M-x <name of command>:: run that command
e.g M-x repl s<Enter>:: search and replace

Emacs has different major modes. Some of them are meant for different languages
and texts, such as Lisp mode, text mode etc.

Each major mode makes a few commands behaves differently.

To switch between modes:
M-x text-mode<Enter>

To view documentation on your major mode:
C-h m

Minor modes are minor modifications of major modes.
e.g AutoFill mode. It will break long lines into smaller ones
M-x auto-fill-mode
C-x f <- to set the margin
M-q to auto-refill


Searching
C-s <search>
C-r <-- reverse

C-s again to search next occurence
<del> for previous occurence

Multiple windows
C-x 2 <-- this will open another side window with the element placed on the
bottom 

To switch between the 2 windows: C-x o

C-x 4 C-f <- open another file

C-x k <- to close a buffer