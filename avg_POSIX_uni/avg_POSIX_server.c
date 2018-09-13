#include "avg_POSIX_messg.h"

int main(int argc, char *argv[])
{
	mqd_t my_msqid, your_msqid;
	MESSG msg_rcvd, msg_send;
	unsigned int type;

	if (argc < 2)
	{
		printf("Usage: avg_server <initial_value>\n");
		exit(1);
	}

	// just in case it is still there (if we quit using ^C for example)
	if (mq_unlink(SERVER_NAME) == 0)
		printf("SRV: Message queue %s removed.\n", SERVER_NAME);

	float avg = strtol(argv[1], NULL, 0);

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
			printf("SRV: %s REPORTS: %.2f\n", &(msg_rcvd.sender_name[1]), msg_rcvd.val); // skip '/'

			avg = (avg + msg_rcvd.val ) / 2;
			printf("SRV: CURRENT AVERAGE NUMBER: %.2f\n", avg);

			strcpy(msg_send.sender_name, SERVER_NAME);
			msg_send.val = avg;

			if ((your_msqid = mq_open(msg_rcvd.sender_name, O_WRONLY)) < 0)
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
