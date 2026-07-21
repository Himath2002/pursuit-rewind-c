#include "pursuit_rewind/terminal.h"

#include <errno.h>
#include <termios.h>
#include <unistd.h>

static bool read_one_byte(char *key)
{
    ssize_t bytes_read;

    do {
        bytes_read = read(STDIN_FILENO, key, 1);
    } while (bytes_read < 0 && errno == EINTR);

    return bytes_read == 1;
}

bool terminal_read_key(char *key)
{
    struct termios original_mode;
    struct termios immediate_mode;
    bool read_succeeded;

    if (key == NULL) {
        return false;
    }
    if (!isatty(STDIN_FILENO)) {
        return read_one_byte(key);
    }
    if (tcgetattr(STDIN_FILENO, &original_mode) != 0) {
        return false;
    }

    immediate_mode = original_mode;
    immediate_mode.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    immediate_mode.c_cc[VMIN] = 1;
    immediate_mode.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &immediate_mode) != 0) {
        return false;
    }

    read_succeeded = read_one_byte(key);
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &original_mode);
    return read_succeeded;
}

