#include "ae.hpp"
#include "ae_epoll.hpp"

eventloop::eventloop(int setsize):poll(setsize)
{
    this->setsize = setsize;
    events = new fileevent[setsize];
    fired = new firedevent[setsize];
    if(events == nullptr || fired == nullptr) return nullptr;
    timeeventshead = nullptr;
    stop = 0;
    for(int i =0;i<setsize;i++)
        events[i].setmask(AE_NONE);
}

eventloop::~eventloop()
{
    delete events[];
    events = nullptr;
    delete fired[];
    fired = nullptr;
}

void
eventloop::aestop()
{
    stop = 1;
}

int
eventloop::creat_file_event(int fd, int mask,callback proc, void*date);
{
    if(fd >= setsize)
    {
        errno = ERANGE;
        return AE_ERR;
    }
    pfile fe = events[fd];
    if(poll.add_intersted_event(fd,*fe,mask) == -1)
        return AE_ERR;

    fe->setmask(fe->getmask() |=mask);
    if (mask & AE_READABLE) fe->set_callback("r",proc);
    if (mask & AE_WRITABLE) fe->set_callbakc("w",proc);
    fe->setdate(date);
    return AE_OK;

}

void
eventloop::delete_file_event(int fd, int mask)
{
    if(fd > setsize) return;
    pfile fe = events[fd];
    if(fe.getmask() == AE_NONE) return;
    if(mask & AE_WRITABLE) mask |= AE_BARRIER;

    poll.del_intersted_event(fd,*fe,mask);

}

int
eventloop::get_file_events(int fd)
{
    if(fd >= setsize) return 0;
    return events[fd].getmask();
}

int
eventloop::process_events(int flags)
{
    //将就绪事件添加到fired对象数组中
    int numevents = poll.monitor_registed_event(fired);
    int process = 0;

    for(int i = 0; i< numevents; i++)
    {
        pfile fe = events[fired[i].fd];
        int mask = fired[i].mask;
        int fd = fired[i].fd;
        int fires = 0;


        int revert = fe->getmask() & AE_BARRIER;
        if(!revert && fe->getmask() & mask & AE_READABLE)
        {
            fe->read_file_proc(fd);
            fires++;
        }

        if(fe->getmask() & mask & AE_WRITABLE)
        {
            fe->write_file_proc(fd);
            fires++;
        }

        process++;
    }
    return process;
}


void
eventloop::loop()
{
    while (!stop)
    {
        process_events(AE_ALL_EVENTS|AE_CALL_AFTER_SLEEP);
    }
}


