#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<unistd.h>
#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<map>

#define MAXLINE 4096

// 设计两个线程
// 线程1:每隔一段时间发送数据,1到100
// 线程2:接受数据,会一直堵塞


std::mutex my_mutex;

// 分别为:产生的所有的数据编号,传输过去的数据编号,收到的数据编号
std::vector<int> KFsDepthAll;
std::vector<bool> KFsDepthTrans;
std::vector<bool> KFsDepthRece;
int received_int;


char buff_res[4096]; 
char sendline[100];

int get_client_connnect(int host)
{
    int my_server;
    struct sockaddr_in  servaddr;
    if( (my_server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(host);
    if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
        printf("inet_pton error for 127.0.0.1:%d\n",host);
        return 0;
    }

    if( connect(my_server, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    return my_server;
}

void send_data(char * code, int my_server)
{
    if( send(my_server, code, strlen(code), 0) < 0)
    {
       printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    }
}
void receive_data(int my_server)
{
    int n = recv(my_server, buff_res, MAXLINE, 0);
    buff_res[n] = '\0';
    received_int = std::stoi(buff_res);
    printf("recv msg from client: %d\n", received_int);
}

void task1(int my_server)
{
    int numToTrans = 0;
    printf("send msg to server: \n");
    while(1){
        // 自定义需要发送的数据(变量)
        numToTrans ++;
        sprintf(sendline,"%d",numToTrans);
        my_mutex.lock();
        KFsDepthAll[numToTrans-1] = numToTrans;
        KFsDepthTrans[numToTrans-1] = true;
        KFsDepthRece[numToTrans-1] = false;        
        send_data(sendline,my_server);
        my_mutex.unlock();
        std::cout << "send data:" << numToTrans << std::endl;
        // 退出,自定义条件
        if (numToTrans > 100)
            break;
        usleep(10000);
    }
}
void task2(int my_server)
{
    int stop = 0;
    while(1)
    {
        // 自定义退出条件
        stop++;
        if (stop>50) break;
        receive_data(my_server);
        my_mutex.lock();
        KFsDepthRece[received_int-1] = true;        
        my_mutex.unlock();
    }
}

int main(int argc, char** argv)
{
    int my_server1 = get_client_connnect(5001);
    int my_server2 = get_client_connnect(5002);
    
    KFsDepthAll.resize(120);
    KFsDepthTrans.resize(120);
    KFsDepthRece.resize(120);

    std::thread th1(task1,my_server1);
    std::thread th2(task2,my_server2);
    th1.join();
    th2.join();
    
    for (int i = 0; i<KFsDepthAll.size(); i++)
        std::cout << "size:" << KFsDepthAll[i] << " send:" << KFsDepthTrans[i] << "Receive:" << KFsDepthRece[i] << std::endl;
    close(my_server1);
    close(my_server2);
    return 0;
}
