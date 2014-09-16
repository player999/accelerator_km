#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#define REGISTER_SPAN 0x04000000
#define ADDRESS_STM 0xFC000000
#define OFFSET_LWFPGASLAVES 0x03200000
#define ACCEL_OFFSET 0x00040000

int main(int argc, char *argv[]) {
 
        void *virtual_base;
        void *h2p_lw_accel_addr;
        int fd;
        unsigned int a1, a2;

        if (argc != 3) {
        	printf("Wring arguments");
        	return -1;
        }

        a1 = atoi(argv[1]);
        a2 = atoi(argv[2]);

        if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
                printf( "ERROR: could not open \"/dev/mem\"...\n" );
                return( 1 );
        }
 
        virtual_base = mmap( NULL, REGISTER_SPAN, ( PROT_READ | PROT_WRITE ), 
        	MAP_SHARED, fd, ADDRESS_STM);
 
        if( virtual_base == MAP_FAILED ) {
                printf( "ERROR: mmap() failed...\n" );
                close( fd );
                return( 1 );
        }
       
        h2p_lw_accel_addr = virtual_base + (unsigned int)(OFFSET_LWFPGASLAVES + 
        	ACCEL_OFFSET);

        printf("Writing 1st argument\n");
        *(unsigned int *)(h2p_lw_accel_addr + 0) = a1;
        printf("Writing 2nd argument\n");
        *(unsigned int *)(h2p_lw_accel_addr + 4) = a2;
        sleep(1);
        printf("Reading result\n");
        printf("Res: %d\n", *(unsigned int *)(h2p_lw_accel_addr + 0));
		
        if( munmap( virtual_base, REGISTER_SPAN) != 0 ) {
                printf( "ERROR: munmap() failed...\n" );
                close( fd );
                return( 1 );
        }
 
        close( fd );
 
        return( 0 );
}