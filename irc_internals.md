# IRC Internals

## Part 1: IRC Protocol

### 1.1 What is IRC?

IRC (Internet Relay Chat) is a text-based communication protocol designed for real-time messaging over the internet. It was created in 1988 by Jarkko Oikarinen and remains one of the oldest and most widely used chat protocols.

IRC is built around a simple idea: clients connect to a server, and the server relays messages between them. Users join named channels to have group conversations, or exchange private messages directly with other users.

Key characteristics of IRC:
- **Plain text protocol** — all communication is human-readable text over TCP
- **Client-server model** — clients never talk directly to each other; everything goes through the server

### 1.2 Text-Based vs Binary Protocols

Network protocols generally fall into two categories: **text-based** and **binary-based**.

A **binary protocol** encodes data as raw bytes. The meaning of each byte depends on its position and the protocol's specification. Examples include HTTP/2 and DNS. Binary protocols are compact and fast to parse, but not human-readable.

A **text-based protocol** encodes everything as plain readable text. Messages are strings that humans can read directly in a terminal. Examples include HTTP/1.1, SMTP, and IRC.

IRC is fully text-based. Every message sent between client and server is a readable string terminated by `\r\n`. This means you can connect to an IRC server using a basic tool like `netcat` and type commands by hand:

```
NICK hamza\r\n
USER hamza 0 * :Hamza\r\n
```

The server replies with readable text as well:

```
:server 001 hamza :Welcome to the IRC network\r\n
```

This simplicity is one of IRC's strengths — it is easy to debug, easy to implement, and easy to understand by reading raw traffic.

### 1.3 Message Format

Every IRC message follows a strict structure defined by the protocol. A message has up to three parts:

```
[':' prefix SPACE] command [params] '\r\n'
```

- **prefix** (optional) — identifies who sent the message. Usually the server name or a user in the form `nick!user@host`
- **command** — either a keyword (`PRIVMSG`, `JOIN`, `NICK`) or a 3-digit numeric code (`001`, `433`)
- **params** — up to 15 parameters. The last parameter can contain spaces if prefixed with `:`
- **`\r\n`** — every message ends with a carriage return and newline

**Examples:**

A client sending a message to a channel:
```
PRIVMSG #general :Hello everyone\r\n
```

A server forwarding that message (with prefix):
```
:hamza!hamza@localhost PRIVMSG #general :Hello everyone\r\n
```

A server sending a numeric reply:
```
:irc.example.com 001 hamza :Welcome to the IRC Network hamza!hamza@localhost\r\n
:irc.example.com 002 hamza :Your host is irc.example.com, running version 1.0\r\n
:irc.example.com 003 hamza :This server was created Jun 24 2026\r\n
:irc.example.com 004 hamza irc.example.com 1.0 io kst\r\n
```

The `:` before the last parameter tells the parser that everything after it — including spaces — is a single parameter.

The prefix is optional. When the server speaks directly — not on behalf of a user — it omits the prefix entirely. For example:

```
PING :server\r\n
```

When the prefix is present, it tells the client who the original sender is.

### 1.4 Connection Flow

When a client connects to an IRC server, it must go through a handshake before it can do anything. The steps are:

**1. Client sends PASS, NICK and USER**
```
PASS mypassword\r\n
NICK hamza\r\n
USER hamza 0 * :Hamza Boutaleb\r\n
```
- `PASS` provides the server password (required before anything else)
- `NICK` sets the client's nickname
- `USER` provides the username, and the real name (after `:`)

**2. Server validates**

The server checks:
- The password is correct
- The nickname is not already taken
- The nickname and username follow the allowed format

**3. Server sends welcome replies**

If everything is valid, the server responds with `001`:
```
:server 001 hamza :Welcome to the IRC network hamza\r\n
```

> Note: the server also sends `002`, `003`, and `004` after `001`, but they are informational only — they carry server version, creation date, and supported modes. The critical reply is `001`, which marks the client as registered.

After `001`, the client is considered **registered** and can now join channels, send messages, and use all other commands.

**4. If something goes wrong**

If the password is wrong:
```
:server 464 * :Password incorrect\r\n
```
If the nickname is taken:
```
:server 433 * hamza :Nickname is already in use\r\n
```

### 1.5 Channels

A channel is a named group conversation on an IRC server. Any registered user can join a channel and exchange messages with everyone else in it.

Channel names always start with `#`:
```
#general
#42school
#cpp
```

When a user sends a message to a channel, the server forwards it to every user currently in that channel. Users can join multiple channels at the same time.

A channel is more than just a name. It holds:

