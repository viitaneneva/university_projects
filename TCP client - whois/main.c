
#include<stdio.h>       // Default output module
#include<string.h>      // Function: strlen
#include<sys/socket.h>  // Function: socket
#include<netinet/in.h>  // Struct: sockaddr_in
#include<arpa/inet.h>   // Functions: inet_ntoa, inet_addr
#include<netdb.h>       // Struct: hostent
#include<unistd.h>      // Function: close

int main(void)
{

    printf("COMP.CS.410: TCP CLIENT FOR QUERYING TRAFICOM AT FICORA.FI\n");

    // Initialize buffers for messages
    char client_message[100] , server_reply[1500];

    // Create a client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_socket < 0){
        printf("The program was unable to create a socket!\n");
        return -1;
    }

    // Create structs and fill them with required parameters
    struct hostent *hostname_ficora;
    hostname_ficora = gethostbyname("ficora.fi");
    char* query_ip = inet_ntoa(*((struct in_addr*) hostname_ficora->h_addr_list[0]));

    struct hostent *hostname_whois;
    hostname_whois = gethostbyname("whois.fi"); // .fi ending is required when querying for a finnish website!
    char* ip = inet_ntoa(*((struct in_addr*) hostname_whois->h_addr_list[0]));

    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr( ip );
    server_info.sin_port = htons( 43 ); // port number, whois service runs on port 43

    printf("\nConnecting to the server..");

    // Connecting to whois server for query
    if(connect( client_socket, (const struct sockaddr*) &server_info, sizeof(server_info) ) < 0)
    {
        printf("\nConnection to the server failed.");
    }
    else{
        printf("\nConnection to the server succeeded.");
    }

    printf("\nIP: %s resolved.", query_ip);
    printf("\n--------------------------------------\n");

    // Construct the client message
    sprintf(client_message , "%s\r\n" , "ficora.fi");

    // Send a query message to the server
    if( send(client_socket, client_message, strlen(client_message), 0) < 0)
    {
        printf("\nSending the client message failed!");
    }

    // Saving the received reply to the corresponding buffer
    recv(client_socket, server_reply, sizeof(server_reply), 0);

    // Print out the message
    printf("%s", server_reply);

    // Close the socket after receiving the reply
    close(client_socket);

    return 0;
}
