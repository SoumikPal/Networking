# Multi-Client-Server-Chat
Group chat program where there is a single server and multiple clients. The server forwards a client's message to a particular client.

<b>Platform:</b> Linux with GCC compiler

<b>Compiling:</b> gcc server.c -pthread -o server
<b>Compiling:</b> gcc client.c -pthread -o client

<b>Running the server:</b> ./server 8000(or any other port number of your choice)

<b>Running the clients:</b> ./client usernumber 8000(same port as server)

<b>Stopping the clients and server:</b> CTRL-C

<b>Note:</b> The portnumber provided to clients should be same as the one given to server. Usernumber is just an identity of yourself.

<b>Sending messages :</b> If you want to send to a particular client then send the message in the following format.
 usernumber.<Your Message> without the <>
