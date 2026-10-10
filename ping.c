#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>


#pragma comment(lib, "Ws2_32.lib")


    WSADATA wsaData;
    int iResult;


int main(int argc, char *argv[]){

    if (argc < 2)
    {
        printf("No socket address provided, shutting down");
        return 1;
    }
    

    // initalise Ws2 DLL and winsock2 lib, check for errors

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult !=0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
} else {
    printf("WSAStartup Successful\n");
}

    // create 3 addrinfo strcuts - or 2 pointers and 1 full, these hold adress infomation for connection
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family     = AF_UNSPEC; // defines if connection accepts IPv4 or IPv6
    hints.ai_socktype   = SOCK_DGRAM; // Telling it to use UDP socket
    hints.ai_protocol   = IPPROTO_UDP; // Telling it to use UDP protocol

    printf("Adress Objects Created\n");

    #define DEFAULT_PORT "27015"

    // request address info from user using default port assgined above

    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult !=0){
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    printf("Generating Socket...\n");
    // generate a Socket
    SOCKET ConnectSocket = INVALID_SOCKET;

    ptr = result;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);


    // checking socket is valid
    if (ConnectSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
    }
    

    //clean up in required order ofc - my comments getting more causal the longer I go lol
    freeaddrinfo(result);
    closesocket(ConnectSocket);
    WSACleanup();

    printf("Process fully complete\n");
    return 0;
}