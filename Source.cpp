#include <iostream>
#include <cstdlib>

#include <sstream>

#include <SFML/Graphics.hpp>



sf::RenderWindow window(sf::VideoMode(200,200), "SFML works!");

//translate decimal to hex
std::string toHex(uint16_t dec) {

	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase << dec;
	return ss.str();
}

void toBin(unsigned n)
{
	/* step 1 */
	if (n > 1)
		toBin(n / 2);

	/* step 2 */
	std::cout << n % 2;
}

std::pair<uint16_t, uint16_t> getAddress(uint16_t instruction) {

	std::pair<uint16_t, uint16_t> address;

	address.first = instruction & 0x0f00;
	address.first = address.first >> 8;
	address.second = instruction & 0x00f0;
	address.second = address.second >> 4;

	return address;
}


int main() {

	



	sf::RenderWindow windowInfo(sf::VideoMode(400,500), "window info");
	windowInfo.setPosition(sf::Vector2i(0, 0));

	sf::Font font;
	if (!font.loadFromFile("Retro Gaming.ttf")) {
		std::cout << "ERROR :: failed to load font \n";
	}

	


	while (window.isOpen())
	{
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
				windowInfo.close();
			}
		}

		window.clear();
		windowInfo.clear(sf::Color::Blue);
		//--------------------//

		//--------------------//
		window.display();
		windowInfo.display();

		

	}
}