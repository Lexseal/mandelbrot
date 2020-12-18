#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

#define INIT_WIN_WIDTH 640
#define INIT_WIN_HEIGHT 360
#define RESOLUTION 1024
#define SCALING_FACTOR 1.1

#define ITER 64

void set(sf::Uint8* pixels, const int &r, const int &c, const sf::Color &color) {
	sf::Uint8 *pixel = pixels+(r*INIT_WIN_WIDTH+c)*4;
	*pixel = color.r; pixel++;
	*pixel = color.g; pixel++;
	*pixel = color.b; pixel++;
	*pixel = color.a;
}

int main() {
	double winWidth = INIT_WIN_WIDTH;
	double winHeight = INIT_WIN_HEIGHT;
	double scaling = 0.2; // default scaling factor
	double xCenter = 0.0;
	double xMax = xCenter + winWidth/2.0/double(RESOLUTION)/scaling;
	double xMin = xCenter - winWidth/2.0/double(RESOLUTION)/scaling;
	double xSpan = xMax-xMin;
	double yCenter = 0.0;
	double yMax = yCenter + winHeight/2.0/double(RESOLUTION)/scaling;
	double yMin = yCenter - winHeight/2.0/double(RESOLUTION)/scaling;;
	double ySpan = yMax-yMin;
	

	std::cout << xMin << " " << xMax << " " << yMin << " " << yMax << std::endl;
	std::cout << xSpan << " " << ySpan << std::endl;

	sf::RenderWindow window(sf::VideoMode(INIT_WIN_WIDTH, INIT_WIN_HEIGHT), "mandelbrot");
	sf::Uint8 *pixels = new sf::Uint8[INIT_WIN_WIDTH * INIT_WIN_HEIGHT * 4];
	sf::Texture texture;
	texture.create(INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	int count = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0) scaling *= SCALING_FACTOR;
					else scaling /= SCALING_FACTOR;
			        xMax = xCenter + winWidth/2.0/double(RESOLUTION)/scaling;
			        xMin = xCenter - winWidth/2.0/double(RESOLUTION)/scaling;
			        xSpan = xMax-xMin;
			        yMax = yCenter + winHeight/2.0/double(RESOLUTION)/scaling;
			        yMin = yCenter - winHeight/2.0/double(RESOLUTION)/scaling;
			        ySpan = yMax-yMin;
				}
			} else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					xCenter = xMin + double(event.mouseButton.x)/winWidth*xSpan;
					yCenter = yMin + ySpan - double(event.mouseButton.y)/winHeight*ySpan;
					xMax = xCenter + winWidth/2.0/double(RESOLUTION)/scaling;
			        xMin = xCenter - winWidth/2.0/double(RESOLUTION)/scaling;
			        yMax = yCenter + winHeight/2.0/double(RESOLUTION)/scaling;
			        yMin = yCenter - winHeight/2.0/double(RESOLUTION)/scaling;
					std::cout << xCenter << " " << yCenter << std::endl;
				}
			}
			
		// 	int width = xPixMax-xPixMin+1;
		// 	int height = yPixMax-yPixMin+1;
		}

		for (int row = 0; row < INIT_WIN_HEIGHT; row++) {
			for (int col = 0; col < INIT_WIN_WIDTH; col++) {
				double a = xMin + double(col)/winWidth*xSpan;
				double b = yMax - double(row)/winHeight*ySpan;
				//std::cout << a << " " << b << std::endl;
				std::complex<double> c(a, b);
				std::complex<double> z = c;
				double length = norm(z);
				int iter = 0;
				while (length <= 2 && (++iter) < ITER) {
					z = pow(z, 2) + c;
					length = norm(z);
				}
				
				//std::cout << length << std::endl;
				
				if (iter == ITER) {
					set(pixels, row, col, sf::Color::Black);
				} else {
					set(pixels, row, col, sf::Color((iter%4)*64, (iter%8)*32, (iter%16)*16));
				}
			}
		}
		std::cout << "loop " << count++ << std::endl;

		texture.update(pixels);

		window.clear();
		window.draw(sprite);
		window.display();
	}
	return 0;
}
