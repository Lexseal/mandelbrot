#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

#define WIDTH 1000
#define HEIGHT 1000

#define ITER 10

void set(sf::Uint8* pixels, const int &r, const int &c, const sf::Color &color) {
	sf::Uint8 *pixel = pixels+(r*WIDTH+c)*4;
	*pixel = color.r; pixel++;
	*pixel = color.g; pixel++;
	*pixel = color.b; pixel++;
	*pixel = color.a;
}

int main() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "mandelbrot");
	sf::Uint8 *pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
	sf::Texture texture;
	texture.create(WIDTH, HEIGHT);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		for (int r = 0; r < HEIGHT; r++) {
			for (int c = 0; c < WIDTH; c++) {
				set(pixels, r, c, sf::Color((r%(c+1))%255, (r*(c+1))%255, (r+c)%255, 255));
			}
		}
		for (int a = -WIDTH/2; a < WIDTH/2; a++) {
			for (int b = -HEIGHT/2; b < HEIGHT/2; b++) {
				std::complex<double> z(0., 0.);
				std::complex<double> c(double(a)/500., double(b)/500.);
				for (int i = 0; i < ITER; i++) {
					z = pow(z, 2) + c;
				}
				double length = norm(z);
				std::cout << length << std::endl;
				
				if (length > 20) {
					set(pixels, b+HEIGHT/2, a+WIDTH/2, sf::Color(int(length)%255, 
																 int(length)%255, 
																 int(length)%255, 
																 255));
				} else {
					set(pixels, b+HEIGHT/2, a+WIDTH/2, sf::Color(10, 0, 0, 255));
				}
			}
		}

		texture.update(pixels);

		window.clear();
		window.draw(sprite);
		window.display();
	}
	return 0;
}
