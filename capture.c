#include <sys/time.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <pcap/pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#define PROMISCUOUS 1
#define NONPROMISCUOUS 0

struct ip* iph;
struct tcphdr* tcph;
struct icmphdr* icmph;
FILE* pac_file;
int counter;
pcap_t* pcd2;
void terminate_process(int signum);
void callback(u_char* useless, const struct pcap_pkthdr* pkthdr,
    const u_char* packet)
{
    static int count = 1;
    struct ether_header* ep;
    unsigned short ether_type;
    int chcnt = 0;
    int length = pkthdr->len;
    char fname[10];
    char* payload;
    int i, payload_len;
    int sockfd, retval, n;

    ep = (struct ether_header*)packet;


    packet += sizeof(struct ether_header);


    ether_type = ntohs(ep->ether_type);


    if (ether_type == ETHERTYPE_IP)
    {

        struct ip* iph = (struct ip*)packet;
        struct ip* tph = (struct ip*)packet;
        //ip header
        struct tcphdr* tcph;
        struct udphdr* udph;
        count = (count * counter);
        counter = 1;
        count++;
        sprintf(fname, "%d", count);
        pac_file = fopen(fname, "w");
        if (pac_file == NULL) {
            printf("ERROR\n");
            exit(1);
        }
        fprintf(pac_file, "capture number: %d\n", count);
        printf("capture number : %d\n", count);
        fprintf(pac_file, "IP packet\n");
        printf("IP packet\n");
        printf("Version     : %d\n", tph->ip_v);
        printf("Header Len  : %d\n", tph->ip_hl);
        printf("Ident       : %d\n", ntohs(tph->ip_id));
        printf("TTL         : %d\n", tph->ip_ttl);
        printf("Src Address : %s\n", inet_ntoa(tph->ip_src));
        printf("Dst Address : %s\n", inet_ntoa(tph->ip_dst));


        if (tph->ip_p == IPPROTO_TCP)
        {
            tcph = (struct tcphdr*)(packet + tph->ip_hl * 4);
            fprintf(pac_file, "Src Port : %d\n", ntohs(tcph->source));
            printf("Src Port : %d\n", ntohs(tcph->source));
            fprintf(pac_file, "Dst Port : %d\n", ntohs(tcph->dest));
            printf("Dst Port : %d\n", ntohs(tcph->dest));
        }
        if (iph->ip_p == IPPROTO_UDP)
        {
            udph = (struct udphdr*)(packet + iph->ip_hl * 4);
            fprintf(pac_file, "Src Port : %d\n", ntohs(udph->source));
            printf("Src Port : %d\n", ntohs(udph->source));
            fprintf(pac_file, "Dst Port : %d\n", ntohs(udph->dest));
            printf("Dst Port : %d\n", ntohs(udph->dest));
            fprintf(pac_file, "UDP Length : %d\n", ntohs(udph->len));
            printf("UDP Length : %d\n", ntohs(udph->len));
            fprintf(pac_file, "UDP Checksum : %d\n", ntohs(udph->check));
            printf("UDP Checksum : %d\n", ntohs(udph->check));
            printf("\n---------------------------");
            fprintf(pac_file, "\n");
            payload = (u_char *)(packet + (iph->ip_hl * 4));
            payload_len = ntohs(iph->ip_len) - ((iph->ip_hl * 4));
            for (i = 0; i < payload_len; i++){
                if(payload[i] >= 32 && payload[i] <= 128){
                    fprintf(pac_file, "%c", payload[i]);
                } else {
                    fprintf(pac_file, "*");
                }
                
                if(i % 16 == 0){
                    fprintf(pac_file, "\n");
                }
            }
        }
        fprintf(pac_file, "\n");

        
        while (length--)
        {
            fprintf(pac_file, "%02x ", *(packet++));
            if ((++chcnt % 16) == 0)
                fprintf(pac_file, "\n");
        }
        fprintf(pac_file, "\n");
        fclose(pac_file);
    }

    else
    {
        printf("NONE IP packet\n");
    }
    printf("\n\n");
}

