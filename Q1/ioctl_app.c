#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "main.h"

int main()
{
	int Fd;
	struct Status test = {10 , "KAJOL"};
	Fd = open("/dev/DESD_Device", O_RDWR, 0777);

	if(Fd < 0)
	{
		printf("\nError opening device.\n");
		exit(1);
	}

	write(Fd, STATUS,sizeof(test));
	printf("ioctl writting was successfull\n");
	close(Fd);
return 0;

}
