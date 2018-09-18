#include "avg_POSIX_messg.h"

int main(int argc, char *argv[])
{
	mqd_t my_msqid, server_msqid;
	MESSG msg_rcvd, msg_send;
	unsigned int type;

	//Check for proper arguments
	if (argc < 3)
	{
		printf("Usage: node <name> <value>\n");
		exit(1);
	}

	//Try to open monitor queue
	if ((server_msqid = mq_open(MONITOR_QUEUE, O_WRONLY)) < 0)
		oops("CLI: Error opening the monitor queue.", errno);


	//Initialize msg_send variables
	msg_send.stable = false;
	msg_send.nodeId = atoi(argv[1]);
	msg_send.temperature = strtol(argv[2], NULL, 10);
	char *name = malloc(sizeof(char));
	sprintf(name, "/%s%d", NODE_NAME_PREFIX, msg_send.nodeId);

	//Send msg_send to monitor
	if (mq_send(server_msqid, (char *) &msg_send, sizeof(MESSG), (unsigned int) TYPE) < 0)
	{
		oops("CLI: Error sending a message to server.", errno);
	}
	else
	{
		printf("%s: Sent message to monitor\n", name);
	}

	
	

	// just in case the old queue is still there (e.g., after ^C)
	if (mq_unlink(name) == 0 )
		printf("%s: Message queue %s removed from system.\n", name, name);

	// initialize the queue attributes
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;


	//Open a queue for the node
	if ((my_msqid = mq_open(name, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0)
	{
		oops("CLI: Error opening a node queue.", errno);
	}
	else
	{
		printf("%s: Opened queue for %s\n", name, name);
	}
	
	
	//Continuously read messages
	while(true)
	{
		//If we are able to grab a message
		if (mq_receive(my_msqid, (char *) &msg_rcvd, MAX_MSG_SIZE, &type) >= 0)
		{

			if(msg_rcvd.stable)
			{
				printf("NODE_%d TERMINATING...", msg_send.nodeId);
				mq_unlink(name);
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("%s: SERVER REPORTS CURRENT AVERAGE NUMBER: %.2f\n", name, msg_rcvd.temperature);

				msg_send.stable = false;
				msg_send.nodeId = msg_rcvd.nodeId;
				msg_send.temperature = (msg_send.temperature * 3 + 2 * msg_rcvd.temperature) / 5;

				printf("%s: NEW TEMPERATURE: %f\n", name, msg_send.temperature);

			}
			
			if (mq_send(server_msqid, (char *) &msg_send, sizeof(MESSG), (unsigned int) TYPE) < 0)
			{
				oops("CLI: Error sending a message to monitor.", errno);
			}
			else
			{
				printf("%s: Sent new message to monitor\n", name);
			}

		}
		else
			oops("CLI: Error receiving data.", errno);

	}
	mq_unlink(name);

	exit(EXIT_SUCCESS);
}
