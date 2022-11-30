#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int listDir(char *arg)
{
	DIR* pdir;
	struct dirent* dirt;
	struct stat statBuff;
	struct passwd* username;
	struct group* groupname;
	struct tm* t;
	int i = 0, count = 0;
	char* dirName[255], buf[255], permission[11], mtime[20];

	memset(dirName, 0, sizeof(dirName));
	memset(&dirt, 0, sizeof(dirt));
	memset(&statBuff, 0, sizeof(statBuff));

	if((pdir = opendir(arg)) <= 0)
	{
		perror("opendir");
		return -1;
	}

	chdir(arg);
	getcwd(buf, 255);
	printf("\n%s: Directory\n", arg);

	while((dirt = readdir(pdir)) != NULL)
	{
		lstat(dirt->d_name, &statBuff);
		if(S_ISDIR(statBuff.st_mode))
			permission[0] = 'd';
		else if(S_ISLNK(statBuff.st_mode))
			permission[0] = 'l';
		else if(S_ISCHR(statBuff.st_mode))
			permission[0] = 'c';
		else if(S_ISBLK(statBuff.st_mode))
			permission[0] = 'b';
		else if(S_ISSOCK(statBuff.st_mode))
			permission[0] = 's';
		else if(S_ISFIFO(statBuff.st_mode))
			permission[0] = 'P';
		else
			permission[0] = '-';

		permission[1] = statBuff.st_mode&S_IRUSR? 'r' : '-';
		permission[2] = statBuff.st_mode&S_IWUSR? 'w' : '-';
		permission[3] = statBuff.st_mode&S_IXUSR? 'x' :
				statBuff.st_mode&S_ISUID? 'S' : '-';

		permission[4] = statBuff.st_mode&S_IRGRP? 'r' : '-';
		permission[5] = statBuff.st_mode&S_IWGRP? 'w' : '-';
		permission[6] = statBuff.st_mode&S_IXGRP? 'x' :
				statBuff.st_mode&S_ISGID? 'S' : '-';
		
		permission[7] = statBuff.st_mode&S_IROTH? 'r' : '-';
		permission[8] = statBuff.st_mode&S_IWOTH? 'w' : '-';
		permission[9] = statBuff.st_mode&S_IXOTH? 'x' : '-';

		if(statBuff.st_mode & S_IXOTH)
		{
			permission[9] = statBuff.st_mode&S_ISVTX? 't' : 'x';
		}
		else
		{
			permission[9] = statBuff.st_mode&S_ISVTX? 'T' : '-';
		}


		permission[10] = '\0';

		if(S_ISDIR(statBuff.st_mode) == 1)
		{
			if(strcmp(dirt->d_name, ".") && strcmp(dirt->d_name, ".."))
			{
				dirName[count] = dirt->d_name;
				count = count + 1;
			}
		}

		username = getpwuid(statBuff.st_uid);
		groupname = getgrgid(statBuff.st_gid);
		t = localtime(&statBuff.st_mtime);

		sprintf(mtime, "%04d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);

		printf("%s %2ld %s %s %9ld\t%s\t%s\n",
			permission, statBuff.st_nlink,
			username->pw_name, groupname->gr_name,
			statBuff.st_size, mtime, dirt->d_name);
	}

	for(i = 0; i < count; i++)
	{
		if(listDir(dirName[i] == -1))break;
	}

	printf("\n");
	closedir(pdir);
	chdir("..");
	return 0;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage : %s directory_name.\n", argv[0]);
		return -1;
	}	
	listDir(argv[1]);

	return 0;
} 