- **Topic** — a short description visible to all members when they join. By default any user can change it, but if the `t` mode is set, only the operator can.
- **Member list** — the list of users currently in the channel
- **Modes** — settings that control channel behavior (password protected, invite-only, etc.)
- **Operator** — a member with special privileges to manage the channel (kick users, change topic, set modes)

**Joining a channel**

A client sends:
```
JOIN #general\r\n
```

The server confirms by broadcasting the JOIN to everyone in the channel including the joining user:
```
:hamza!hamza@localhost JOIN #general\r\n
```

The server then sends the channel's topic and the list of current members to the new user.

If the channel is invite-only (`i` mode set), the client cannot join freely. They must first be invited by an operator using the `INVITE` command, otherwise the server rejects the join with:
```
:irc.example.com 473 hamza #general :Cannot join channel (+i)\r\n
```

If the channel has a password (`k` mode set), the client must provide it when joining:
```
JOIN #general mypassword\r\n
```
If the password is wrong or missing, the server rejects with:
```
:irc.example.com 475 hamza #general :Cannot join channel (+k)\r\n
```

**Leaving a channel**

A client sends:
```
PART #general :Goodbye\r\n
```

The server broadcasts the PART to everyone in the channel:
```
:hamza!hamza@localhost PART #general :Goodbye\r\n
```

The text after `:` is an optional leave message.

**Channel Modes**

A mode is a setting that controls the behavior of a channel. Modes are set by the operator using the `MODE` command.

The supported channel modes are:

| Mode | Name | Description |
|---|---|---|
| `i` | Invite-only | Only invited users can join |
| `t` | Topic restricted | Only operators can change the topic |
| `k` | Key | Channel requires a password to join |
| `o` | Operator | Grants or revokes operator privileges to a user |
| `l` | Limit | Sets a maximum number of users allowed in the channel |

**Setting a mode:**
```
MODE #general +k mypassword\r\n
```

**Unsetting a mode:**
```
MODE #general -k\r\n
```

The `+` sets a mode, `-` unsets it.

### 1.6 Commands

**PASS**

The `PASS` command is used to provide the server password before registering. It must be sent before `NICK` and `USER`.

Syntax:
```
PASS <password>
```

Example:
```
PASS mypassword\r\n
```

If the password is wrong, the server replies:
```
:irc.example.com 464 * :Password incorrect\r\n
```

**NICK**

The `NICK` command sets or changes the client's nickname. It is sent during registration and can also be used at any time to change the current nickname.

Syntax:
```
NICK <nickname>
```

Example during registration:
```
NICK hamza\r\n
```

Example changing nickname after registration:
```
NICK hamza42\r\n
```

The server broadcasts the change to all channels the user is in:
```
:hamza!hamza@localhost NICK hamza42\r\n
```

If the nickname is already taken:
```
:irc.example.com 433 * hamza :Nickname is already in use\r\n
```

**USER**

The `USER` command is sent once during registration to provide the client's username and real name. It must be sent together with `NICK` to complete the handshake.

Syntax:
```
USER <username> <mode> <unused> :<realname>
```

- `<username>` — the client's username
- `<mode>` — sets initial user modes at connection. Most clients send `0` to ignore it. For example, `8` sets the user as invisible. Basic server implementations often ignore this field entirely.
- `<unused>` — always `*`, ignored by the server
- `<realname>` — the client's real name, can contain spaces

Example:
```
USER hamza 0 * :Hamza Boutaleb\r\n
```

`USER` can only be sent once. Sending it again after registration is ignored by the server.

**JOIN**

The `JOIN` command is used to enter a channel. A client can join multiple channels at once.

Syntax:
```
JOIN <channel> [password]
```

Example:
```
JOIN #general\r\n
```

With a password:
```
JOIN #general mypassword\r\n
```

The server confirms by broadcasting the JOIN to everyone in the channel:
```
:hamza!hamza@localhost JOIN #general\r\n
```

The server then sends:
- The channel topic (`332`)
- The list of current members (`353`, `366`)

If the channel does not exist, the server creates it and the joining user becomes the operator automatically.

**PART**

The `PART` command is used to leave a channel.

Syntax:
```
PART <channel> [:<message>]
```

Example:
```
PART #general :Goodbye\r\n
```

The server broadcasts the PART to everyone in the channel:
```
:hamza!hamza@localhost PART #general :Goodbye\r\n
```

The leave message is optional. If omitted:
```
PART #general\r\n
```

**PRIVMSG**

The `PRIVMSG` command is used to send a message to a channel or directly to another user.

Syntax:
```
PRIVMSG <target> :<message>
```

- `<target>` — either a channel (`#general`) or a nickname (`hamza`)

