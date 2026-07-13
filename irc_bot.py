#!/usr/bin/env python3
"""
Simple IRC bot written in Python.
Connects to any RFC1459-ish IRC server (like your custom C++ ircserv).

Usage:
    python3 irc_bot.py <host> <port> <password> <nickname> <channel>

Example:
    python3 irc_bot.py 127.0.0.1 6667 mypass BotName #general
"""

import socket
import sys
import time


class IRCBot:
    def __init__(self, host, port, password, nickname, channel):
        self.host = host
        self.port = int(port)
        self.password = password
        self.nickname = nickname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.buffer = ""

    def connect(self):
        self.sock.connect((self.host, self.port))
        self.send_raw(f"PASS {self.password}")
        self.send_raw(f"NICK {self.nickname}")
        self.send_raw(f"USER {self.nickname} 0 * :{self.nickname} bot")

    def send_raw(self, msg):
        self.sock.sendall(f"{msg}\r\n".encode("utf-8"))

    def send_msg(self, target, msg):
        self.send_raw(f"PRIVMSG {target} :{msg}")

    def join_channel(self, channel):
        self.send_raw(f"JOIN {channel}")

    def read_lines(self):
        """Yield complete lines from the socket buffer as they arrive."""
        data = self.sock.recv(4096).decode("utf-8", errors="ignore")
        if not data:
            raise ConnectionError("Server closed the connection")
        self.buffer += data
        while "\r\n" in self.buffer:
            line, self.buffer = self.buffer.split("\r\n", 1)
            yield line

    def handle_line(self, line):
        print(f"<< {line}")

        # Respond to server PING to stay connected
        if line.startswith("PING"):
            token = line.split(":", 1)[-1]
            self.send_raw(f"PONG :{token}")
            return

        # Once we see the welcome message (001), join our channel
        if " 001 " in line:
            self.join_channel(self.channel)
            return

        # Parse PRIVMSG lines: :nick!user@host PRIVMSG #chan :message
        if "PRIVMSG" in line:
            try:
                prefix, rest = line[1:].split(" PRIVMSG ", 1)
                sender = prefix.split("!", 1)[0]
                target, message = rest.split(" :", 1)
            except ValueError:
                return

            # Reply target: channel if it's a channel, else the sender directly
            reply_to = target if target.startswith("#") else sender

            self.handle_command(sender, reply_to, message.strip())

    def handle_command(self, sender, reply_to, message):
        """Basic command handling — extend this with your own commands."""
        if message == "!ping":
            self.send_msg(reply_to, "pong!")
        elif message == "!hello":
            self.send_msg(reply_to, f"Hello, {sender}!")
        elif message == "!time":
            self.send_msg(reply_to, time.strftime("%Y-%m-%d %H:%M:%S"))
        elif message.startswith("!echo "):
            self.send_msg(reply_to, message[len("!echo "):])

    def run(self):
        self.connect()
        while True:
            try:
                for line in self.read_lines():
                    self.handle_line(line)
            except ConnectionError as e:
                print(f"Disconnected: {e}")
                break


if __name__ == "__main__":
    if len(sys.argv) != 6:
        print(f"Usage: python3 {sys.argv[0]} <host> <port> <password> <nickname> <channel>")
        sys.exit(1)

    bot = IRCBot(*sys.argv[1:6])
    bot.run()
