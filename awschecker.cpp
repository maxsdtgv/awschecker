// AWS checker 

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <netdb.h>

uint dns_to_ip(char *dns_in);

uint dns_to_ip(char *dns_in){
    struct hostent *hp = gethostbyname(dns_in);
    char aaa111[INET_ADDRSTRLEN];
    if (hp == NULL) {
       printf("gethostbyname() failed\n");
    } else {
       printf("%s = ", hp->h_name);

       inet_ntop(AF_INET, hp -> h_addr_list[0], aaa111, INET_ADDRSTRLEN);            

       printf( "%s \n", aaa111);
    }	
	printf("%s >>>> !!!\n",aaa111);
	return *aaa111;
}

int main(int argc, char **argv)
{
  	

    if (argc < 2) {
        printf("Usage: %s hostname", argv[0]);
        exit(-1);
    }

    //char aaaddd[INET_ADDRSTRLEN];
    //aaaddd = *dns_to_ip(argv[1]);
	//printf("%s --- \n", aaaddd);
  	dns_to_ip(argv[1]);
  	//printf("%s >>>", aaaddd);


	struct sockaddr_in serv_addr;

	char *hello = "HelloMy";
	char buffer[1024] = {0};

	int sock = 0;

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "1.1.1.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 


  return 0;
}