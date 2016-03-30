#include "screen.h"
#include "type.h"

void say_hello() {
	clear_screen();
	print("Welcome to my developed OS: kiddd~\n");
}

int main(){
	say_hello();
	return 0;
}