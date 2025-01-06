#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


char*
getname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  strcpy(buf, p);
  return buf;
}

void find(char *path, char *target_word) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // printf("Trying to do shit to: %s\n", path);

    if(stat(path, &st) < 0){
        return;
    }
    switch(st.type){
        case T_DEVICE:
        case T_FILE:
            char* str = getname(path);
            // printf("I am a file: %s\n", str);
            // If matches the target
            if (strcmp(str, target_word) == 0) {
                // printf("Found file: %s\n", str);
                printf("%s\n", path);
            }
            // Also lowkey a base case
            return;
        case T_DIR:
            if((fd = open(path, O_RDONLY)) < 0){
                printf("Failed to open path: %s\n", path);
                return;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                // printf("Current buf before memmove: %s\n", buf);  
                // printf("Current de.name: %s\n", de.name);         

                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                // printf("Constructed path: %s\n", buf); 
            
                find(buf, target_word);
            }
            close(fd);
            break;
    }
    
}

int main(int argc, char *argv[])
{
    if (argc != 3) { 
        printf("Usage: find <path> <filename>\n"); 
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);

}
