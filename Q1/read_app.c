#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int Fd;
//	char Ubuff[] = "This is the user buffer ==> sending data to the kernel";
	char Kbuff[100];
	Fd = open("/dev/DESD_Device", O_RDWR, 0777);

	if(Fd < 0)
	{
		printf("\nError opening device.\n");
		exit(1);
	}

//	write(Fd, Ubuff,sizeof(Ubuff));  // writing to the kernal
	read(Fd, Kbuff, 50);
	printf("\nThe reading data ==> %s\n", Kbuff);  // reading data from kernal
	close(Fd);
}
