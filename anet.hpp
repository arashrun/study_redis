


class anet
{
    int port;               //端口号
    string bindaddr;
    int ipfd[CONFIG_BINDADDR_MAX];//套接字数组

public:
    using ptr = std::shared_ptr<anet>;
    anet(string bindaddr);
    int setblock(int fd, int non_block);
    int tcpserver(int af_family);
    int tcpserverlisten();
    int listentoport(int port);

};



int
anet::listentoport(int port=6379)
{
    this->port = port;
    if(tcpserver(AF_INET)==A_ERR) return -1;
    if(tcpserverlisten()==A_ERR) return -2;
}
