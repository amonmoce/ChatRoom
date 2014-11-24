# Overview

  We write two programs, a client and a server to
  build a chat room system. Clients talk to one another via connecting to the server.
  The server accepts connections from clients and sends commands to clients.

  Note: We use select() for constructing the client and server programs.


# Specification

## Server

  A server can serve multiple clients simultaneously. After a connection
  is built, server will send a hello message to the client, and give the client
  a username 'anonymous'. Clients can send different commands to server.

  Messages which is transmitted between clients and the server are as following:


### Hello Message
  When a client connects to server, the server sends hello message to the client,
  and broadcast this user's coming to other clients.

  Send Format:
    - To the new connected client
    [Server] Hello, anonymous! From: <Client IP>/<Client Port>

    - To existing client
    [Server] Someone is coming!

### Offline Message
  When a client disconnect to server, the server sends offline message to all the other online clients
  to tell them someone has been offline.

  Send Format:
    [Server] <USERNAME> is offline.

### Who Message *
  A client can type command below to list all online users.

  Recv Format:
    who

  Server will reply to sender a list of online users and tag the sender client.
  For N user, Server will send N lines.
  Each of them shows details of a user.

  Send Format:
    [Server] <USERNAME> <CLIENT IP>/<CLIENT PORT>
    [Server] <SENDER USERNAME> <CLIENT IP>/<CLIENT PORT> ->me

### Change Username Message

  A client can type command below to change his/her username.

  Recv Format
    name <NEW USERNAME>

  Server has to verify if the new name is valid which means the input name is
  (1) not anonymous, (2) unique, and (3) 2~12 English letters.
  It will reject user's request if this name cannot fit the rule.

  Send Format:
    - If the new name is anonymous.
    [Server] ERROR: Username cannot be anonymous.

    - If the new name is not unique.
    [Server] ERROR: <NEW USERNAME> has been used by others.

    - If the new name does not consist of 2~12 English letters.
    [Server] ERROR: Username can only consists of 2~12 English letters.

  Server will reply some messages to all users once a user changes his/her name.

  Send Format:
    - To user which changed his/her name
    [Server] You're now known as <NEW USERNAME>.

    - To other users
    [Server] <OLD USERNAME> is now known as <NEW USERNAME>.

  Note:
    A user can be rename as itself, that is, when "userA" wants to rename as "userA",
    server should not return error message.

--------------------------------------------------------------------------------
* Private Message *
  A client can send a private message to a specific client.

  Recv Format:
    tell <USERNAME> <MESSAGE>

  Server will send an error message back to the sender if either the sender's name or the receiver's name is anonymous.

  Send Format:
    - If the sender's name is anonymous
    [Server] ERROR: You are anonymous.

    - If the receiver's name is anonymous
    [Server] ERROR: The client to which you sent is anonymous.

    - If the receiver doesn't exist
    [Server] ERROR: The receiver doesn't exist.

  Otherwise, the server sends the private message to the specific client and sends back a notification to the sender.

  Send Format:
    To sender:
    -If message is sent
    [Server] SUCCESS: Your message has been sent.

    To receiver:
    - If both client's name are not anonymous
    [Server] <SENDER USERNAME> tell you <MESSAGE>

### Broadcast Message
   A client can send a broadcast message to all clients.

  Recv Format:
    yell <MESSAGE>

  While receiving the command from a user, server adds
  "<SENDER USERNAME>" at the head of it and broadcasts to all users including the sender.

  Send Format:
    [Server] <SENDER USERNAME> yell <MESSAGE>

### Error Command
  Commands which haven't been declared above are error commands.
  When a server receives an error command, it should send an error message back to the sending client.

  Send Format:
    [Server] ERROR: Error command.


## Client

  A client cannot connect to more than one server at the same time.
  Users should give server IP and Port as the arguments of client program.
  If the number of arguments is not two, the client program should terminate.

  ````
   ./client <SERVER IP> <SERVER PORT>
 ```

### Exit Command
  User can type command below to terminate the process at any time.
  Command Format:
    exit

  Note: This command should be process by the client locally.
        That is, the client should close the connection and terminate
        the process while it receives the exit command

### Receive & Display Format
  Client keeps receiving messages from stdin and sends messages to the server
  directly. While receiving a message from the server, the client should send to the server without modification

  Note:
     For messages received from stdin, client can only process the exit command, others
    should be sent to the server without modification. All messages received from stdin
    (except exit command) should be sent to server directly.
