// Server Application

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>

void SendFileToClient(int ClientSocket, char * Filename)
{
    int fd = 0;
    struct stat sobj;
    char Buffer[1024];
    int BytesRead = 0;
    char Header[64] = {'\0'};

    printf("File name is : %s : %lu\n",Filename,strlen(Filename));

    fd = open(Filename, O_RDONLY);

    // Unable to open file
    if(fd < 0)
    {
        printf("Unable to open file\n");
        // Send error message to client
        write(ClientSocket, "ERR\n",4);

        return;
    }

    stat(Filename,&sobj);

    // Header : "OK 1700"
    snprintf(Header,sizeof(Header),"OK %ld\n",(long)sobj.st_size);

    // Write header to client
    write(ClientSocket,Header,strlen(Header));
    
    // Send actual file contents
    while((BytesRead = read(fd, Buffer, sizeof(Buffer))) > 0)
    {
        // Send the data to client
        write(ClientSocket,Buffer,BytesRead);
    }

    close(fd);
}

/////////////////////////////////////////////////
//
//  Commandline Argument Application
//  1st Argument : Port Number 
//  ./server    9000
//  argv[0]     argv[1]
/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    int ServerSocket = 0;
    int ClientSocket = 0;
    int Port = 0;
    int iRet = 0;

    char FileName[50] = {'\0'};

    pid_t pid = 0;

    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
     
    socklen_t AddrLen = sizeof(ClientAddr);

    if((argc < 2) || (argc > 2))
    {
        printf("Unabel to processed as invalid number of arguments\n");
        printf("Please provide the port number\n");
        
        return -1;
    }

    // Port number of server
    Port = atoi(argv[1]);

    //////////////////////////////////////////////////
    //  Step 1 : Create TCP socket
    //////////////////////////////////////////////////

    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(ServerSocket < 0)
    {
        printf("Unable to create server socket\n");
        return -1;
    }

    //////////////////////////////////////////////////
    //  Step 2 : Bind socket to IP and Port
    //////////////////////////////////////////////////
    
    memset(&ServerAddr, 0,sizeof(ServerAddr));

    // Initialise the structure
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(Port);
    ServerAddr.sin_addr.s_addr = INADDR_ANY;

    iRet = bind(ServerSocket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));

    if(iRet == -1)
    {
        printf("Unable to bind\n");

        close(ServerSocket);

        return -1;
    }

    //////////////////////////////////////////////////
    //  Step 3 : Listen for client connections
    //////////////////////////////////////////////////

    iRet = listen(ServerSocket,11);

    if(iRet == -1)
    {
        printf("Server unable to listen the request\n");

        close(ServerSocket);

        return -1;
    }

    printf("Server is running on port : %d\n",Port);
    
    //////////////////////////////////////////////////
    //  Loop which accepts client requests continiously
    //////////////////////////////////////////////////

    // Loop to accept multiple client requests
    while(1)
    {
        //////////////////////////////////////////////////
        //  Step 4 : Accept the client request
        ////////////////////////////////////////////////// 
        
        memset(&ClientAddr, 0,sizeof(ClientAddr));

        printf("Server is waiting for client request\n");
        
        ClientSocket = accept(ServerSocket, (struct sockaddr *)&ClientAddr, &AddrLen);

        if(ClientSocket < 0)
        {
            printf("Unable to accept client request\n");

            continue;   // Used for while
        }

        printf("Client gets connected : %s\n",inet_ntoa(ClientAddr.sin_addr));

        //////////////////////////////////////////////////
        //  Step 5 : Create new process to handle client request
        //////////////////////////////////////////////////

        pid = fork();

        if(pid < 0)
        {
            printf("Unabel to create a new process for client request\n");

            close(ClientSocket);

            continue;
        }

        // New prcoecss gets created for client
        if(pid == 0)
        {
            printf("New process is cearetd for client request\n");

            close(ServerSocket);
            
            iRet = read(ClientSocket, FileName,sizeof(FileName));

            printf("Requested file by client : %s\n",FileName);            
            
            FileName[strcspn(FileName, "\r\n")] = '\0';

            SendFileToClient(ClientSocket, FileName);

            close(ClientSocket);

            printf("File transfer done & client disconnected\n");           
        
            exit(0);    // Kill the child process
        }   // End of if (fork)
        else    // Parent process (Server)
        {
            close(ClientSocket);
        } // End of else
    }   // End of while

    close(ServerSocket);

    return 0;
}   // End of main