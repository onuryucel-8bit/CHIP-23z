#include "Window_SFML.h"
#include "Chip8.h"

/*
std::string toBin(int n){
	std::string bin = "";

	while (n > 1){

		bin +=  std::to_string(n % 2);
		n /= 2;
	}

	bin += std::to_string(n);

	std::reverse(bin.begin(), bin.end());

	return bin;
}
*/

int main() {

	Window_SFML window;

	window.run();

}