#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>


pthread_t tid[500]; //max 500 thread
typedef struct arg_struct {
    char asal[1000];
    char cwd[1000];
}arg_struct;

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void pindahFile(char *argv, char *cwd){
  
  char stringfile[1000];
  strcpy(stringfile, argv);
  int isFile = is_regular_file(stringfile);
  char* TipeFolder = strchr(stringfile, '.');
  char* nama = strrchr(stringfile, '/');
//   printf("TES DOLO : %s\n", stringfile);
//   printf("TES DOLO : %s\n", TipeFolder);

  char TipeFolderLCase[100];
  if(TipeFolder!=0)
  {
    if((TipeFolder[strlen(TipeFolder)-1] >= 'a' && TipeFolder[strlen(TipeFolder)-1] <= 'z') || (TipeFolder[strlen(TipeFolder)-1] >= 'A' && TipeFolder[strlen(TipeFolder)-1] <= 'Z') || (TipeFolder[strlen(TipeFolder)-1] >= '0' && TipeFolder[strlen(TipeFolder)-1] <= '9'))
    {
      strcpy(TipeFolderLCase, TipeFolder);
      for(int i = 0; TipeFolderLCase[i]; i++)
        TipeFolderLCase[i] = tolower(TipeFolderLCase[i]);
    }
  }
  else
  {
    if(isFile==0) // Jika File bentuk Folder
    {
      printf("Sad, gagal :(\n");
      return;
    }
    else // Jika File tidak berbentuk
    {
      strcpy(TipeFolderLCase, " Unknown");
    }
  }

    mkdir((TipeFolderLCase + 1), 0777); //bikin folder ekstensi

    size_t len = 0 ;
    // Mindahin
    char fileawal[1000] ; 
    strcpy(fileawal, argv);
    char filepindah[1000] ; 
    strcpy(filepindah, cwd);
    strcat(filepindah, "/");
    strcat(filepindah, TipeFolderLCase+1);
    strcat(filepindah, nama);
    // printf("fileawal = %s\n", fileawal);

    // printf("filepindah = %s\n", filepindah);
    rename(fileawal, filepindah);
    remove(fileawal);
    // char notifsukses[1000]
    // strcpy(notifsukses, argv);
    printf("%s : Berhasil Dikategorikan\n", nama);
}

void *pindahinf(void* arg){
  arg_struct args = *(arg_struct*) arg;
  pindahFile(args.asal, args.cwd);
  pthread_exit(0);
}

void commandf(int argc, char* argv[], arg_struct args)
{
//  printf("F hehehehe");
            int index = 0;
        for (int i = 2; i < argc; i++)
        {
            strcpy(args.asal, argv[i]);
            pthread_create(&tid[index], NULL, pindahinf, (void *)&args);
           pthread_join(tid[index], NULL);
            index++;  
        }
        // for (int i = 0; i < index; i++) {
        //     pthread_join(tid[i], NULL);
        // }
}

void commandstar(char *asal,int argc, char* argv[],arg_struct args)
{
    printf("%s" , asal);
    DIR *dirp;
        struct dirent *entry;
        dirp = opendir(asal);
        char namafilecomplete[1000];
        char namaprogram[] = "/home/bill/Modul3/soal3.c";
        char namacompiled[] = "/home/bill/Modul3/soal3";

        int index=0;
        while((entry = readdir(dirp)) != NULL)
        {
            if(entry->d_type == DT_REG)
            {
                char namafile[1000];
                sprintf(namafile, "%s/%s", asal, entry->d_name);
                printf("\n");
               
                strcpy(namafilecomplete, asal);
                strcpy(namafilecomplete, namafile);
        
                strcpy(args.asal, namafilecomplete);
                // strcpy (args.asal=argv[i]) ;
                // printf("namafilecomplete = %s\n", namafilecomplete);
                // printf("namaprogram = %s\n", namaprogram);
                
                if(strcmp(namafilecomplete, namaprogram) != 0 && strcmp(namafilecomplete, namacompiled) != 0)
                {
                     pthread_create(&tid[index], NULL, pindahinf, (void *)&args);
                      pthread_join(tid[index],NULL);
                    // printf("%s\n", namafilecomplete);
                    // sleep(1);
                     index++;    
                }
            }
        }
}

void commanddelta(char *asal,int argc, char* argv[],arg_struct args)
{
    printf("%s" , asal);
    char asalD[100];
    strcpy(asalD, argv[2]);

    DIR *dirp;
        struct dirent *entry;
        dirp = opendir(argv[2]);
        char namafilecomplete[1000];
        char namaprogram[] = "/home/bill/Modul3/soal3.c";
        char namacompiled[] = "/home/bill/Modul3/soal3";

        int index=0;
        while((entry = readdir(dirp)) != NULL)
        {
            if(entry->d_type == DT_REG)
            {
                char namafile[1000];
                sprintf(namafile, "%s/%s", argv[2], entry->d_name);
                printf("\n");
               
                strcpy(namafilecomplete, argv[2]);
                strcpy(namafilecomplete, namafile);
        
                strcpy(args.asal, namafilecomplete);
                printf("Asal = %s\n", asal);
                printf("ARGS ASAL = %s \n", args.asal);
                printf("ARGV2 = %s \n", argv[2]);
                // strcpy (args.asal=argv[i]) ;
                printf("namafilecomplete = %s\n", namafilecomplete);
                printf("namaprogram = %s\n", namaprogram);
                
                if(strcmp(namafilecomplete, namaprogram) != 0 && strcmp(namafilecomplete, namacompiled) != 0)
                {
                     pthread_create(&tid[index], NULL, pindahinf, (void *)&args);
                     pthread_join(tid[index],NULL);
                    // printf("%s\n", namafilecomplete);
                    // sleep(1);
                     index++;    
                }
            }
        }
}
int main(int argc, char* argv[])
{

    arg_struct args;
  getcwd(args.cwd, sizeof(args.cwd));

     if(strcmp(argv[1],"-f")==0)
     {
        // printf("F hehehehe");
        commandf(argc, argv, args);
     }
     else if(strcmp(argv[1],"-d")==0)
     {
        //  printf("D hehehehe");
        commanddelta(args.cwd, argc, argv, args);
     }
     else if(strcmp(argv[1],"*")==0)
     {
        // printf("* hehehehe\n");
        commandstar(args.cwd, argc, argv, args);
     }
    
}