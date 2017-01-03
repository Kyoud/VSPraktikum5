#include <cstdlib>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <sys/time.h>
#include <fstream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/resourceserver.h"// service

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace std::chrono;
/*
 * 
 */
int main(int argc, char** argv) {
    int n1 = 2, n2 = 3;
    boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    
    resourceserverClient client(protocol);
    try {
	transport->open();
	std::cout<<"lock the server" <<std::endl;
	client.lock();
	transport->close();
	
	
    } catch (TException &tx) {
        std::cout << "error calling server: " << tx.what() << std::endl;
    }
    return 0;
}

