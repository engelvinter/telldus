#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "TellUsbD101.h"
#ifdef __MINGW32__
  #define sleep(x) _sleep((x)*1000)
#endif

void print_usage( char *name ) {
	printf("Usage:\n");
	printf("   %s [ options ]\n", name);
	printf("\n");
	printf("Options: -[lh] [--list] [--help]\n");
	printf("               [--on device] [--off device]\n");
	printf("\n");
	printf("Options:\n");
	printf("       --list (-l short option)\n");
	printf("             List currently configured devices.\n");
	printf("\n");
	printf("       --help (-h short option)\n");
	printf("             Shows this screen.\n");
	printf("\n");
	printf("       --on device\n");
	printf("             Turns on device. device should be an integer of the device-id.\n");
	printf("             The device-id is outputed with the --list option\n");
	printf("\n");
	printf("       --off device\n");
	printf("             Turns off device. device should be an integer of the device-id.\n");
	printf("             The device-id is outputed with the --list option\n");
	printf("\n");
}

void print_device( int index ) {
	int intId = devGetDeviceId(index);
	char *name = devGetName(intId);
	printf("%i\t%s\n", intId, name);
}

void list_devices() {
	int intNum = devGetNumberOfDevices();
	printf("Number of devices: %i\n", intNum);
	int i = 0;
	while (i < intNum) {
		print_device( i );
		i++;
	}
}

void switch_device( bool turnOn, char *device ) {
	int deviceId = atoi(device);
	if (deviceId == 0) { //Try to find the id from the name
		int intNum = devGetNumberOfDevices();
		int index = 0;
		while (index < intNum) {
			int id = devGetDeviceId(index);
			char *name = devGetName( id );
			if (strcasecmp(name, device) == 0) {
				deviceId = id;
				break;
			}
			index++;
		}
		if (deviceId == 0) {
			printf("Device '%s', not found!\n", device);
			return;
		}
	}

	if (turnOn) {
		char *name = devGetName( deviceId );
		bool ok = devTurnOn( deviceId );
		printf("Turning on device: %i %s - %s\n", deviceId, name, (ok ? "ok" : "failed"));
	} else {
		char *name = devGetName( deviceId );
		bool ok = devTurnOff( deviceId );
		printf("Turning off device: %i %s - %s\n", deviceId, name, (ok ? "ok" : "failed"));
	}
	sleep(1);
}

int main(int argc, char **argv)
{
	int optch, longindex;
	static char optstring[] = "ln:f:h";
	static struct option long_opts[] = {
		{ "list", 0, 0, 'l' },
		{ "on", 1, 0, 'n' },
		{ "off", 1, 0, 'f' },
		{ "help", 1, 0, 'h' },
		{ 0, 0, 0, 0}
	};

	if (argc < 2) {
		print_usage( argv[0] );
		return -1;
	}

	while ( (optch = getopt_long(argc,argv,optstring,long_opts,&longindex)) != -1 )
		switch (optch) {
			case 'f' :
				switch_device(false, &optarg[0]);
				break;
			case 'h' :
				print_usage( argv[0] );
				break;
			case 'l' :
				list_devices();
				break;
			case 'n' :
				switch_device(true, &optarg[0]);
				break;
			default :
				print_usage( argv[0] );
				return -1;
		}

	return 0;
}
