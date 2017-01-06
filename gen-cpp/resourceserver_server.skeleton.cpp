// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it

//This part is for beeing a Server
#include "resourceserver.h"
#include <iostream>
#include <vector>
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
 public:
  resourceserverHandler() {
    // Your initialization goes here

	
  }

  void lock() {
	//unlock();
//std::vector<resourceserverClient> servers;
    //connect to resource server 9091 res1
    boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", 9091));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    resourceserverClient res1(protocol);
    
    servers.push_back(res1);
    // Your implementation goes here
	 try {
	transport->open();
	std::cout<<"request server" <<std::endl;
	res1.request("test", 1);
	transport->close();
	
	
    } catch (TException &tx) {
        std::cout << "error calling server: " << tx.what() << std::endl;
    }
    printf("lock\n");
  }

  void unlock() {
    // Your implementation goes here
    printf("unlock\n");
  }

  void request(const std::string& name, const int64_t time) {
    // Your implementation goes here

    printf("request\n");
	//res1.ack();
  }

  void ack(const std::string& name, const int64_t time) {
    // Your implementation goes here
    printf("ack\n");
  }
private:
std::vector<resourceserverClient> servers;

};

int main(int argc, char **argv) {
  int clock =0;
  int port = 9090;
  std::cout<< "Please enter the port number for this server (4 Nubers)"<< std::endl;
  std::cin>>port;

//initialize server profile
  shared_ptr<resourceserverHandler> handler(new resourceserverHandler());
  shared_ptr<TProcessor> processor(new resourceserverProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());



  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);


  server.serve();
  return 0;
}

