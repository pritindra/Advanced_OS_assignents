
#include<stdio.h>
#include<unistd.h>
#include<rpc/rpc.h>
#include "../server/who.h"

//long bin_date(void);
//char *str_date(long bintime);

int main(int argc, char **argv) {

    char **sresult;
    char *server = "localhost";

    extern char *optarg;
    extern int optind;
    int err = 0;
    int c;
    while((c = getopt(argc, argv, "h:")) != -1) {
        switch (c) {
        case 'h':
            server = optarg;
            break;
        case '?':
            err = 1;
            break;
        }
    }
    if (err || (optind < argc)) {
        fprintf(stderr, "usage: %s [-h hostname]\n", argv[0]);
        exit(1);
    }

    CLIENT *cl;
    cl = clnt_create(server, WHO_PROG, WHO_VERS, "tcp");


    if((sresult = str_who_1(NULL, cl)) == NULL){
        clnt_perror(cl,server);
    }
    printf("user is %s", *sresult);
    exit(0);
}
