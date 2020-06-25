# Fifteen

The initial state of the field should be set in the code so that it can be easily changed.

Game process:
- when turned on, you must UART using ASCII graphics (i.e. symbols) to depict the initial state of the field. The field should be displayed the same for any width of the UART window (if the field fits into it at all).
- the player sends you one byte corresponding to the move, and:
  - symbol w - means move the empty cell up
  - character s - move down
  - character a - move left
  - character d - move right

- if such a move is permissible (i.e. the player is not trying to go beyond the field) - you must draw a new position for the field
- if such a move is unacceptable - i.e. the player is trying to go out of the field or the received byte does not correspond to the move at all - you should give an error message.
- if this move is a winning one (i.e., all the cells line up in the correct order) - you must draw a field and inform the player about the victory.

It uses UART1, pins PA9, PA10, 9600 baudrate, 8 data bits, 1 stop bit, no parity bit.

The result:

• The program implements the game in "Fifteen" using the UART-port.

• A static two-dimensional array of characters is used as a field, which can be easily changed directly in the program text. It would be optimal to implement an algorithm for shuffling the array, but I could not find or come up with one, so I decided not to focus on it.

• Used ‘*’ as an empty cell.

• Added a pause for convenience.

• A simple comparison with a “winning” array is used to check the victory conditions. Most likely there are more practical options.

• I did not use switch instead of big branching, because wanted to use continue to pause.
