#include "avg_POSIX_messg.h"

int main(int argc, char *argv[])
{
	pid_t pid;
	mqd_t my_msqid, your_msqid;
	MESSG msg_rcvd, msg_send;
	unsigned int type;
	int numOfNodes;

	if (argc < 7)
	{
		printf("Usage: avg_server <initial_value> <numOfNodes> <initial_node_vals>\n");
		exit(1);
	}

	numOfNodes = atoi(argv[2]); //Make 4 a number

	// just in case it is still there (if we quit using ^C for example)
	if (mq_unlink(MONITOR_QUEUE) == 0)
		printf("SRV: Message queue %s removed.\n", MONITOR_QUEUE);

	float monitorTemp = strtol(argv[1], NULL, 10);
	float sumOfClients = 0;
	float new_integrated_temp = 0;

	// initialize the queue attributes
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	if ((my_msqid = mq_open(MONITOR_QUEUE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0)
		oops("SRV: Error opening a monitor queue.", errno);

	printf("SRV: Message queue %s created.\n", MONITOR_QUEUE);

	//Start forking client nodes
	for(int i = 0; i < numOfNodes; i++)
	{
		pid = fork();

		if(pid < 0)
		{
			oops("SVR: Fork Failed!", errno);
		}
		else if(pid == 0) //If child, run client node
		{
			char buffer[5];
			sprintf(buffer, "%d", i+1);
			execlp("node", "node", buffer, argv[i+3], (char *) NULL);
		}
		else{} //DO NOTHING
	} 
	

	TEMPERATURE nodeData[numOfNodes];

	for(int i = 0; i < numOfNodes; i++)
	{
		nodeData[i].previousTemperature = 0;
	}


	int isStable = 0;

	while (isStable != 4)
	{
		//If we recieved a message successfully
		if (mq_receive(my_msqid, (char*) &msg_rcvd, MAX_MSG_SIZE, &type) >= 0)
		{
			if(nodeData[msg_rcvd.nodeId - 1].previousTemperature == msg_rcvd.temperature)//Old temp == received temp
			{

				msg_send.stable = true;
				isStable++;
			}
			else
			{
				char name[10];
				sprintf(name, "/%s%d", NODE_NAME_PREFIX, msg_rcvd.nodeId);

				if ( (your_msqid = mq_open(name, O_WRONLY)) < 0)
					oops("SRV: Error opening a client's queue.", errno);
			
				printf("SRV: NODE_%d REPORTS: %.2f\n", msg_rcvd.nodeId, msg_rcvd.temperature);

				sumOfClients = sumOfClients - nodeData[msg_rcvd.nodeId - 1].previousTemperature + msg_rcvd.temperature;
				monitorTemp = (2 * monitorTemp + sumOfClients) / 6;
				nodeData[msg_rcvd.nodeId - 1].previousTemperature = msg_rcvd.temperature;

				printf("SRV: CURRENT AVERAGE NUMBER: %.2f\n", new_integrated_temp);
				msg_send.temperature = monitorTemp;
			}
			
			msg_send.nodeId = msg_rcvd.nodeId;

			if (mq_send(your_msqid, (const char*) &msg_send, sizeof(msg_send), (unsigned int) TYPE) < 0)
				oops("SRV: Cannot respond to a client.", errno);
		}
		else
			oops("SRV: Error receiving data.", errno);
	}

	mq_unlink(MONITOR_QUEUE);

	exit(EXIT_SUCCESS);
}