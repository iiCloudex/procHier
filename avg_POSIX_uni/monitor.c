#include "avg_POSIX_messg.h"

// Description: Function adds each previousTemperature
// value in the array and returns the sum.
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
	if (mq_unlink(MONITOR_QUEUE) == 0)
		printf("SRV: Message queue %s removed.\n", MONITOR_QUEUE);

	int numOfNodes = atoi(argv[2]); //Make 4 a number
	TEMPERATURE nodeData[numOfNodes];

	for(int i = 0; i < numOfNodes; i++)
	{
		nodeData[i].previousTemperature = 0;
	}

	//Start forking client nodes
	for(int i = 0; i < numOfNodes; i++)
	{
		nodeData[i].previousTemperature = 0; //initialize each spot in array as 0

		//Fork 4 children
		if(i < numOfNodes)
		{
			pid = fork();

			if(pid < 0)
			{
				oops("SVR: Fork Failed!", errno);
			}
			else if(pid == 0) //If child, run client node
			{
				printf("I am the child\n");
				execlp("node", "node", i+1, argv[i+3], (char *) NULL);
				printf("SVR: Child got passed execlp!\n");
			}
			else{} //DO NOTHING
		} 
	}

	
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

	while (true)
	{
		//If we recieved a message successfully
		if (mq_receive(my_msqid, (char*) &msg_rcvd, MAX_MSG_SIZE, &type) >= 0)
		{
			//Go to spot in nodeData array
			TEMPERATURE currNodeSpot = nodeData[msg_rcvd.nodeId - 1];

			//Need to check if all spots in nodeData are all stable if they are then terminate.

			if(currNodeSpot.previousTemperature == msg_rcvd.temperature)
			{
				msg_send.stable = true;

			}
			else //Do calculation and prepare a message
			{
				printf("SRV: NODE %d REPORTS: %.2f\n", msg_rcvd.nodeId, msg_rcvd.temperature);

				sumOfClients = arraySum(nodeData);
				new_integrated_temp = (2 * currNodeSpot.previousTemperature + sumOfClients) / 6;
				currNodeSpot.previousTemperature = msg_rcvd.temperature;

				printf("SRV: CURRENT AVERAGE NUMBER: %.2f\n", new_integrated_temp);

				msg_send.temperature = new_integrated_temp;
			}

			sprintf(your_msqid, "/%s_%d", NODE_NAME_PREFIX, msg_rcvd.nodeId);
			if (mq_open( your_msqid, O_WRONLY) < 0)
				oops("SRV: Error opening a client's queue.", errno);

			if (mq_send(your_msqid, (const char*) &msg_send, sizeof(msg_send), (unsigned int) TYPE) < 0)
				oops("SRV: Cannot respond to a client.", errno);
		}
		else
			oops("SRV: Error receiving data.", errno);
	}

	mq_unlink(MONITOR_QUEUE);

	exit(EXIT_SUCCESS);
}
