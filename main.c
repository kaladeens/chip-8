# include <stdio.h>
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    char *fileName = argv[1];
    FILE *file = fopen(fileName, "r");
    return 0;
}

