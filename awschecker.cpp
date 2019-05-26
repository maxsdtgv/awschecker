////////////////////////////////////////////////////////
// AWS checker Ver 1
// 
//
// Run:
//       ./awschecker <host> <port> <interval_seconds> <out_file>
// 
//
/////////////////////////////////////////////////////////

#include <unistd.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <chrono>
#include <ctime> 

static void dns_to_ip(char* dns_in, char* ip_out){
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
    if (argc < 5) {
        printf("Usage: %s <host> <port> <interval_seconds> <out_file>\n", argv[0]);
        exit(-1);
    }
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    time_t start_time;
    //time_t end_time;
    std::chrono::duration<double> duration_seconds;

    start = std::chrono::system_clock::now();
    start_time = std::chrono::system_clock::to_time_t(start);

    std::ofstream out_file;
    out_file.open(argv[4]);
    out_file << "Started echo ping to " << argv[1] << " at " <<  std::ctime(&start_time) << std::endl;



  	char ip_out[INET_ADDRSTRLEN] = {0};
  	dns_to_ip(argv[1], ip_out);

	struct sockaddr_in serv_addr;
	char const *request = "HelloMy \n";
	char reply[16] = {0};

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
    int i = 0;
    while(1){
	    i++;
	    
	    start = std::chrono::system_clock::now();
	    start_time = std::chrono::system_clock::to_time_t(start);

	    send(sock, request, strlen(request), 0); 
	    out_file << "seq = " << i <<", Request message sent, size = " << strlen(request) << " at " <<  ctime(&start_time);
	    out_file.flush();
	    printf("seq = %i, Request message sent, size = %zu , at %s ", i, strlen(request), ctime(&start_time)); 

	    read(sock, reply, strlen(request)); 
	    end = std::chrono::system_clock::now();
	    duration_seconds = end-start;
	    out_file << "seq = " << i <<", Reply received, duration time: " << duration_seconds.count() << "s\n\n";
	    out_file.flush();
	    printf("seq = %i, Reply received, duration time: %lf \n\n", i, duration_seconds.count());

	    sleep(atoi(argv[3]));

    }       

    out_file.close();
    return 0;
}