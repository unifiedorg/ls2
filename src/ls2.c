/*
    ls2 - An advanced version of the ls command that displays more information in a table-like manner, with colors!
    Copyright (C) 2023  Unified

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define COLOR_RESET "\x1B[0m"
#define COLOR_FILE "\e[0;36m"
#define COLOR_DIR "\e[0;34m"
#define COLOR_YELLOW "\x1B[33m"

int show_permissions = 0; // Variable to control numerical permissions display

// Function to print file information
void print_file_info(const struct dirent *entry, const char *dir_name, int max_name_width) {
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, entry->d_name);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    const char *color = S_ISDIR(file_stat.st_mode) ? COLOR_DIR : COLOR_FILE;

    char file_name[PATH_MAX];
    // Add "./" prefix to directories
    if (S_ISDIR(file_stat.st_mode)) {
        snprintf(file_name, sizeof(file_name), "./%s", entry->d_name);
    } else {
        snprintf(file_name, sizeof(file_name), "%s", entry->d_name);
    }

    char permissions[11];
    if (show_permissions) {
        // Display numerical permissions
        snprintf(permissions, sizeof(permissions), "%o", (unsigned int)(file_stat.st_mode & 0777));
    } else {
        // Display permissions as a string (e.g., "rwxr-xr-x")
        permissions[0] = S_ISDIR(file_stat.st_mode) ? 'd' : '-';
        permissions[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
        permissions[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
        permissions[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
        permissions[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
        permissions[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
        permissions[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
        permissions[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
        permissions[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
        permissions[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
        permissions[10] = '\0';
    }

    // Print file information with formatting
    printf("%s%-*s%s  %-*lld  %-11s %-19s %s\n",
           color, max_name_width, file_name, COLOR_RESET,
           12, (long long)file_stat.st_size, // Increased spacing for file size
           permissions,
           COLOR_YELLOW, ctime(&file_stat.st_mtime), COLOR_RESET);
}

// Main function
int main(int argc, char *argv[]) {
    int show_hidden = 0;
    int max_name_width = 0;

    // Parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "anhv")) != -1) {
        switch (opt) {
            case 'a':
                show_hidden = 1; // Show hidden files
                break;
            case 'n':
                show_permissions = 1; // Display permissions in numerical form
                break;
            case 'v':
                printf("Running ls2 v1.0 by unium @ unified\n"); // Display version information
                exit(EXIT_SUCCESS);
            case 'h':
                // Display help menu
                printf("\n------------------------------------------------------------------------------------\n");
                printf("ls2 v1.0 by unium @ unified\n");
                printf("Description : A replacement for the command ls, with nicer formatting and colors.\n");
                printf("Usage: ls2 [options]\n");
                printf("Options:\n");
                printf("-h                Displays this menu.\n");
                printf("-v                Displays the version of this command.\n");
                printf("-a                Displays hidden files.\n");
                printf("-n                Displays permissions in a numerical form.\n");
                printf("------------------------------------------------------------------------------------\n\n");
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s [-h] [-v] [-a] [-n]\n", argv[0]); // Invalid option
                exit(EXIT_FAILURE);
        }
    }

    // Open the current directory
    const char *dir_name = ".";
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read and store directory entries
    struct dirent **entries = NULL;
    int num_entries = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        num_entries++;

        int name_width = strlen(entry->d_name);
        if (name_width > max_name_width) {
            max_name_width = name_width + 2;
        }
    }
    rewinddir(dir);

    entries = (struct dirent **)malloc(num_entries * sizeof(struct dirent *));
    if (entries == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        entries[i++] = entry;
    }

    // Sort the entries alphabetically
    qsort(entries, num_entries, sizeof(struct dirent *), (int (*)(const void *, const void *))strcasecmp);

    // Print table header
    printf("%-*s %-10s     %-11s                   %-19s\n", max_name_width, "Name", "Size (b)", "Permissions", "Last Modified");
    printf("--------------------------------------------------------------------------------------\n");

    for (i = 0; i < num_entries; i++) {
        print_file_info(entries[i], dir_name, max_name_width); // Print file information for each entry
    }

    closedir(dir);
    free(entries);

    return EXIT_SUCCESS;
}
