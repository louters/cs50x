#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Check for jpeg signature
bool is_jpeg(unsigned char buffer[]);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Remember filename
    char *filename = argv[1];

    // Open input file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s\n", filename);
        return 1;
    }

    // JPEG counter
    int cnt = 0;

    // JPEG file name
    char *jpeg_name = malloc(sizeof("###.jpg"));

    // Buffer of 512B. Allocate memory of buffer.
    unsigned char buffer [512];

    // Initialize outfile
    FILE *outfile = NULL;

    // Read input file until end
    while (fread(buffer, 512, 1, file))
    {
        // Test whether start of a jpeg
        if (is_jpeg(buffer))
        {
            // Close previous file if any
            if (outfile)
            {
                fclose(outfile);
                outfile = NULL;
            }

            // Create JPEG output filename
            sprintf(jpeg_name, "%03i.jpg", cnt);
            // Open JPEG output file
            outfile = fopen(jpeg_name, "w");

            // Update counter
            cnt++;
        }

        // if file opened, write
        if (outfile)
        {
            // Write JPEG
            fwrite(buffer, 512, 1, outfile);
        }
    }

    // Close input file
    fclose(file);

    // Free memory
    free(jpeg_name);

    return 0;
}

// Check for JPEG signature
bool is_jpeg(unsigned char buffer[])
{
    if
    (
        buffer[0] == 0xff
        && buffer[1] == 0xd8
        && buffer[2] == 0xff
        && (buffer [4] >> 4) ^ 0b1110
    )
    {
        return true;
    }
    return false;
}