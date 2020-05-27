#pragma once
#include "common.hpp"
#include <sys/epoll.h>
/*
*IO多路复用程序
*linux下使用epoll 
对应于redis源码中的ae_epoll.h
*/

class ae_epoll
{
    int epfd;//file describtion of epoll
    epoll_event *ev;//arr to epoll_event
    int ev_size;

public:
    ae_epoll(int setsize);
    ~ae_epoll();
    int add_intersted_event(int fd, fileevent &fe, int mask);
    void del_intersted_event(int fd, fileevent &fe, int delmask);
    int monitor_registed_event(int setsize,pfired fired);
};

ae_epoll::ae_epoll(int setsize)
{
    ev = new epoll_event[setsize];
    ev_size = setsize;
    if(!ev)
    {
        delete ev;
        ev = nullptr;
    }

    epfd = epoll_create(1024);
    if(epfd == -1)
        PERROR("epoll create:")
}

ae_epoll::~ae_epoll()
{
    close(epfd);
    delete ev;
    ev = nullptr;
}

int
ae_epoll::add_intersted_event(int fd, fileevent &fe, int mask)
{
    epoll_event ee={0};
    int op = fe.getmask() == AE_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= fe.getmask();
    if(mask & AE_READABLE) ee.events |= EPOLLIN;
    if(mask & AE_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    if(epoll_ctl(epfd, op, fd, &ee) == -1) return -1;
    return 0;
}

void
ae_epoll::del_intersted_event(int fd, fileevent&fe, int delmask)
{
    epoll_event ee={0};
    int mask = fe.getmask() & (~delmask);

    ee.events = 0;
    if (mask & AE_READABLE) ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    if (mask != AE_NONE) {
        epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ee);
    } else {
        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ee);
    }
}

int
ae_epoll::monitor_registed_event(pfired fired)
{
    int retval;
    retval = epoll_wait(epfd, ev, ev_size, 1);
    if(retval > 0)
    {  
        for(int i = 0;i<retval;i++)
        {
            int mask = 0;
            epoll_event *e = ev+i;
            if (e->events & EPOLLIN) mask |= AE_READABLE;
            if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
            if (e->events & EPOLLERR) mask |= AE_WRITABLE|AE_READABLE;
            if (e->events & EPOLLHUP) mask |= AE_WRITABLE|AE_READABLE;
            fired[i].fd = e->data.fd;
            fired[i].mask = mask;
        }

    }
    return retval;
}



