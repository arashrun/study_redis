

#include "server.hpp"

redis_server server;

command_dic::rediscommandtable[]={
    redis_command("set",setcommand,-3,0),
    redis_command("get",getcommand,2,0),
    redis_command("del",delcommand,-2),
    redis_command("ping",pingcommand,-1,0)
};

command_dic::command_dic()
{
    populateCommandTable();
}

void
command_dic::populateCommandTable()
{
    int numcommands = sizeof(rediscommandtable)/sizeof(redis_command);
    for(int i =0;i<numcommands;i++)
    {
        redis_command *c = rediscommandtable+i;
        int retval = dic.insert({c->name,c->proc});
    }
}



void
redis_server::init_server_config()
{
    //初始化命令字典
    ct = new command_dic();
}

void
redis_server::init_server()
{
    current_client = nullptr;
    clients = new forword_list<client>;
    //初始化网络
    nt = anet("127.0.0.1");
    nt.listentoport(port);

    eventloop = eventloop(100);
    if(eventloop == nullptr)
        PERROR("create eventloop:")
    hder = handler();

    if(eventloop.creat_file_event(nt.get_sfd(),AE_READABLE,hder.accepthandler,NULL))

}


int main(int argc, char const *argv[])
{
    server.init_server_config();
    server.init_server();
    return 0;
}
