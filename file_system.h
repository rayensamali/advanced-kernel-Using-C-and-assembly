#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#define MAX_FILES 16
#define SECTOR_SIZE 512
#define FILE_TABLE_SECTOR 1
#define START_DATA_SECTOR 2
#define FILENAME_MAX_LEN 16

void initialize_file_system(void);
void save_file_to_disk(const char *filename, const char **content, const int n);
int load_file_from_disk(const char *filename, char *buffer);
void delete_file(const char *filename);
void list_files(void);

#endif

