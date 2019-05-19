////////////////////////////////////////////////////////
// AWS checker Ver 1
// 
//
// Run:
//       ./awschecker <host> <port> <interval> <out file>
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
using namespace std::chrono;
using namespace std;

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
    if (argc < 5) {
        printf("Usage: %s hostname port timeout out_file", argv[0]);
        exit(-1);
    }
    system_clock::time_point start;
    system_clock::time_point end;
    time_t start_time;
    time_t end_time;
    duration<double> duration_seconds;

    start = system_clock::now();
    start_time = system_clock::to_time_t(start);

    ofstream out_file;
    out_file.open(argv[4]);
    out_file << "Started echo ping to " << argv[1] << " at " <<  ctime(&start_time) << endl;



  	char ip_out[INET_ADDRSTRLEN] = {0};
  	dns_to_ip(argv[1], ip_out);

	struct sockaddr_in serv_addr;
	char *request = "HelloMy \n";
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

    while(1){

 	

    start = system_clock::now();
    start_time = system_clock::to_time_t(start);

    send(sock, request, strlen(request), 0); 
    out_file << "Request message sent, size = " << strlen(request) << " at " <<  ctime(&start_time);
    out_file.flush();
    printf("Request message sent\n"); 

    read(sock, reply, strlen(request)); 
    end = system_clock::now();
    duration_seconds = end-start;
    out_file << "Reply received, duration time: " << duration_seconds.count() << "s\n\n";
    out_file.flush();
    sleep(atoi(argv[3]));

    }       

    out_file.close();
    return 0;

}