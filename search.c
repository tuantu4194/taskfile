#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#ifndef MAX_ARG
#define MAX_ARG 10
#endif

char *keywords[MAX_ARG];
int arg;
 
void dir(char *directory, char *parent);

void search(char *filename);

int main(int argc, char const *argv[])
{
	char current[PATH_MAX],parent[PATH_MAX];
    int check=0;
    /* Change to or set directory */
    if (argc>11){perror("Too many argument!!!"); return(1);}
    strcpy(current,argv[1]);
    if (chdir(current)){getcwd(current,PATH_MAX);check=1;}
    
    /* fetch parent directory */
	chdir("..");
	getcwd(parent,PATH_MAX);
	
    /* get keywords */
    arg=argc;
    if (check==0 && arg==2){fprintf(stderr,"No keyword entered!!!\n"); return(1);}
    if (check == 1){ 
        keywords[0]=malloc(strlen(argv[1])+1);
        strcpy(keywords[0],argv[1]);
    }

    for (int i=2;i<argc;i++){
        keywords[i-1]=malloc(strlen(argv[i])+1);
        strcpy(keywords[i-1],argv[i]);
    }
    
    /* run */
	dir(current,parent);
    
    for (int i=0;i<argc;i++){
        free(keywords[i]);
    }

	return(0);
}

void dir(char *directory, char *parent)
{
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];

    /* Change to the named directory */
	if(chdir(directory))
	{
		fprintf(stderr,"Error changing to %s\n",directory);
		exit(1);
	}

    /* open the directory */
	folder = opendir(".");
	if(folder == NULL){
		fprintf(stderr,"Unable to read directory %s\n",directory);
		exit(1);
	}

    getcwd(pathname,PATH_MAX);

    while (entry=readdir(folder)){
        stat(entry->d_name,&filestat);

        if(S_ISDIR(filestat.st_mode)){              //if directory
            /* skip the . and .. entries */
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
				{continue;}
			/* get size of directory */
		    dir(entry->d_name,pathname);
		}
        else{                   //if file
            search(entry->d_name);
        }
    }
    closedir(folder);
    chdir(parent);
}

void search(char *filename)
{
    int line_num=1, check=0;
    char line[PATH_MAX];
    char pathname[PATH_MAX];
    FILE *f;
    f=fopen(filename,"r");
    if (f==NULL){exit(1);}
    getcwd(pathname,PATH_MAX);

    while (!feof(f))
        {
            fgets(line,PATH_MAX,f);
            line[strlen(line)-1]='\0';
            for (int i=0;i<arg;i++)
                {
                    if (keywords[i]!=NULL){
                        if (strstr(line,keywords[i])!=NULL){
                            if (check==0){
                                printf("%s/%s\n",pathname,filename);
                                check=1;
                            }
                        printf("\t%s\tL%d\n",keywords[i],line_num);
                        }  
                    }
                }   
            line_num++;
        }  
    fclose(f);  
} 
