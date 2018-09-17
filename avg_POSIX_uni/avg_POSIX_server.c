#include "avg_POSIX_messg.h"

float arraySum(TEMPERATURE *nodeData)
{
	float sum = 0;

	for(int i = 0; i < sizeof(nodeData); i++)
	{
		sum += nodeData[i].previousTemperature;
	}

	return sum;
}

int main(int argc, char *argv[])
{
	pid_t pid;
	mqd_t my_msqid, your_msqid;
	MESSG msg_rcvd, msg_send;
	unsigned int type;

	if (argc < 7)
	{
		printf("Usage: avg_server <initial_value> <numOfNodes> <initial_node_vals>\n");
		exit(1);
	}

	// just in case it is still there (if we quit using ^C for example)
	if (mq_unlink(SERVER_NAME) == 0)
		printf("SRV: Message queue %s removed.\n", SERVER_NAME);

	int numOfNodes = strtol(argv[2], NULL, 0);
	TEMPERATURE nodeData[numOfNodes];

	for(int i = 0; i < numOfNodes; i++)
	{
		nodeData[i].previousTemperature = 0;
		if(i < numOfNodes)
		{
			pid = fork();

			if(pid < 0)
			{
				oops("SVR: Fork Failed!", errno);
			}
			else if(pid == 0)
			{
				execlp("./avg_server", "avg_server", i+1, argv[i+3], NULL);
				printf("SVR: Child got passed exec!\n");
			}
			else{} //DO NOTHING
		} 
	}

	
	float monitorTemp = strtol(argv[1], NULL, 0);
	float sumOfClients;
	float new_integrated_temp;

	// initialize the queue attributes
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	if ((my_msqid = mq_open(SERVER_NAME, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0)
		oops("SRV: Error opening a server queue.", errno);

	printf("SRV: Message queue %s created.\n", SERVER_NAME);

	while (true)
	{
		if (mq_receive(my_msqid, (char*) &msg_rcvd, MAX_MSG_SIZE, &type) >= 0)
		{
			TEMPERATURE currNodeSpot = nodeData[msg_rcvd.nodeId - 1];

			if(currNodeSpot.previousTemperature == msg_rcvd.temperature)
			{
				msg_send.stable = true;

			}
			else
			{
				printf("SRV: NODE %d REPORTS: %.2f\n", msg_rcvd.nodeId, msg_rcvd.temperature); // skip '/'

				sumOfClients = arraySum(nodeData);
				new_integrated_temp = (2 * currNodeSpot.previousTemperature + sumOfClients) / 6;
				currNodeSpot.previousTemperature = msg_rcvd.temperature;

				printf("SRV: CURRENT AVERAGE NUMBER: %.2f\n", new_integrated_temp);

				strcpy(msg_send.nodeId, SERVER_NAME);
				msg_send.temperature = new_integrated_temp;
			}

			if ((your_msqid = mq_open((const char*) &msg_rcvd.nodeId, O_WRONLY)) < 0)
				oops("SRV: Error opening a client's queue.", errno);

			if (mq_send(your_msqid, (const char*) &msg_send, sizeof(msg_send), (unsigned int) TYPE) < 0)
				oops("SRV: Cannot respond to a client.", errno);
		}
		else
			oops("SRV: Error receiving data.", errno);
	}

	mq_unlink(SERVER_NAME);

	exit(EXIT_SUCCESS);
}