Sending to a channel:
```
PRIVMSG #general :Hello everyone\r\n
```

The server broadcasts it to all members of the channel:
```
:hamza!hamza@localhost PRIVMSG #general :Hello everyone\r\n
```

Sending directly to a user:
```
PRIVMSG john :Hey, how are you?\r\n
```

The server forwards it only to that user:
```
:hamza!hamza@localhost PRIVMSG john :Hey, how are you?\r\n
```

**KICK**

The `KICK` command is used by an operator to forcefully remove a user from a channel.

Syntax:
```
KICK <channel> <nickname> [:<reason>]
```

Example:
```
KICK #general john :Stop spamming\r\n
```

The server broadcasts the KICK to everyone in the channel:
```
:hamza!hamza@localhost KICK #general john :Stop spamming\r\n
```

The kicked user is removed from the channel but is not banned — they can rejoin unless the channel is invite-only.

If the kicker is not an operator:
```
:irc.example.com 482 hamza #general :You're not channel operator\r\n
```

**INVITE**

The `INVITE` command is used by an operator to invite a user to a channel. It is required when the channel has the `i` (invite-only) mode set.

Syntax:
```
INVITE <nickname> <channel>
```

Example:
```
INVITE john #general\r\n
```

The server confirms to the inviter:
```
:irc.example.com 341 hamza john #general\r\n
```

The invited user receives:
```
:hamza!hamza@localhost INVITE john #general\r\n
```

If the target user is not on the server:
```
:irc.example.com 401 hamza john :No such nick\r\n
```

**TOPIC**

The `TOPIC` command is used to get or set the topic of a channel.

Syntax:
```
TOPIC <channel> [:<topic>]
```

Getting the current topic:
```
TOPIC #general\r\n
```

The server replies with the current topic:
```
:irc.example.com 332 hamza #general :Welcome to the general channel\r\n
```

If no topic is set:
```
:irc.example.com 331 hamza #general :No topic is set\r\n
```

Setting a new topic:
```
TOPIC #general :New topic here\r\n
```

The server broadcasts the change to everyone in the channel:
```
:hamza!hamza@localhost TOPIC #general :New topic here\r\n
```

If the `t` mode is set and the user is not an operator:
```
:irc.example.com 482 hamza #general :You're not channel operator\r\n
```

**MODE**

The `MODE` command is used by an operator to set or unset channel modes.

Syntax:
```
MODE <channel> <+/-><mode> [parameter]
```

Setting a mode:
```
MODE #general +k mypassword\r\n
```

Unsetting a mode:
```
MODE #general -k\r\n
```

The server broadcasts the change to everyone in the channel:
```
:hamza!hamza@localhost MODE #general +k mypassword\r\n
```

Some modes require a parameter:

| Mode | Set | Unset |
|---|---|---|
| `i` | `MODE #general +i` | `MODE #general -i` |
| `t` | `MODE #general +t` | `MODE #general -t` |
| `k` | `MODE #general +k mypassword` | `MODE #general -k` |
| `o` | `MODE #general +o john` | `MODE #general -o john` |
| `l` | `MODE #general +l 10` | `MODE #general -l` |

If the user is not an operator:
```
:irc.example.com 482 hamza #general :You're not channel operator\r\n
```

**QUIT**

The `QUIT` command is used to disconnect from the server.

Syntax:
```
QUIT [:<message>]
```

Example:
```
QUIT :Bye!\r\n
```

The server broadcasts the quit to all channels the user was in:
```
:hamza!hamza@localhost QUIT :Bye!\r\n
```

The quit message is optional. If omitted:
```
QUIT\r\n
```

After receiving `QUIT`, the server closes the connection with the client.

### 1.7 Reply Codes

Reply codes are 3-digit numeric codes the server sends to inform the client about the result of a command or the state of the server. They follow the same message format as regular commands.

Reply codes are grouped by category:

| Range | Category |
|---|---|
| `001–099` | Connection and registration replies |
| `200–399` | Informational replies |
| `400–499` | Client errors |
| `500–599` | Server errors |

The most important codes used in this project:

| Code | Name | Meaning |
|---|---|---|
| `001` | RPL_WELCOME | Client successfully registered |
| `331` | RPL_NOTOPIC | No topic is set on the channel |
| `332` | RPL_TOPIC | Channel topic |
| `341` | RPL_INVITING | User invited successfully |
| `353` | RPL_NAMREPLY | List of users in a channel |
| `366` | RPL_ENDOFNAMES | End of user list |
| `401` | ERR_NOSUCHNICK | No such nickname on the server |
| `433` | ERR_NICKNAMEINUSE | Nickname already taken |
| `464` | ERR_PASSWDMISMATCH | Wrong server password |
| `473` | ERR_INVITEONLYCHAN | Cannot join invite-only channel |
| `475` | ERR_BADCHANNELKEY | Wrong channel password |
| `482` | ERR_CHANOPRIVSNEEDED | Not a channel operator |

