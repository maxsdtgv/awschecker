// AWS checker 

#include <unistd.h> 
#include <stdio.h> 
#include <iostream>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <chrono>
#include <ctime> 

void dns_to_ip(char* dns_in, char* ip_out);

void dns_to_ip(char* dns_in, char* ip_out){
    struct hostent *hp = gethostbyname(dns_in);
    if (hp == NULL) {
       printf("gethostbyname() failed\n");
    } else {
       printf("%s = ", hp->h_name);

       inet_ntop(AF_INET, hp -> h_addr_list[0], ip_out, INET_ADDRSTRLEN);            

       printf( "%s \n", ip_out);
    }	
	return;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s hostname port timeout out_file", argv[0]);
        exit(-1);
    }


  	char ip_out[INET_ADDRSTRLEN] = {0};
  	dns_to_ip(argv[1], ip_out);

printf("%s >>>> !!!22\n", ip_out);

	struct sockaddr_in serv_addr;

	char *request = "HelloMy \n";
	char reply[1024] = {0};

	int sock = 0;

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    // Convert IP from text to binary form 
    if(inet_pton(AF_INET, ip_out, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\n Connection Failed \n"); 
        return -1; 
    } 

    auto start = std::chrono::system_clock::now();

    send(sock, request, strlen(request), 0); 
    printf("Hello message sent\n"); 

    read(sock, reply, strlen(request)); 
    printf("%s\n", reply); 

    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
                



  return 0;
}