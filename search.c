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
#define MAX_ARG 15
#endif

DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	char pathname[PATH_MAX];

char *key_list[MAX_ARG];
int arg;
 
void dir(char *directory, char *previous);

void search(char *filename);

int main(int argc, char const *argv[])
{
	char current[PATH_MAX],previous[PATH_MAX];
    int check=0;
    /* change to given dir */
    if (argc>15){perror("argc>max_arg!!!"); return(1);}
    strcpy(current,argv[1]);
    if (chdir(current)){getcwd(current,PATH_MAX);check=1;}
    
    /* set previous directory */
	chdir("..");
	getcwd(previous,PATH_MAX);
	
    /* get keywords */
    arg=argc;
    if (check==0 && arg==2){fprintf(stderr,"No keyword in argv[]!!!\n"); return(1);}
    if (check == 1){ 
        key_list[0]=malloc(strlen(argv[1])+1);
        strcpy(key_list[0],argv[1]);
    }

    for (int i=2;i<argc;i++){
        key_list[i-1]=malloc(strlen(argv[i])+1);
        strcpy(key_list[i-1],argv[i]);
    }
    
    
	dir(current,previous); // Actual main
    
    for (int i=0;i<argc;i++){
        free(key_list[i]);
    }

	return(0);
}

void dir(char *directory, char *previous)
{

    /* cd directory */
	if(chdir(directory))
	{
		fprintf(stderr,"cd %s error\n",directory);
		exit(1);
	}

    /* open directory */
	folder = opendir(".");
	if(folder == NULL){
		fprintf(stderr,"Can't open directory %s\n",directory);
		exit(1);
	}

    getcwd(pathname,PATH_MAX);

    while (entry=readdir(folder)){
        stat(entry->d_name,&filestat);

        if(S_ISDIR(filestat.st_mode)){              
            /* skip special files */
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
				{continue;}
			/* get size of directory */
		    dir(entry->d_name,pathname);
		}
        else{                  
            search(entry->d_name);
        }
    }
    closedir(folder);
    chdir(previous);
}

void search(char *filename)
{
    int line_pos=1, check=0;
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
                    if (key_list[i]!=NULL){
                        if (strstr(line,key_list[i])!=NULL){
                            if (check==0){
                                printf("%s/%s\n",pathname,filename);
                                check=1;
                            }
                        printf("\t%s\tL%d\n",key_list[i],line_pos);
                        }  
                    }
                }   
            line_pos++;
        }  
    fclose(f);  
} 
