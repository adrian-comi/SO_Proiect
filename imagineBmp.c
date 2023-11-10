#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("nr incorect de argumente.\n");
        exit(-1);
    }

    char *fisier_intrare = argv[1];

    int fd_i = open(fisier_intrare, O_RDONLY);
    if (fd_i == -1)
    {
        perror("Eroare la deschiderea fisierului de intrare\n");
        return 1;
    }

    const char *ext = strrchr(fisier_intrare, '.');
    if (ext == NULL || strcmp(ext, ".bmp") != 0)
    {
        perror("Fisierul nu este bmp\n");
        exit(-1);
    }

    BMPheader bmp_header;
    if (read(fd_i, &bmp_header, sizeof(BMPheader)) != sizeof(BMPheader))
    {
        perror("Eroare la citirea headerului bmp\n");
    }

    int width = bmp_header.width;
    int height = bmp_header.height;

    char *fileName = strrchr(fisier_intrare, '/');
    if (fileName == NULL)
    {
        fileName = (char *)fisier_intrare;
    }
    else
    {
        fileName++;
    }

    struct stat file_info;
    if (fstat(fd_i, &file_info) == -1)
    {
        perror("Eroare la obtinerea info");
        close(fd_i);
        return 1;
    }


    char mod_time[20];
    strcpy(mod_time, (ctime(&file_info.st_mtime)));

    int outputFile = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (outputFile == -1)
    {
        perror("Eroare la deschiderea fisierului de intrare\n");
        close(fd_i);
        return 1;
    }

    char stats[1024];
int numChars = snprintf(stats, sizeof(stats),
    "Nume fisier: %s\nInaltime: %d\nLatime: %d\nDimensiune: %lu\nIdentificatorul Utilizatorului: %d\ntimpul ultimei modificari: %s\ncontorul de legaturi: %lu\n",
    fileName, height, width, (unsigned long)file_info.st_size, file_info.st_uid, mod_time, (unsigned long)file_info.st_nlink);

numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
    "drepturi de acces user: %c %c %c\n",
    (file_info.st_mode & S_IRUSR) ? 'R' : '-',
    (file_info.st_mode & S_IWUSR) ? 'W' : '-',
    (file_info.st_mode & S_IXUSR) ? 'X' : '-');

numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
    "drepturi de acces grup: %c %c %c\n",
    (file_info.st_mode & S_IRGRP) ? 'R' : '-',
    (file_info.st_mode & S_IWGRP) ? 'W' : '-',
    (file_info.st_mode & S_IXGRP) ? 'X' : '-');

numChars += snprintf(stats + numChars, sizeof(stats) - numChars,
    "drepturi de acces altii: %c %c %c\n",
    (file_info.st_mode & S_IROTH) ? 'R' : '-',
    (file_info.st_mode & S_IWOTH) ? 'W' : '-',
    (file_info.st_mode & S_IXOTH) ? 'X' : '-');

write(outputFile, stats, numChars);
    close(fd_i);
    close(outputFile);

    return 0;
}