#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

#define BUFFER_SIZE 65536

void process_packet(unsigned char* buffer, int size);
void print_ethernet_header(unsigned char* buffer, int size);
void print_ip_header(unsigned char* buffer, int size);

int main() {
    int sock_raw;
    struct sockaddr saddr;
    int saddr_len = sizeof(saddr);

    // Create raw socket using AF_PACKET to capture all Ethernet frames
    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        perror("Socket Error. Make sure you are using unshare or have capabilities.");
        return 1;
    }

    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Memory allocation failed");
        close(sock_raw);
        return 1;
    }

    printf("Starting packet capture (with MAC parsing)... Press Ctrl+C to stop.\n\n");

    while (1) {
        int data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t*)&saddr_len);
        if (data_size < 0) {
            perror("Recvfrom error");
            break;
        }

        process_packet(buffer, data_size);
    }

    free(buffer);
    close(sock_raw);
    return 0;
}

void process_packet(unsigned char* buffer, int size) {
    struct ethhdr *eth = (struct ethhdr *)buffer;

    // 1. Print Layer 2 Ethernet Details (MAC Addresses)
    print_ethernet_header(buffer, size);

    // 2. If it's an IP packet, print Layer 3 IP Details
    if (ntohs(eth->h_proto) == ETH_P_IP) {
        print_ip_header(buffer + sizeof(struct ethhdr), size - sizeof(struct ethhdr));
    }
    
    printf("\n----------------------------------------\n");
}

void print_ethernet_header(unsigned char* buffer, int size) {
    struct ethhdr *eth = (struct ethhdr *)buffer;

    printf("--- Ethernet Header ---\n");
    printf("  |- Destination MAC : %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("  |- Source MAC      : %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("  |- Protocol Type   : 0x%.4x\n", ntohs(eth->h_proto));
}

void print_ip_header(unsigned char* buffer, int size) {
    struct iphdr *iph = (struct iphdr *)buffer;
    
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    printf("--- IP Header ---\n");
    printf("  |- Version         : %d\n", (unsigned int)iph->version);
    printf("  |- Protocol        : %d ", (unsigned int)iph->protocol);
    
    if (iph->protocol == IPPROTO_TCP) {
        printf("(TCP)\n");
    } else if (iph->protocol == IPPROTO_UDP) {
        printf("(UDP)\n");
    } else if (iph->protocol == IPPROTO_ICMP) {
        printf("(ICMP)\n");
    } else {
        printf("(Other)\n");
    }

    printf("  |- Source IP       : %s\n", inet_ntoa(source.sin_addr));
    printf("  |- Dest IP         : %s\n", inet_ntoa(dest.sin_addr));
}
