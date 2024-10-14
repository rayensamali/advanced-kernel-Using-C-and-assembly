#include "file_system.h"
#include "disk_io.h"
#include "stringlib.h"
#include "kernel.h"

// Struct to represent file metadata in the file table.
typedef struct {
    char filename[FILENAME_MAX_LEN];
    int sector;
} FileEntry;

FileEntry file_table[MAX_FILES];  // Global file table

/* Function declarations */
int find_empty_file_slot(void);
int find_file_by_name(const char *filename);
void list_file_table(void);  // Helper function to print the file table for debugging

/*
 * Initialize the file system (loads file table from disk)
 */
void initialize_file_system(void) {
    print_string("Loading file table from disk...\n");
    
    int result = ata_read_sector(FILE_TABLE_SECTOR, (unsigned char*)file_table);
    if (result != 0) {
        print_string("Error: Failed to read the file table from disk.\n");
        return;
    }

    // Print the loaded file table for debugging
    print_string("Current file table after loading:\n");
    list_file_table();
}


/*
 * Save file to disk by writing the content to a free sector
 */
void save_file_to_disk(const char *filename, const char **content, const int n) {
    print_string("Starting file save operation...\n");

    // Find an empty slot in the file table
    int file_slot = find_empty_file_slot();
    if (file_slot == -1) {
        print_string("Error: File table is full\n");
        return;
    }

    // Check if the file already exists
    int file_exists = find_file_by_name(filename);
    if (file_exists != -1) {
        print_string("Error: File already exists\n");
        return;
    }

    // Assign a free sector for the file
    unsigned int sector = file_slot + START_DATA_SECTOR;  // Adjust this if sectors need to be calculated differently
    file_table[file_slot].sector = sector;
    strncpy(file_table[file_slot].filename, filename, sizeof(file_table[file_slot].filename) - 1);
    file_table[file_slot].filename[sizeof(file_table[file_slot].filename) - 1] = '\0';  // Ensure null-termination

    print_string("Assigned file slot: ");
    print_int(file_slot);
    print_string(", sector: ");
    print_int(sector);
    kprint_newline();

    // Ensure the entire content is copied without truncation
    unsigned char buffer[SECTOR_SIZE] = {0};  // Initialize buffer to zero
    size_t total_len = 0;     // Calculate the length of the content

    // Calculate total length of content
    for (size_t i = 0; i < n; i++) {
        total_len += strlen(content[i]) + 1;  // +1 for the space or null terminator
    }

    if (total_len < SECTOR_SIZE) {
        // Copy content into the buffer
        char *c = (char *)buffer; // Start at the beginning of the buffer
        for (size_t i = 0; i < n; i++) {
            size_t len = strlen(content[i]);
            strncpy(c, content[i], len);
            c += len; // Move the pointer forward by the length of the string
            *c++ = ' '; // Add a space between entries
        }
        *(c - 1) = '\0'; // Null terminate the last entry
    } else {
        print_string("Error: Content too large for sector\n");
        return;
    }

    print_string("Writing content to disk at sector: ");
    print_int(sector);
    kprint_newline();

    int write_result = ata_write_sector(sector, buffer);
    if (write_result != 0) {
        print_string("Error: Failed to write data to disk.\n");
        return;
    }

    // Write the updated file table back to the disk
    print_string("Writing updated file table to disk...\n");
    int result = ata_write_sector(FILE_TABLE_SECTOR, (unsigned char *)file_table);

    if (result == 0) {
        print_string("File saved successfully to sector ");
        print_int(sector);
        kprint_newline();
    } else {
        print_string("Error: Failed to write file table to disk\n");
    }

    list_file_table();  // Print the file table after saving for debugging
}


/*
 * List all files in the file table
 */
void list_files(void) {
    print_string("Files on disk:\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].filename[0] != '\0') {
            print_string("File ");
            print_int(i);  // Print file index
            print_string(": ");
            print_string(file_table[i].filename);  // Print file name
            kprint_newline();
        }
    }
}


/*
 * Helper function to print the entire file table for debugging purposes
 */
void list_file_table() {
    print_string("Current file table:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].filename[0] != '\0') {
            print_string("File ");
            print_int(i);
            print_string(": ");
            print_string(file_table[i].filename);
            print_string(", Sector: ");
            print_int(file_table[i].sector);
            kprint_newline();
        }
    }
}


/*
 * Find the first empty slot in the file table
 */
int find_empty_file_slot(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].filename[0] == '\0') {
            return i;
        }
    }
    return -1;
}

/*
 * Find a file by its name in the file table
 */
int find_file_by_name(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            print_string("File found: ");
            print_string(filename);
            print_string(" in slot: ");
            print_int(i);
            kprint_newline();
            return i;
        }
    }

    print_string("File not found: ");
    print_string(filename);
    kprint_newline();
    return -1;
}

/*
 * Load file content from disk by filename
 */
int load_file_from_disk(const char *filename, char *buffer) {
    int file_slot = find_file_by_name(filename);
    if (file_slot == -1) {
        print_string("Error: File not found\n");
        return -1; // File not found
    }

    unsigned int sector = file_table[file_slot].sector;

    print_string("Loading file from sector: ");
    print_int(sector);
    kprint_newline();

    ata_read_sector(sector, (unsigned char *)buffer);
    
    print_string("File content: ");
    print_string(buffer);  // Ensure full content is printed
    kprint_newline();

    return 0; // Success
}

/*
 * Delete a file by clearing its entry in the file table
 */
void delete_file(const char *filename) {
    int file_slot = find_file_by_name(filename);
    if (file_slot == -1) {
        print_string("Error: File not found\n");
        return;
    }

    // Clear the file entry
    file_table[file_slot].filename[0] = '\0';
    file_table[file_slot].sector = 0;

    // Write the updated file table to disk
    ata_write_sector(FILE_TABLE_SECTOR, (unsigned char *)file_table);

    print_string("File deleted successfully\n");

    list_file_table();  // Print the file table after deletion for debugging
}

