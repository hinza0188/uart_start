#include <stdio.h>
#include "Timer.h"

void run_timer() {
    char test[] = "hi\r\n";
	printf("%s", test);
	return;
}

void stop_timer() {
    char test[] = "bye\r\n";
	printf("%s", test);
	return;
}
