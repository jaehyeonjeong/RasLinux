#include <stdio.h>

typedef struct
{
	int id;
	char name[16];
	char phoneNum[20];
}ClientInfo;

int main()
{
	ClientInfo pInfo[2] = {{100, "Park", "010-1234-5637"},
				{101, "Lee", "010-9876-5432"}};

	ClientInfo p;
	int n = 0;
	
	FILE* fp = fopen("client.txt", "w+");
	fwrite(pInfo, sizeof(ClientInfo), 2, fp);
	fclose(fp);

	fp = fopen("client.txt", "r");
	while(1)
	{
		n = fread(&p, sizeof(ClientInfo), 1, fp);
		if(n <= 0)break;
		printf("ID : %d\n", p.id);
		printf("Name : %s\n", p.name);
		printf("Phone number : %s\n\n", p.phoneNum);
	}
	return 0;
}

