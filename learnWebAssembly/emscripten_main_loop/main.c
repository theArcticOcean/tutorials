#include <emscripten.h>
#include <stdio.h>

void msg_loop() {
	static int count = 0;
	if (count % 60 == 0) {
		printf("count:%d\n", count);
	}
	count++;
}

int main() {
	printf("main() start\n");
    while(1) {
        msg_loop();
    }
	//emscripten_set_main_loop(msg_loop, 0, 1);
	printf("main() end\n");
	return 0;
}