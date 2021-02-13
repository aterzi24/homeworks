#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <sys/wait.h>
#include "logging.h"

#define SIZE 100
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

using namespace std;

class Bid_type {
    public:
        string exe;
        char **arglist;
        int no_arg;
        int pid;
};

void func1(sm &message, const int &v1, const int &v2, const int &v3, const int &v4)
{
    message.params.start_info.client_id = v1;
    message.params.start_info.starting_bid = v2;
    message.params.start_info.current_bid = v3;
    message.params.start_info.minimum_increment = v4;
}

bool func2(sm &message, const int &bid, const int &start, int &current, const int &inc)
{
    message.params.result_info.current_bid = current;
    if(bid < start)
    {
        message.params.result_info.result = BID_LOWER_THAN_STARTING_BID;
        return false;
    }
    if(bid < current)
    {
        message.params.result_info.result = BID_LOWER_THAN_CURRENT;
        return false;
    }
    if(bid < current + inc)
    {
        message.params.result_info.result = BID_INCREMENT_LOWER_THAN_MINIMUM;
        return false;
    }
    message.params.result_info.result = BID_ACCEPTED;
    message.params.result_info.current_bid = bid;
    current = bid;
    return true;
}

int find_func(int wpid, Bid_type *bidders, int n)
{
    int i;
    for( i = 0 ; i < n ; i++ )
        if( bidders[i].pid == wpid )
            return i;
}

int main() {

    int i,j;
    int start_bid, min_inc, no_bidders, active_bidders, cpid, curr_bid, max_bidder;

    cin>>start_bid>>min_inc>>no_bidders;

    Bid_type *bidders = new Bid_type [no_bidders];
    int (*fd)[2] = new int [no_bidders][2];
    struct pollfd *fds = new struct pollfd [no_bidders];
    int *statuses = new int [no_bidders];

    for( i = 0 ; i < no_bidders ; i++ )
    {
        cin>>bidders[i].exe>>bidders[i].no_arg;
        bidders[i].arglist = new char* [bidders[i].no_arg+2];

        bidders[i].arglist[0] = new char [SIZE];
        strcpy(bidders[i].arglist[0], bidders[i].exe.c_str());
        for( j = 1 ; j < bidders[i].no_arg+1 ; j++ )
        {
            bidders[i].arglist[j] = new char [SIZE];
            cin>>bidders[i].arglist[j];
        }
        bidders[i].arglist[j] = (char *)0;
    }

    for( i = 0 ; i < no_bidders ; i++ )
    {
        PIPE(fd[i]);
        cpid = fork();
        if( cpid )
        {
            close(fd[i][1]);
            fds[i].fd = fd[i][0];
            fds[i].events = POLLIN;
            fds[i].revents = 0;
            bidders[i].pid = cpid;
        }
        else
        {
            close(fd[i][0]);
            dup2(fd[i][1], 0);
            dup2(fd[i][1], 1);
            close(fd[i][1]);
            execv(bidders[i].arglist[0], bidders[i].arglist);
        }
    }

    active_bidders = no_bidders;
    curr_bid = 0;
    while(active_bidders)
    {
        poll(fds, no_bidders, -1);

        for( i = 0 ; i < no_bidders ; i++ )
        {
            if( (fds[i].revents & POLLIN) != POLLIN )
                continue;

            cm rec_mes;
            read(fds[i].fd, (void *)&rec_mes, sizeof(cm));

            ii i_data;
            i_data.pid = bidders[i].pid;
            i_data.info = rec_mes.params;

            if(rec_mes.message_id == CLIENT_CONNECT)
            {
                i_data.type = CLIENT_CONNECT;
                print_input(&i_data, i);

                sm mes_to_send;
                mes_to_send.message_id = SERVER_CONNECTION_ESTABLISHED;
                func1(mes_to_send, i, start_bid, curr_bid, min_inc);

                write(fds[i].fd, (void *)&mes_to_send, sizeof(sm));

                oi o_data;
                o_data.type = SERVER_CONNECTION_ESTABLISHED;
                o_data.pid = bidders[i].pid;
                o_data.info = mes_to_send.params;
                print_output(&o_data, i);
            }
            else if(rec_mes.message_id == CLIENT_BID)
            {
                i_data.type = CLIENT_BID;
                print_input(&i_data, i);

                sm mes_to_send;
                mes_to_send.message_id = SERVER_BID_RESULT;
                if( func2(mes_to_send, rec_mes.params.bid, start_bid, curr_bid, min_inc) )
                    max_bidder = i;

                write(fds[i].fd, (void *)&mes_to_send, sizeof(sm));

                oi o_data;
                o_data.type = SERVER_BID_RESULT;
                o_data.pid = bidders[i].pid;
                o_data.info = mes_to_send.params;
                print_output(&o_data, i);
            }
            else if(rec_mes.message_id == CLIENT_FINISHED)
            {
                i_data.type = CLIENT_FINISHED;
                print_input(&i_data, i);

                statuses[i] = rec_mes.params.status;
                active_bidders--;
                fds[i].fd = -1;
            }
        }
    }

    print_server_finished(max_bidder, curr_bid);
    sm mes_to_send;
    mes_to_send.message_id = SERVER_AUCTION_FINISHED;
    mes_to_send.params.winner_info.winner_id = max_bidder;
    mes_to_send.params.winner_info.winning_bid = curr_bid;

    for( i = 0 ; i < no_bidders ; i++ )
    {
        write(fd[i][0], (void *)&mes_to_send, sizeof(sm));

        oi o_data;
        o_data.type = SERVER_AUCTION_FINISHED;
        o_data.pid = bidders[i].pid;
        o_data.info = mes_to_send.params;
        print_output(&o_data, i);
    }
    for( i = 0 ; i < no_bidders ; i++ )
    {
        int child_status, wpid, client_id;
        wpid = wait(&child_status);
        client_id = find_func(wpid, bidders, no_bidders);
        print_client_finished(client_id, statuses[client_id], statuses[client_id] == child_status);
        close(fd[i][0]);
    }
    delete [] fd;
    delete [] fds;
    delete [] statuses;
    for( i = 0 ; i < no_bidders ; i++ )
    {
        for( j = 0 ; j < bidders[i].no_arg+1 ; j++ )
            delete [] bidders[i].arglist[j];
        delete [] bidders[i].arglist;
    }
    delete [] bidders;

    return 0;
}
