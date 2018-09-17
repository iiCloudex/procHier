#include "avg_POSIX_messg.h"

int main(int argc, char *argv[])
{
	mqd_t my_msqid, server_msqid;
	MESSG msg_rcvd, msg_send;
	unsigned int type;

	if (argc < 3)
	{
		printf("Usage: avg_client <name> <value>\n");
		exit(1);
	}

	if ((server_msqid = mq_open(MONITOR_QUEUE, O_WRONLY)) < 0)
		oops("CLI: Error opening the monitor queue.", errno);

	msg_send.stable = false;
	msg_send.nodeId = atoi(argv[1]);
	msg_send.temperature = strtol(argv[2], NULL, 10);

	if (mq_send(server_msqid, (char *) &msg_send, sizeof(MESSG), (unsigned int) TYPE) < 0)
		oops("CLI: Error sending a message to server.", errno);

	// just in case the old queue is still there (e.g., after ^C)
	if (mq_unlink(sprintf(my_msqid, "/NODE_%d", msg_send.nodeId)) == 0 )
		printf("CLI: Message queue %s removed from system.\n", sprintf(NODE_NAME_PREFIX, "%s", msg_send.nodeId) );

	// initialize the queue attributes
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;



	if ( mq_open(my_msqid, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr) < 0)
	{
		oops("CLI: Error opening a client queue.", errno);
	}
	else
	{
		printf("CLI: Successfully opened client queue. Name: %s\n", my_msqid);
	}

	while(true)
	{
		if (mq_receive(my_msqid, (char *) &msg_rcvd, MAX_MSG_SIZE, &type) >= 0)
		{
			if(msg_rcvd.stable)
			{
				printf("NODE_%d TERMINATING...", msg_send.nodeId);
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("CLI: SERVER REPORTS CURRENT AVERAGE NUMBER: %.2f\n", msg_rcvd.temperature);

				float new_node_temp = (msg_send.temperature * 3 + 2 * msg_rcvd.temperature) / 5;
				msg_send.temperature = new_node_temp;

			}

		}
		else
			oops("CLI: Error receiving data.", errno);

		


		
	}
	mq_unlink(my_msqid);

	exit(EXIT_SUCCESS);
}
