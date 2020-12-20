#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main (int argc, char *argv[]) {
    FILE *fpt;
    unsigned char *img, *smooth;
    int *smoothtemp;
    char header[320];
    int rows,cols,bytes;
    int r,c,r2,c2,sum,temp;

	//Read the image from the file
    if ((fpt=fopen(argv[1],"rb"))==NULL) {
        printf("Unable to open image for reading\n");
        exit(0);
    }

    //Read the header of the image
    fscanf(fpt,"%s %d %d %d",header,&cols,&rows,&bytes);

    //Check for greyscale 8-bit ppm
    if (strcmp(header,"P5")!= 0 || bytes!=255) {
        printf("Not a greyscale 8-bit PPM image\n");
        exit(0);
    }

    //Allocate space for image data
    img=(unsigned char *)calloc((rows*cols),sizeof(unsigned char));
    header[0]=fgetc(fpt);
    fread(img,1,cols*rows,fpt);
    fclose(fpt);

	//Memory allocation for new image
    smooth=(unsigned char *)calloc((rows*cols),sizeof(unsigned char));
    smoothtemp=(int *)calloc((rows*cols),sizeof(int));

    //Column
	for (r=0; r<rows; r++) {
        for (c=3; c<(cols-3); c++) {
            sum=0;
            temp=0;
            for (c2=-3; c2<=3; c2++) {
                temp=img[(r*cols)+c+c2];
                sum=sum+temp;
            }
            smoothtemp[(r*rows)+c]=sum;
        }
    }  

    //Row
    for (r=3; r<(rows-3); r++) {
        for (c=3; c<(cols-3); c++) {
            sum=0;
            temp=0;
            for (r2=-3; r2<=3; r2++) {
                temp=smoothtemp[((r+r2)*cols)+c];
                sum=sum+temp;
            }
            smooth[(r*rows)+c]=sum/49;
        }
    } 

	//Write out the image to new file
    fpt=fopen("smoothed_sepfilters.ppm","w");
    fprintf(fpt,"P5 %d %d 255\n",cols,rows);
    fwrite(smooth,cols*rows,1,fpt);
    fclose(fpt);

    free(img);
    free(smooth);
    free(smoothtemp);
}