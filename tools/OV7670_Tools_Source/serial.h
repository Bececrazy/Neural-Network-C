#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

class SERIAL{
	public:
	
	int USB;
	char* buff;
	int baud_rate;
	struct termios tty;
	struct termios tty_old;
	
	void begin( int r_baud ){
		USB = -1;
		baud_rate = r_baud;
		buff = new char[1];
		memset ( &tty, 0, sizeof tty );
		memset ( buff,0, sizeof(char) );}
	
	bool Connect(){
		USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
		
		// Error Handling
		if ( tcgetattr ( USB, &tty ) != 0 ) {
			printf("Error: %d from tcgetattr: %s\n", errno, strerror(errno));
			return false;}
			 
		/* Save old tty parameters */
		tty_old = tty;

		/* Set Baud Rate */
		cfsetospeed (&tty, (speed_t)baud_rate);
		cfsetispeed (&tty, (speed_t)baud_rate);

		/* Setting other Port Stuff */
		tty.c_cflag     &=  ~PARENB;            // Make 8n1
		tty.c_cflag     &=  ~CSTOPB;
		tty.c_cflag     &=  ~CSIZE;
		tty.c_cflag     |=  CS8;

		tty.c_cflag     &=  ~CRTSCTS;           // no flow control
		tty.c_cc[VMIN]   =  1;                  // read doesn't block
		tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
		tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

		/* Make raw */
		cfmakeraw(&tty);

		//Flush Port, then applies attributes
		tcflush( USB, TCIFLUSH );
		if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
			printf("Error: %d from tcgetattr: %s\n", errno, strerror(errno));
			return false;}
				
	return true;}
	
	void Write( const char* cmd ){
		int n_written = write( USB, cmd, sizeof(cmd) -1 ); }
	
	char Read(){
		memset ( buff,'\0', sizeof(char) );
		int n = read( USB, buff, sizeof(char) ); //printf("%d,%s \n",n,buff);
		if (n < 0) { printf("Error reading: %s\n",strerror(errno)); }
		return buff[0];}
	
	void Close(){ close(USB); }
	
}Serial;