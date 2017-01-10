//This part is for beeing a Server
#include <iostream>
#include <vector>
#include <string>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

//This part is for connecting to other Server
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "resourceserver.h"// service

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

//vector for the lamport servers

class resourceserverHandler : virtual public resourceserverIf {
private:
    std::vector<resourceserverClient> servers;
    int clock = 0;
    std::string name = "";
    bool locktry =false;

public:

    resourceserverHandler(int own_port) {
	std::cout <<"connection of server "<<own_port<<std::endl;
        name = "Server"+own_port;
        // Your initialization goes here
        //connect to resource server 9091 res1
        for (int i = 9090; i < 9094; i++) {
	std::cout<<i<<std::endl;
            if (own_port != i) { 
		std::cout << "connect to server with port: "<<i<<std::endl;
                boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", i));
                boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
                boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
                resourceserverClient res1(protocol);
                servers.push_back(res1);

            }
        }
    }

    void lock() {
        //unlock();
        // Your implementation goes here
        for (int i = 0; i < servers.size(); i++) {
            try {
                servers.at(i).getOutputProtocol()->getTransport()->open();
                std::cout << "request server" << std::endl;
                servers.at(i).request(name, 1);
                servers.at(i).getOutputProtocol()->getTransport()->close();


            } catch (TException &tx) {
                std::cout << "error calling server: " << tx.what() << std::endl;
            }
        }
        printf("lock\n");
        sleep(5);
        
    }

    void unlock() {
        // Your implementation goes here
        std::cout<<"unlock"<<std::endl;
    }

    void request(const std::string& name, const int64_t time) {
        // Your implementation goes here

        std::cout<<"request for resource lock from server: " +name<<std::endl;
	if (locktry == true){
	std::cout<<"this server is also trying to lock the resource"<<std::endl;
	}
        
    }

    void ack(const std::string& name, const int64_t time) {
        // Your implementation goes here
        printf("ack\n");
    }


};

int main(int argc, char **argv) {
    int clock = 0;
    int port = 9090;
    std::cout << "Please enter the port number for this server (4 Nubers)" << std::endl;
    std::cin>>port;

    //initialize server profile
    shared_ptr<resourceserverHandler> handler(new resourceserverHandler(port));
    shared_ptr<TProcessor> processor(new resourceserverProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());



    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

}

