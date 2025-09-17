#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void doLs(const char * dirName, int doLong);
void showStatInfo(const char *fname, struct stat *buf);
int isLong(int argc, char *argv[]);
void getMode(mode_t mode, char* s);

int main(int argc, char *argv[])
{
    int doLong = 0;

    if(isLong(argc, argv))
    {
        doLong = 1;
    }

    if (argc == 1 || (argc == 2 && doLong))
    {
        doLs(".", doLong);
    }
    else
    {
        while(--argc)
        {
            if((strncmp(*++argv, "-l", 3)))
            {
                printf("%s:\n", *argv);
                doLs(*argv, doLong);
            }
        }
    }
}

/**
 * @brief List file in directory 
 * 
 * @param dirName name of directory
 */
void doLs(const char * dirName, int doLong)
{
    DIR *pDir;
    struct dirent *pEntry;
    
    if ((pDir = opendir(dirName))==NULL)
    {
        perror("Opening directory");
    }
    else
    {
        while ((pEntry = readdir(pDir))!=NULL)
        {
            if(pEntry->d_name[0] != '.')
            {
                if(doLong)
                {
                    struct stat info;
                    printf("%s\n", pEntry->d_name);
                    showStatInfo(pEntry->d_name, &info);
                }
                else
                {
                    printf("%s\n", pEntry->d_name);
                }
            }    
        }
    }
}

/**
 * @brief show info for -l flag
 * 
 * @param fname file name
 * @param buf   information buffer
 */
void showStatInfo(const char *fname, struct stat *buf)
{
    printf("   mode: %o\n", buf->st_mode);
    char mode_str[11] = "----------";
    getMode(buf->st_mode, mode_str);
    printf("   mode: %s\n", mode_str); //"rwx__x__x"
    printf("   links: %ld\n", buf->st_nlink);
    printf("   user: %s\n", getpwuid(buf->st_uid)->pw_name);
    //getpwuid(buf->st_uid)->pw_name;
    printf("   group: %s\n", getgrgid(buf->st_gid)->gr_name);
    printf("   size: %ld\n", buf->st_size);
    printf("   modtime: %.12s\n", 4+ctime(&buf->st_mtime));
    printf("   name: %s\n", fname);
}

/**
 * @brief check for -l flag
 * 
 * @param argc  arg count
 * @param argv  arg array
 * @return int  1 if -l is found, 0 otherwise
 */
int isLong(int argc, char *argv[])
{
    for(int i = 0; i < argc; i++)
    {
        if(!(strncmp(argv[i], "-l", 3)))
        {
            return 1; 
        }
    }
    return 0;
}

void getMode(mode_t mode, char* s)
{
    switch(mode & __S_IFMT) 
    {
        case __S_IFBLK:  s[0] = 'b';        break;
        case __S_IFCHR:  s[0] = 'c';        break;
        case __S_IFDIR:  s[0] = 'd';        break;
        case __S_IFIFO:  s[0] = 'p';        break;
        case __S_IFLNK:  s[0] = 'l';        break;
        // case __S_IFREG:                     break;
        case __S_IFSOCK: s[0] = 's';        break;
        //default:       printf("unknown?\n");                break;
    }

    if(mode & S_IRUSR) s[1] = 'r';
    if(mode & S_IWUSR) s[2] = 'w';
    if(mode & S_IXUSR) s[3] = 'x';
    if(mode & S_IRGRP) s[4] = 'r';
    if(mode & S_IWGRP) s[5] = 'w';
    if(mode & S_IXGRP) s[6] = 'x';
    if(mode & S_IROTH) s[7] = 'r';
    if(mode & S_IWOTH) s[8] = 'w';
    if(mode & S_IXOTH) s[9] = 'x';
}
