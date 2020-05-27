#pragma once
#include"ae_epoll.hpp"
#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0       /* No events registered. */
#define AE_READABLE 1   /* Fire when descriptor is readable. */
#define AE_WRITABLE 2   /* Fire when descriptor is writable. */
#define AE_BARRIER 4    /* With WRITABLE, never 

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT 4
#define AE_CALL_AFTER_SLEEP 8

#define AE_NOMORE -1
#define AE_DELETED_EVENT_ID -1



using callback = std::function<void(int fd, void*data,int mask)>;

class fileevent
{
    int mask;
    void* clientdate;
    callback rcb;
    callback wcb;

public:
    void setmask(int mask){this->mask = mask;}
    int getmask(){return mask;}
    void setdate(void*date){clientdate = date;}
    void set_callback(char flag, callback cb)
    {
        if(flag == "r")
            rcb = cb;
        if(flag == "w")
            wcb = cb;
    }
    void read_file_proc(int fd)
    {
        if(rcb!=nullptr)
            rcb(fd, clientdate, mask);
        else
            PERROR("read_file_proc:")
    }
    void write_file_proc()
    {
        if(wcb!=nullptr)
            wcb(fd, clientdate,mask);
        else
            PERROR("write_file_proc")
    }
};

class firedevent
{
public:
    int fd;
    int mask;
};

class timeevent
{

};


class eventloop
{
    using pfile = fileevent *;
    using pfired = firedevent *;
    using ptime = timeevent *;


    int setsize;
    pfile events;
    pfired fired;
    ptime timeeventshead;
    ae_epoll poll;
    int stop;

public:
    eventloop(int setsize);
    ~eventloop();
    void aestop();
    int creat_file_event(int fd, int mask, callback proc, void*date);
    void delete_file_event(int fd, int mask);
    int get_file_events(int fd);
    int process_events(int flags);
    
    void loop();
}