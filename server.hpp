

#define CONFIG_BINDADDR_MAX 16

class redis_db
{
    map<string, rtype> allk_v;
};


class command_dic
{
    static redis_command rediscommandtable[4];
    map<string,redis_command> dic;
    void populateCommandTable();

public:
        command_dic();
};

class redis_command
{
    string name;
    commandproc *proc;
    int command_size;
    int flag;
    
public:
    using commandproc = std::function<void(client*)>;
    redis_command(string name, commandproc *proc, int size, int flag);
};

class client
{
public:
    using ptr = std::shared_ptr<client>;
private:
    string querybuf;
    string outbuf;
    vector<string> argv;
    int argc;
    redis_command cmd;
    redis_db db;
};

class redis_server
{
public:
    void init_server_config();
    void init_server();
private:
    command_dic ct;
    eventloop eventloop;
    client::ptr current_client;
    forward_list<client> clients;

    //network
    int port;
    anet nt;

};



