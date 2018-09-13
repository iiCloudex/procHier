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

	if ((server_msqid = mq_open(SERVER_NAME, O_WRONLY)) < 0)
		oops("CLI: Error opening a queue.", errno);

	sprintf(msg_send.sender_name, "/%s", argv[1]); // need '/' as a prefix
	msg_send.val = strtol(argv[2], NULL, 10);

	if (mq_send(server_msqid, (char *) &msg_send, sizeof(MESSG), (unsigned int) TYPE) < 0)
		oops("CLI: Error sending a message.", errno);

	// just in case the old queue is still there (e.g., after ^C)
	if (mq_unlink(msg_send.sender_name) == 0)
		printf("CLI: Message queue %s removed from system.\n", msg_send.sender_name);

	// initialize the queue attributes
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	if ((my_msqid = mq_open(msg_send.sender_name, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0)
		oops("CLI: Error opening a client queue.", errno);

	if (mq_receive(my_msqid, (char *) &msg_rcvd, MAX_MSG_SIZE, &type) < 0)
		oops("CLI: Error receiving data.", errno);

	printf("CLI: SERVER REPORTS CURRENT AVERAGE NUMBER: %.2f\n", msg_rcvd.val);

	mq_unlink(msg_send.sender_name);

	exit(EXIT_SUCCESS);
}
