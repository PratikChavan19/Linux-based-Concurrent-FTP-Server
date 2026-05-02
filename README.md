📌 Overview :

This project implements a Concurrent FTP Server in C (Linux environment) using TCP sockets.

Multiple clients handled simultaneously using fork()
File transfer via socket communication
Simple and efficient client-server architecture

-----------------------------------------------------------------------------------------------------------------------------------------------

🏗️ Architecture Diagram :

                +-------------------+
                |     Client 1      |
                +-------------------+
                          |
                          |
                +-------------------+
                |     Client 2      |
                +-------------------+
                          |
                          |
                +-------------------+
                |     Client N      |
                +-------------------+
                          |
                          v
                =====================
                |   TCP Socket      |
                =====================
                          |
                          v
                +-------------------+
                |     Server        |
                |  (Main Process)   |
                +-------------------+
                          |
              +-----------+-----------+
              |           |           |
              v           v           v
        +---------+ +---------+ +---------+
        | Child 1 | | Child 2 | | Child N |
        | fork()  | | fork()  | | fork()  |
        +---------+ +---------+ +---------+
              |           |           |
              v           v           v
        File Transfer  File Transfer  File Transfer

-----------------------------------------------------------------------------------------------------------------------------------------------

⚙️ Features :

📡 TCP-based communication
🔄 Concurrent client handling (fork())
📁 File transfer with size header
📥 Chunk-based file download (1024 bytes)
❌ Error handling for invalid files

-----------------------------------------------------------------------------------------------------------------------------------------------

🚀 How to Compile & Run :

🔧 Compile
gcc Server.c -o server
gcc Client.c -o client

▶️ Run Server
./server <PORT>
Example:
./server 9000

▶️ Run Client
./client <IP> <PORT> <SOURCE_FILE> <DEST_FILE>
Example:
./client 127.0.0.1 9000 Demo.txt Copy.txt

-----------------------------------------------------------------------------------------------------------------------------------------------

🔄 Workflow :

Client :
Create socket
Connect to server
Send filename
Read header (OK <size>)
Receive file

Server :
Create socket
Bind & listen
Accept connection
fork() new process
Send file

-----------------------------------------------------------------------------------------------------------------------------------------------

📚 Libraries Used :

Library	          Purpose
stdio.h	          Input/output
stdlib.h	        Utility functions
string.h	        String operations
unistd.h	        System calls
fcntl.h	          File operations
sys/socket.h	    Socket APIs
sys/stat.h	      File metadata
netinet/in.h	    Network structures
arpa/inet.h	      IP conversion
stdbool.h	        Boolean support

-----------------------------------------------------------------------------------------------------------------------------------------------

🧠 System Calls & Their Usage :

🔌 Socket APIs
socket()
  Creates communication endpoint
  Used in both client & server
connect()
  Client connects to server
bind()
  Assigns IP & port to server
listen()
  Enables server to accept connections
accept()
  Accepts incoming client request

 -----------------------------------------------------------------------------------------------------------------------------------------------

📂 File Operations :

open()
  Opens/creates files
read()
  Reads from file/socket
write()
  Writes to file/socket
close()
  Closes descriptor
stat()
  Gets file size (used in header)

-----------------------------------------------------------------------------------------------------------------------------------------------

⚡ Process Management :

fork()
  Creates child process
  Enables concurrency

-----------------------------------------------------------------------------------------------------------------------------------------------

🌐 Network Utilities :

inet_pton() → IP text → binary
inet_ntoa() → binary → text
htons() → host to network byte order

-----------------------------------------------------------------------------------------------------------------------------------------------

🔧 Custom Function :

ReadLine()
  Reads data from socket until \n
  Used to parse header

-----------------------------------------------------------------------------------------------------------------------------------------------

📊 Sample Output :

Server
  Server is running on port : 9000
  Client gets connected : 127.0.0.1
  Requested file by client : Demo.txt
  File transfer done & client disconnected
  
Client
  Header is : OK 1700
  File size is : 1700
  Download complete...

-----------------------------------------------------------------------------------------------------------------------------------------------

🔮 Future Enhancements :

🔐 Authentication system
🔒 SSL/TLS encryption
📤 File upload support
🧵 Multi-threading (instead of fork)
📂 Directory browsing

-----------------------------------------------------------------------------------------------------------------------------------------------

👨‍💻 Author :

Pratik Chavan

-----------------------------------------------------------------------------------------------------------------------------------------------
