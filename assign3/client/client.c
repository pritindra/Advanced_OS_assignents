#include<stdio.h>
#include<unistd.h>
#include<rpc/rpc.h>
#include "../server/date.h"

//long bin_date(void);
//char *str_date(long bintime);

int main(int argc, char **argv) {

    long *lresult;
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
    cl = clnt_create(server, DATE_PROG, DATE_VERS, "tcp");


    if((lresult = bin_date_1(NULL, cl)) == NULL) {
        clnt_perror(cl, server);
        exit(1);
    }

    printf("server time is %ld\n", *lresult);
    if((sresult = str_date_1(lresult, cl)) == NULL){
        clnt_perror(cl,server);
    }
    printf("server date is %s", *sresult);
    exit(0);
}
