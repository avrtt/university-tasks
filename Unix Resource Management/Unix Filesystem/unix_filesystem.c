#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    DIR *d;
    char cwd[512];
    char input[256];
    struct dirent *dir; 

	if (argv[1] == NULL)
    {
		printf("Error: Directory not specified!\n");
		exit(0);
	}

    d = opendir(argv[1]);
    
    if (!d)
	{
		printf("Error: No such directory!\n");
		exit(0);
	}
    
    printf("\n   Start directory (/%s)\n", argv[1]);

    while ((dir = readdir(d)) != NULL)
        if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
            printf("        %s\n", dir->d_name);
    
    do 
    {
        d = opendir(".");

        if (getcwd(cwd, sizeof(cwd)) != NULL)
            fprintf(stdout, "\n   %s\n", cwd);

        if (d)
        {
            while ((dir = readdir(d)) != NULL)
                if (dir->d_type == DT_DIR && dir->d_name[0] != '.')
                    printf("        %s\n", dir->d_name);
        
        chdir("..");
        closedir(d);
        }

    } while ((int)cwd[1] != 0);

  return 0;

}

