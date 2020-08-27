#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main (int argc, char *argv[]) {
    FILE *fpt;
    unsigned char *img, *smooth;
    char header[320];
    int rows,cols,bytes;
    int a,r,c,r2,c2,sum,temp;
    long int totaltime,totaltemp,avgtime;
    struct timespec	start,end;


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

    totaltemp = 0;
    totaltime = 0;
    printf("\n");

    //Loop to run algorithm 10 times
    for (a=0; a<10; a++) {

        printf("Run #%d:\n",(a+1));

        //Start the timer
        clock_gettime(CLOCK_REALTIME,&start);
        printf("Start time: %ld sec %ld ns\n",(long int)start.tv_sec,start.tv_nsec);

        //Convolution algorithm
	    for (r=3; r<(rows-3); r++) {
            for (c=3; c<(cols-3); c++) {
                sum=0;
                temp=0;
                for (r2=-3; r2<=3; r2++) {
                    for (c2=-3; c2<=3; c2++) {
                        temp=img[(r+r2)*cols+(c+c2)];
                        sum=sum+temp;
                    }
                smooth[r*cols+c]=sum/49;
                }
            }
        }   

	    //End the timer
        clock_gettime(CLOCK_REALTIME,&end);
        printf("End time: %ld sec %ld ns\n",(long int)end.tv_sec,end.tv_nsec);

	    //Print the run time
        if ((end.tv_nsec-start.tv_nsec)<0) {
            totaltemp=(long int)(1000000000+end.tv_nsec-start.tv_nsec);
        }   
        else {
            totaltemp=(long int)(end.tv_nsec-start.tv_nsec);
        }
        printf("Run time: %ld ns\n\n",totaltemp);
        totaltime=totaltime+totaltemp;
    }

    //Average time calculated
    avgtime = 0;
    avgtime=(totaltime/10);
    printf("Average time in ns: %ld ns\n", avgtime);
    printf("Average time in seconds: %lf sec\n\n",(avgtime * 0.000000001));
    printf("\n\n====================================================================\n\n");

	//Write out the image to new file
    fpt=fopen("smoothed_2dconv.ppm","w");
    fprintf(fpt,"P5 %d %d 255\n",cols,rows);
    fwrite(smooth,cols*rows,1,fpt);
    fclose(fpt);
    free(img);
    free(smooth);
}