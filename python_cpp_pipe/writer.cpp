#include <stdio.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define _PATH_ "/tmp/vsd_fifo.pipe"
#define _SIZE_ 100


void Write(){
	int ret = mkfifo(_PATH_, 0666 | S_IFIFO);
	if (ret == -1){
		printf("Failed to create pipe\n");
		//return;
	}

	int fd = open(_PATH_, O_WRONLY);
	if(fd <0){
		printf("Failed to open\n");
		return;
	}

	char buf[_SIZE_];

	while(1){
		printf("Please input: \n");
		scanf("%s", buf);

		int ret = write(fd, buf, strlen(buf) + 1);
		if (ret < 0){
			printf("Write error");
			break;
		}

	}
	close(fd);
}

int main(){
	Write();
	return 1;
}

