#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#pragma pack(1)
typedef struct
{
    char signature[2];
    int32_t file_size;
    int32_t reserved;
    int32_t data_offset;
    int32_t header_size;
    int32_t width;
    int32_t height;
    int16_t planes;
    int16_t bit_count;
    int32_t compression;
    int32_t img_size;
    int32_t x_pixels;
    int32_t y_pixels;
    int32_t colors_used;
    int32_t colors_important;
} BMPheader;
#pragma pack()

BMPheader bmp_header;

void processBMPFile(const char *filePath, int outputFile, BMPheader bmp_header)
{
    int fd_i = open(filePath, O_RDONLY);
    if (fd_i == -1)
    {
        perror("Eroare la deschiderea fisierului de intrare\n");
        exit(-1);
    }
    if (read(fd_i, &bmp_header, sizeof(BMPheader)) != sizeof(BMPheader))
    {
        perror("Eroare la citirea headerului bmp\n");
        exit(-1);
    }
    int width = bmp_header.width;
    int height = bmp_header.height;

    char *fileName = strrchr(filePath, '/');
    if (fileName == NULL)
    {
        fileName = (char *)filePath;
    }
    else
    {
        fileName++;
    }
    struct stat fileInfo;

    if (fstat(fd_i, &fileInfo) == -1)
    {
        perror("Eroare la obtinerea info bmp");
        close(fd_i);
        exit(-1);
    }
    char mod_time[20];
    strcpy(mod_time, (ctime(&fileInfo.st_mtime)));
    char stats[1024];
    int numChars = snprintf(stats, sizeof(stats),
                            "Nume fisier: %s\nInaltime: %d\nLatime: %d\nDimensiune: %lu\nIdentificatorul Utilizatorului: %d\ntimpul ultimei modificari: %s\ncontorul de legaturi: %lu\n",
                            fileName, height, width, (unsigned long)fileInfo.st_size, fileInfo.st_uid, mod_time, (unsigned long)fileInfo.st_nlink);

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces user: %c %c %c\n",
                         (fileInfo.st_mode & S_IRUSR) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWUSR) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXUSR) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces grup: %c %c %c\n",
                         (fileInfo.st_mode & S_IRGRP) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWGRP) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXGRP) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces altii: %c %c %c\n",
                         (fileInfo.st_mode & S_IROTH) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWOTH) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXOTH) ? 'X' : '-');

    write(outputFile, stats, numChars);
    close(fd_i);
    close(outputFile);
}

void processFile(const char *filePath, int outputFile)
{
    int fd_i = open(filePath, O_RDONLY);
    char *fileName = strrchr(filePath, '/');
    if (fileName == NULL)
    {
        fileName = (char *)filePath;
    }
    else
    {
        fileName++;
    }
    struct stat fileInfo;

    if (fstat(fd_i, &fileInfo) == -1)
    {
        perror("Eroare la obtinerea info file ");
        close(fd_i);
        exit(-1);
    }
    char mod_time[20];
    strcpy(mod_time, (ctime(&fileInfo.st_mtime)));
    char stats[1024];
    int numChars = snprintf(stats, sizeof(stats),
                            "Nume fisier: %s\nDimensiune: %lu\nIdentificatorul Utilizatorului: %d\ntimpul ultimei modificari: %s\n",
                            fileName, (unsigned long)fileInfo.st_size, fileInfo.st_uid, mod_time);

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces user: %c %c %c\n",
                         (fileInfo.st_mode & S_IRUSR) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWUSR) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXUSR) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces grup: %c %c %c\n",
                         (fileInfo.st_mode & S_IRGRP) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWGRP) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXGRP) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces altii: %c %c %c\n",
                         (fileInfo.st_mode & S_IROTH) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWOTH) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXOTH) ? 'X' : '-');

    write(outputFile, stats, numChars);
    close(fd_i);
    close(outputFile);
}