## Part 2: Code Architecture

### 2.1 Project Structure

The project is organized into three layers, each in its own directory:

```
irc/
├── src/
│   ├── main.cpp
│   ├── core/        — server, client, channel, message parsing
│   ├── network/     — socket, epoll, I/O handling
│   └── commands/    — one file per IRC command + reply codes
├── include/         — header files mirroring src/ structure
│   ├── core/
│   ├── network/
│   └── commands/
├── build/           — compiled objects (generated)
└── tests/           — test scripts
```

- **`core/`** — the heart of the server: manages clients, channels, and message parsing
- **`network/`** — handles low-level I/O: sockets, epoll, read/write results
- **`commands/`** — each IRC command has its own file; `Replies.cpp` centralizes all server reply formatting

### 2.2 Main Components

The server is built around three core classes: `Server`, `Client`, and `Channel`.

**Server**

The central class that owns everything. It holds:
- A map of connected clients (`fd → Client*`)
- A map of active channels (`name → Channel*`)
- The `CommandHandler` that dispatches incoming commands

It runs the main event loop, accepts new connections, reads incoming data, and routes it to the right client.

**Client**

Represents a connected user. It holds:
- The underlying socket
- An input buffer (incoming data not yet processed)
- An output buffer (data waiting to be sent)
- `ClientInfo` — the client's nickname, username, registration state

**Channel**

Represents a group conversation. It holds:
- The channel name, topic, password, and member limit
- A map of members with their roles (`Client* → ChannelRole`)
- An invite list for invite-only mode
- Flags for modes (`inviteOnly`, `topicLocked`)

It exposes a `broadcast()` method that sends a message to all members, with an optional exclude (used to skip the sender).

### 2.3 Design Patterns

**Command Pattern**

Each IRC command is implemented as its own class that inherits from `ICommand`:

```cpp
class ICommand {
public:
    virtual void execute(Client *client, const Message &msg, Context &ctx) = 0;
};
```

`CommandHandler` holds a map of command name to `ICommand*` and dispatches incoming messages to the right handler:

```cpp
std::map<std::string, ICommand *> _commands;
```

This makes adding a new command as simple as creating a new class and registering it — no changes needed elsewhere.

**Singleton**

`Epoll` is implemented as a singleton — only one epoll instance should exist for the entire server lifetime:

```cpp
static Epoll &instance();
```

The constructor is private, preventing any other part of the code from creating a second instance.

### 2.4 Request Lifecycle

This is the full journey of a message from the moment it arrives to the moment a reply is sent:

**1. Epoll detects activity**

The server runs a loop calling `Epoll::instance().wait()`. When a client socket has data to read, epoll returns its `fd`.

**2. Server reads the data**

`_handleRead(fd)` calls `recv()` on the client's socket and passes the raw bytes to `_processClient()`.

**3. Data is buffered**

The raw bytes are appended to the client's input `Buffer`. The buffer accumulates data until it contains a complete message ending with `\r\n`.

**4. Message is extracted and parsed**

`inBuffer().extractMessage()` pulls one complete message. `Message::parse()` splits it into prefix, command, and parameters.

**5. CommandHandler dispatches**

`_handler.handle(client, msg)` looks up the command name in its map and calls `execute()` on the matching `ICommand`, passing the client, message, and `Context`.

**6. Command executes and replies**

The command does its work and calls `client->send()` to queue a reply. The reply is written to the client's output buffer and flushed when epoll signals the socket is ready to write (`EPOLLOUT`).

### 2.5 Command Dispatch

When a message arrives, `CommandHandler::handle()` looks up the command name in its map:

```cpp
std::map<std::string, ICommand *>::iterator it = _commands.find(msg.command);
if (it == _commands.end())
    return; // unknown command, ignore
it->second->execute(client, msg, _ctx);
```

All commands are registered once at startup in `_registerCommands()`:

```cpp
_commands["PASS"]    = new PassCommand();
_commands["NICK"]    = new NickCommand();
_commands["JOIN"]    = new JoinCommand();
// ...
```

Each command class implements `ICommand::execute()` and receives:
- `client` — the client who sent the command
- `msg` — the parsed message with command and parameters
- `_ctx` — the shared context (clients map, channels map, server password)

If the command is unknown, the server silently ignores it.