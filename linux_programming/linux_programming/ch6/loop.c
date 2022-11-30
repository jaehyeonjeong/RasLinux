#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int i;
	for(i = 0; ; i++)
	{
		printf("%10d\r", i);
		fflush(NULL);
		usleep(100);
	}
	return 0;
}