void regDir(const char *path, int outputFile)
{
    int fd_i = open(path, O_RDONLY);
    char *dirName = strrchr(path, '/');
    if (dirName == NULL)
    {
        dirName = (char *)path;
    }
    else
    {
        dirName++;
    }
    struct stat statbuf;
     if (fstat(fd_i, &statbuf) == -1)
    {
        perror("Eroare la obtinerea info dir");
        close(fd_i);
        exit(-1);
    }

    char stats[1024];
    int numChars = snprintf(stats, sizeof(stats),
                            "Nume director: %s\n Identificatorul Utilizatorului: %d\n",
                            dirName, statbuf.st_uid);

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces user: %c %c %c\n",
                         (statbuf.st_mode & S_IRUSR) ? 'R' : '-',
                         (statbuf.st_mode & S_IWUSR) ? 'W' : '-',
                         (statbuf.st_mode & S_IXUSR) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces grup: %c %c %c\n",
                         (statbuf.st_mode & S_IRGRP) ? 'R' : '-',
                         (statbuf.st_mode & S_IWGRP) ? 'W' : '-',
                         (statbuf.st_mode & S_IXGRP) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces altii: %c %c %c\n",
                         (statbuf.st_mode & S_IROTH) ? 'R' : '-',
                         (statbuf.st_mode & S_IWOTH) ? 'W' : '-',
                         (statbuf.st_mode & S_IXOTH) ? 'X' : '-');

    write(outputFile, stats, numChars);
    close(outputFile);
}

void processLink(const char *filePath, int outputFile)
{
     int fd_i = open(filePath, O_RDONLY);
    char *linkName = strrchr(filePath, '/');
    if (linkName == NULL)
    {
        linkName = (char *)filePath;
    }
    else
    {
        linkName++;
    }
     struct stat fileInfo;

    if (fstat(fd_i, &fileInfo) == -1)
    {
        perror("Eroare la obtinerea info link");
        close(fd_i);
        exit(-1);
    }
    char stats[1024];
    int numChars = snprintf(stats, sizeof(stats),
                            "Nume legatura: %s\nDimensiune: %lu\n",
                            linkName, (unsigned long)fileInfo.st_size);

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces user: %c %c %c\n",
                         (fileInfo.st_mode & S_IRUSR) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWUSR) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXUSR) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces grup: %c %c %c\n",
                         (fileInfo.st_mode & S_IRGRP) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWGRP) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXGRP) ? 'X' : '-');

    numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
                         "drepturi de acces altii: %c %c %c\n",
                         (fileInfo.st_mode & S_IROTH) ? 'R' : '-',
                         (fileInfo.st_mode & S_IWOTH) ? 'W' : '-',
                         (fileInfo.st_mode & S_IXOTH) ? 'X' : '-');

    write(outputFile, stats, numChars);
    close(fd_i);
    close(outputFile);
}

void processDirectory(const char *dirPath, int outputFile)
{
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        perror("eroare la deschiderea directorului\n");
        exit(-1);
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

        struct stat statbuf;
        if (stat(filePath, &statbuf) == -1)
        {
            perror("Eroarea la obtinerea info process \n");
            continue;
        }

        if (S_ISREG(statbuf.st_mode))
        {
	  const char *ext = strrchr(entry->d_name, '.');
	   if (ext != NULL && strcmp(ext, ".bmp") == 0)
	  {
                processBMPFile(filePath, outputFile, bmp_header);
	 }
	 else
		 processFile(filePath, outputFile);
        }
        else if (S_ISDIR(statbuf.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            regDir(filePath, outputFile);
        }
        else if (S_ISLNK(statbuf.st_mode))
        {
            processLink(filePath, outputFile);
        }
        
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("nr incorect de argumente.\n");
        exit(-1);
    }

    char *inputPath = argv[1];

    int outputFile = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (outputFile == -1)
    {
        perror("Eroare la deschiderea fisierului de intrare\n");
        return 1;
    }

    processDirectory(inputPath, outputFile);

    return 0;
}