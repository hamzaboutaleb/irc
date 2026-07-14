*This project has been created as part of the 42 curriculum by hboutale, rhafidi, yoessedr.*

# ft_irc

## Description

`ft_irc` is a from-scratch implementation of an IRC (Internet Relay Chat) server in C++98. The goal of the project is to understand how a real network protocol works under the hood by building a server capable of handling multiple clients at once, without ever blocking, using a single `poll`-style event loop (here implemented with `epoll`).

The server speaks enough of the IRC protocol (RFC 1459-ish) to be usable with a real IRC client (e.g. HexChat, irssi, WeeChat) or with raw tools like `netcat`. It supports:

- Client registration (`PASS`, `NICK`, `USER`)
- Channels: `JOIN`, `PART`, `TOPIC`, `INVITE`, `KICK`, `MODE`
- Messaging: `PRIVMSG`, `PING`
- Channel operator privileges and modes (invite-only, topic restriction, key, user limit, operator status)
- Graceful disconnection (`QUIT`), including handling of clients that disconnect uncleanly (`SIGPIPE` safe)

The codebase is organized around a small command dispatcher (`ICommand` / `CommandHandler`) so each IRC command lives in its own class, plus a `network` layer wrapping non-blocking sockets and `epoll`, and a `core` layer holding the server, client, and channel state.

## Instructions

### Compilation

```sh
make        # builds the ./irc binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

The project is built with `c++` using `-std=c++98 -Wall -Wextra -Werror`, so no external dependencies are required beyond a standard C++ toolchain on Linux (uses `epoll`).

### Running the server

```sh
./irc <port> <password>
```

- `<port>` must be between 1024 and 65535.
- `<password>` must be non-empty and contain only letters, digits and punctuation.

Example:

```sh
./irc 6667 mypass
```

### Connecting

With `netcat`:

```sh
nc 127.0.0.1 6667
PASS mypass
NICK hamza
USER hamza 0 * :Hamza Boutaleb
JOIN #general
PRIVMSG #general :hello everyone
```

Or with any standard IRC client, pointing it at `127.0.0.1:<port>` with the server password set to `<password>`.

### Test bot

`irc_bot.py` is a small Python IRC client that connects to the server and replies to a few basic commands (`!ping`, `!hello`, `!time`, `!echo <text>`). Useful for quick manual testing:

```sh
python3 irc_bot.py 127.0.0.1 6667 mypass BotName #general
```

## Resources

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Modern IRC documentation (modern.ircdocs.horse)](https://modern.ircdocs.horse/)
- `man epoll`, `man 7 socket`, `man 2 poll` — for the non-blocking I/O event loop
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- `irc_internals.md` (in this repository) — internal notes written while building the project, covering the IRC protocol, message format, and connection flow

### AI usage

An AI assistant was used during this project as a learning and productivity aid, specifically for:

- Explaining IRC protocol details and RFC semantics (message framing, numeric replies, registration handshake)
- Debugging edge cases around non-blocking sockets, partial reads/writes, and `SIGPIPE` handling

All core IRC protocol logic, the command dispatch architecture, and the `epoll`-based networking code were designed and implemented by the team.
