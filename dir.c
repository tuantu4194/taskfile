#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

long int get_dir_size(char *directory, char *parent);

void print_size(int size)
{
    float fsize=size;
    char *mesure;
    if (fsize<1000) {mesure="B";} 
            else if (fsize<1000000) {mesure="KB";fsize/=1000;}
            else {mesure="MB";fsize/=1000000;}
            printf("%.3f %s\n",fsize,mesure); 
}

int main(int argc, char const *argv[])
{
	char current[PATH_MAX],parent[PATH_MAX];
    /* Change to or set current directory */
	if(argc==2)
	{
		strcpy(current,argv[1]);
	}
	else
	{
		getcwd(current,PATH_MAX);
	}

	if(chdir(current))
	{
		fprintf(stderr,"Error changing to %s\n",current);
		exit(1);
	}
 
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];
    long int total=0;

    /* open the directory */
	folder = opendir(".");
	if(folder == NULL)
	{
		fprintf(stderr,"Unable to read directory %s\n",current);
		exit(1);
	}

    while (entry=readdir(folder))
    {
        stat(entry->d_name,&filestat);
        printf("%s\t\t\t\t\t",entry->d_name);

        if(S_ISDIR(filestat.st_mode))
	{              //skip file with "." and ".."
            
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
			{
				printf("\n");continue;
			}
			/* get size of directory */
		    total+=get_dir_size(entry->d_name,pathname); 
            print_size(get_dir_size(entry->d_name,pathname));
	}
        else
	{                   
            total+=filestat.st_size;
            print_size(filestat.st_size);
        }
    }
    printf("Total size of this Directory:\t\t"); print_size(total);
    closedir(folder);

	return(0);
}

long int get_dir_size(char *directory, char *parent)
{
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];
    long int size=0;
    /* Change to the named directory */
	if(chdir(directory))
	{
		fprintf(stderr,"Error changing to %s\n",directory);
		exit(1);
	}

	/* open the directory */
	folder = opendir(".");
	if(folder == NULL)
	{
		fprintf(stderr,"Unable to read directory %s\n",directory);
		exit(1);
	}

	getcwd(pathname,PATH_MAX);
	printf("%s\n",pathname);
	/* Look for a subdirectory */
	while( (entry=readdir(folder)) )
	{
		stat(entry->d_name,&filestat);
		/* look for directories */
		if( S_ISDIR(filestat.st_mode) )
		{
			/* skip the . and .. entries */
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
				continue;
			/* recurse to get size*/
			size+=get_dir_size(entry->d_name,pathname);
		}
        else
        {
            size+=filestat.st_size;
        }
        
	}
	closedir(folder);
	chdir(parent);
    return size;
}
