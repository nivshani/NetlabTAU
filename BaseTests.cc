#include <iostream>
#include <gtest/gtest.h>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <pthread.h>

#include "L2.h"
#include "L3.h"
#include "L4_TCP.h"
#include "NIC.h"
#include "L2_ARP.h"
#include "NIC_Cable.h"
#include "L0_buffer.h"


class ServerClientTest : public ::testing::Test {
protected:
    
    inet_os inet_server;
    inet_os inet_client;
    NIC nic_server;
    NIC nic_client;
    L2_impl datalink_server;
    L2_impl datalink_client;
    L2_ARP_impl arp_server;
    L2_ARP_impl arp_client;
    netlab::L5_socket_impl* ListenSocket;
    netlab::L5_socket_impl* ConnectSocket;
    sockaddr_in service;
    sockaddr_in clientService;

    ServerClientTest()
    : inet_server(),
      inet_client(),
      nic_server(inet_server, "10.0.0.10", "aa:aa:aa:aa:aa:aa", nullptr, nullptr, true, 
      "(arp and ether src bb:bb:bb:bb:bb:bb) or (tcp port 8888 and not ether src aa:aa:aa:aa:aa:aa)"),
      nic_client(inet_client, "10.0.0.15", "bb:bb:bb:bb:bb:bb", nullptr, nullptr, true, 
      "(arp and ether src aa:aa:aa:aa:aa:aa) or (tcp port 8888 and not ether src bb:bb:bb:bb:bb:bb)"),
      datalink_server(inet_server),
      datalink_client(inet_client),
      arp_server(inet_server, 10, 10000),
      arp_client(inet_client, 10, 10000),
      ListenSocket(nullptr),
      ConnectSocket(nullptr) {
      
    }

    void SetUp() override {
        // Code to set up the test environment (run before each test)

        // Initializing servers, protocols, and sockets
        inet_server.domaininit();
        inet_client.domaininit();
        arp_server.insertPermanent(nic_server.ip_addr().s_addr, nic_server.mac());
        arp_client.insertPermanent(nic_client.ip_addr().s_addr, nic_client.mac());

        // Setting up ListenSocket
        ListenSocket = new netlab::L5_socket_impl(AF_INET, SOCK_STREAM, IPPROTO_TCP, inet_server);
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_server.nic()->ip_addr().s_addr;
        service.sin_port = htons(8888);
        ListenSocket->bind((SOCKADDR*)&service, sizeof(service));
        ListenSocket->listen(5);

        // Setting up ConnectSocket
        ConnectSocket = new netlab::L5_socket_impl(AF_INET, SOCK_STREAM, IPPROTO_TCP, inet_client);
        clientService.sin_family = AF_INET;
        clientService.sin_addr.s_addr = inet_server.nic()->ip_addr().s_addr;
        clientService.sin_port = htons(8888);
        ConnectSocket->connect((SOCKADDR*)&clientService, sizeof(clientService));
    }

    void TearDown() override {
        
        delete ListenSocket;
        delete ConnectSocket;
        inet_client.stop_fasttimo();
        inet_client.stop_slowtimo();
        inet_server.stop_fasttimo();
        inet_server.stop_slowtimo();
    }

    ~ServerClientTest() {
        // Code for clean up after all tests run
    }
};

TEST_F(ServerClientTest, ConnectionTest) {
    // Test to check if the server accepts the connection
    netlab::L5_socket_impl* AcceptSocket = nullptr;
    AcceptSocket = ListenSocket->accept(nullptr, nullptr);
    ASSERT_NE(AcceptSocket, nullptr);

    // Additional assertions to validate the connection can go here

    delete AcceptSocket;
}

TEST_F(ServerClientTest, CommunicationTest) {
    // Test to check if communication between client and server works properly
    // Send and receive data, then validate the data received is as expected

    // Example: ASSERT_EQ(sent_data, received_data);
}

TEST_F(ServerClientTest, ShutdownTest) {
    // Test to check proper shutdown of sockets and servers
    ListenSocket->shutdown(SD_RECEIVE);
    // Additional assertions to validate the shutdown can go here
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
