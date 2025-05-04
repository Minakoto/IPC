#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>

#define BUFFER_SIZE 65536

#define CHECK(item, msg, text, wrong) ({ if( item wrong) {\
    msg(text);\
    exit(EXIT_FAILURE);\
    } })

void endit(int);
void catch_packets(char*, int);
void process_packets(char*, int, char*, int);
void unpack_packet(char*, int);
void show_packet(char*, uint16_t, char*, char*, uint16_t, uint16_t);
int right_dest(char*, char*, int, uint16_t);

int raw_socket, filefd;
sig_atomic_t end = 0;

int main(int argc, char* argv[]) {
    signal(SIGINT, endit);
    if(argc != 3) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    char* ip = argv[1];
    int port = atoi(argv[2]);

    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    CHECK(raw_socket, perror, "socket", < 0);

    filefd = open("packets.txt", O_CREAT|O_WRONLY, 0777);
    CHECK(filefd, perror, "file", < 0);
    printf("Sniffing...\n");
    while(!end) catch_packets(ip, port);

    endit(SIGINT);
}
void catch_packets(char* ip, int port) {
    char buffer[BUFFER_SIZE];
    int bytes_read = recvfrom(raw_socket, buffer, sizeof(buffer), 0, NULL, NULL);
    if(bytes_read < 0) {
        perror("recvfrom");
        end = 1;
        return;
    }
    process_packets(ip, port, buffer, bytes_read);
}

void process_packets(char* ip, int port, char* buffer, int size) {
    struct iphdr* ip_header = (struct iphdr*) buffer;
    struct udphdr* udp_header = (struct udphdr*)(buffer + (ip_header->ihl*4));
    char dest_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip_header->daddr), dest_ip, INET6_ADDRSTRLEN);
    if(!right_dest(dest_ip, ip, port, udp_header->dest)) return;
    else unpack_packet(buffer, size);
}

void unpack_packet(char* buffer, int size) {
    struct iphdr* ip_header = (struct iphdr*) buffer;
    unsigned int ip_headerlen = ip_header->ihl * 4;
    char src[INET_ADDRSTRLEN], dest[INET_ADDRSTRLEN];

    if(ip_header->protocol != IPPROTO_UDP) return;
    inet_ntop(AF_INET, &(ip_header->saddr), src, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_header->daddr), dest, INET_ADDRSTRLEN);

    struct udphdr* udp_header = (struct udphdr*) (buffer + ip_headerlen);
    uint16_t src_port = ntohs(udp_header->source);
    uint16_t dest_port = ntohs(udp_header->dest);
    uint16_t payload_len = ntohs(udp_header->len) - sizeof(struct udphdr);
    char* payload = buffer + ip_headerlen + sizeof(struct udphdr);
    show_packet(payload, payload_len, src, dest, src_port, dest_port);
}

void show_packet(char* payload, uint16_t payload_len, char* src, char* dest, uint16_t src_port, uint16_t dest_port) {
    char msg[1024];
    char* pload = malloc(sizeof(char) * payload_len);
    strncpy(pload, payload, payload_len);
    int cur_pos = sprintf(msg, "Packet from %s:%hd to %s:%hd -> %s\nlength = %hd\n", src, src_port, dest, dest_port, pload, payload_len);
    printf("%s", msg);
    write(filefd, msg, strlen(msg));
    free(pload);
}

int right_dest(char* dest_ip, char* ip, int port, uint16_t udp_dest) {
    return strcmp(dest_ip, ip) == 0 && ntohs(udp_dest) == port;
}

void endit(int signal) {
    printf("Closing\n");
    close(filefd);
    close(raw_socket);
    end = 1;
}