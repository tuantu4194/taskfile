#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];
    long int total=0;

long int sz_sub_dir(char *directory, char *previous);

void output_dir_size(int size)
{
    float fsize=size;
    char *unit;
    if (fsize<1000) {unit="B";} 
            else if (fsize<1000000) {unit="KB";fsize/=1000;}
            else {unit="MB";fsize/=1000000;}
            printf("%.3f %s\n",fsize,unit); 
}

int main(int argc, char const *argv[])
{
	char current[PATH_MAX],previous[PATH_MAX];
    /* Get Given Directory */
	if(argc==2)
	{
		strcpy(current,argv[1]);
	}
	else
	{
		getcwd(current,PATH_MAX);
	}

    //cd current
	if(chdir(current))
	{
		fprintf(stderr,"Error changing to %s\n",current);
		exit(1);
	}
 
    /* open the directory */
	folder = opendir(".");
	if(folder == NULL)
	{
		fprintf(stderr,"Can't open current directory\n");
		exit(1);
	}

    while (entry=readdir(folder))
    {
        stat(entry->d_name,&filestat);
        printf("%s\t\t\t",entry->d_name);

        if(S_ISDIR(filestat.st_mode))
	{              //skip special file(current/parent)
            
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
			{
				printf("\n");continue;
			}
			/* get directory size (in total) */
		    total+=sz_sub_dir(entry->d_name,pathname); 
            output_dir_size(sz_sub_dir(entry->d_name,pathname));
	}
        else
	{                   
            total+=filestat.st_size;
            output_dir_size(filestat.st_size);
        }
    }
    printf("Total size of this Directory:\t"); output_dir_size(total);
    closedir(folder);

	return(0);
}

long int sz_sub_dir(char *directory, char *previous)
{

    /* cd */
	if(chdir(directory))
	{
		fprintf(stderr,"Error changing to %s\n",directory);
		exit(1);
	}

	/* open directory */
	folder = opendir(".");
	if(folder == NULL)
	{
		fprintf(stderr,"Can't open directory %s\n",directory);
		exit(1);
	}

	getcwd(pathname,PATH_MAX);
	printf("%s\n",pathname);
	/* Looking for a sub-dir */
	while( (entry=readdir(folder)) )
	{
		stat(entry->d_name,&filestat);
		/* if dir */
		if( S_ISDIR(filestat.st_mode) )
		{
			/* skip special files */
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
				continue;
			/* call recursion */
			size+=sz_sub_dir(entry->d_name,pathname);
		}
        else
        {
            size+=filestat.st_size;
        }
        
	}
	closedir(folder);
	chdir(previous);
    return size;
}