int main()
{
    char* dev;
    char* net;
    char* mask;

    pcap_if_t* alldevs;
    pcap_if_t* d;
    bpf_u_int32 netp;
    bpf_u_int32 maskp;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret, i = 0, inum = 0;
    struct pcap_pkthdr hdr;
    struct in_addr net_addr, mask_addr;
    struct ether_header* eptr;
    const u_char* packet;

    struct bpf_program fp;

    int sockfd, retval, n;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char buf[10000];
    int q;
    clilen = sizeof(struct sockaddr_in);
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    pcap_t* pcd;
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == -1)
    {
        printf("%s\n", errbuf);
        exit(1);
    }

    for (d = alldevs; d; d = d->next)
    {
        printf("%d: %s: ", ++i, d->name);
        if (d->description)
            printf("%d description: %s\n", i, d->description);
        else
            printf("No description available\n");
    }

    printf("Enter interface number (1-%d): ", i);
    scanf("%d", &inum);

    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
    printf("DEV : %s\n", d->name);


    ret = pcap_lookupnet(d->name, &netp, &maskp, errbuf);
    if (ret == -1)
    {
        printf("%s\n", errbuf);
        exit(1);
    }

    // net and mask info
    net_addr.s_addr = netp;
    net = inet_ntoa(net_addr);
    printf("NET : %s\n", net);

    mask_addr.s_addr = maskp;
    mask = inet_ntoa(mask_addr);
    printf("MSK : %s\n", mask);
    printf("==========================\n");


    while (1) {
        int select = 0;
        char dname[10];
        int i;
        char str[100];
        printf("Select number\n");
        printf("1. TCP capture\n");
        printf("2. UDP capture\n");
        printf("3. ICMP\n");
        printf("4. Check packet detail\n");
        printf("5. exit\n");
        scanf("%d", &select);
        printf("\n");
        switch (select)
        {
        case 1:

            pcd = pcap_open_live(d->name, BUFSIZ, 1, -1, errbuf);
            if (pcd == NULL) {
                printf("%s\n", errbuf);
                exit(1);
            }

            if (pcap_compile(pcd, &fp, "port 80", 0, netp) == -1) {
                printf("compile error\n");
                exit(1);
            }

            if (pcap_setfilter(pcd, &fp) == -1) {
                printf("setfilter error\n");
                exit(0);
            }

            pcd2 = pcd;
            signal(SIGINT, terminate_process);
            pcap_loop(pcd, -1, callback, NULL);
            counter = 0;
            printf("\n\n");
            break;

        case 2:

            pcd = pcap_open_live(d->name, BUFSIZ, 1, -1, errbuf);
            if (pcd == NULL) {
                printf("%s\n", errbuf);
                exit(1);
            }

            if (pcap_compile(pcd, &fp, "port 53", 0, netp) == -1) {
                printf("compile error\n");
                exit(1);
            }

            if (pcap_setfilter(pcd, &fp) == -1) {
                printf("setfilter error\n");
                exit(0);
            }

            pcd2 = pcd;
            signal(SIGINT, terminate_process);
            pcap_loop(pcd, -1, callback, NULL);
            counter = 0;
            printf("\n\n");
            break;
            
        case 3:

            while ("Seq 291") {
                n = recvfrom(sockfd, buf, 10000, 0, (struct sockaddr*) & cliaddr, &clilen);
                printf(" recv %d bytes\n", n);

                struct iphdr* ip_hdr = (struct iphdr*)buf;

                printf("IP header is %d bytes\n", ip_hdr->ihl * 4);

                for (q = 0; q < n; q++) {
                    printf("%02X%s", (uint8_t)buf[q], (q + 1) % 16 ? " " : "n");
                }
                printf("\n");

                struct icmphdr* icmp_hdr = (struct icmphdr*)((char*)ip_hdr + (4 * ip_hdr->ihl));

                printf("ICMP msgtype=%d, code=%d\n", icmp_hdr->type, icmp_hdr->code);
                printf("Checksum=%04x\n", ntohs(icmp_hdr->checksum));
                printf("Identifier=%d\n", icmp_hdr->un.echo.id);
                printf("Seq=%d\n", icmp_hdr->un.echo.sequence);
                printf("\n");
            }

        case 4:

            printf("Select capture number.\n");
            scanf("%d", &i);
            printf("\n");
            sprintf(dname, "%d", i);
            pac_file = fopen(dname, "r");
            if (pac_file == NULL){
                printf("ERROR\n");
                break;
            }
            while (feof(pac_file) == 0) {
                fgets(str, 100, pac_file);
                printf("%s", str);
            }
            printf("\n\n");
            fclose(pac_file);
            break;

        case 5:
            exit(0);
            break;
        default:
            printf("ERROR, Chose correct number\n");
            break;
        }
    }
    pcap_freealldevs(alldevs);
    return 0;
}

void terminate_process(int signum) {
    pcap_breakloop(pcd2);
    pcap_close(pcd2);
}