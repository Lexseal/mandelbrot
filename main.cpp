#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

#define X_MIN -1.03
#define X_MAX -0.88
#define Y_MIN -0.3
#define Y_MAX -0.25

#define RESOLUTION 10000

#define ITER 90

sf::Color hsv(int hue, float sat, float val) {
    hue %= 360;
    while (hue<0) hue += 360;

    if (sat<0.f) sat = 0.f;
    if (sat>1.f) sat = 1.f;

    if (val<0.f) val = 0.f;
    if (val>1.f) val = 1.f;

    int h = hue / 60;
    float f = float(hue) / 60 - h;
    float p = val*(1.f - sat);
    float q = val*(1.f - sat*f);
    float t = val*(1.f - sat*(1 - f));

    switch (h) {
		default:
		case 0:
		case 6: return sf::Color(val * 255, t * 255, p * 255);
		case 1: return sf::Color(q * 255, val * 255, p * 255);
		case 2: return sf::Color(p * 255, val * 255, t * 255);
		case 3: return sf::Color(p * 255, q * 255, val * 255);
		case 4: return sf::Color(t * 255, p * 255, val * 255);
		case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}

void set(sf::Uint8* pixels, const int &r, const int &c, const sf::Color &color) {
	int width = (X_MAX-X_MIN)*RESOLUTION;
	sf::Uint8 *pixel = pixels+(r*width+c)*4;
	*pixel = color.r; pixel++;
	*pixel = color.g; pixel++;
	*pixel = color.b; pixel++;
	*pixel = color.a;
}

int main() {
	int width = ((X_MAX-X_MIN)*RESOLUTION+0.5);
	int height = ((Y_MAX-Y_MIN)*RESOLUTION+0.5);
	std::cout << width << " " << height << std::endl;
	std::cout << X_MIN*RESOLUTION << " " << X_MAX*RESOLUTION << std::endl;
	std::cout << Y_MIN*RESOLUTION << " " << Y_MAX*RESOLUTION << std::endl;
	sf::RenderWindow window(sf::VideoMode(width, height), "mandelbrot");
	sf::Uint8 *pixels = new sf::Uint8[width * height * 4];
	sf::Texture texture;
	texture.create(width, height);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	int count = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		for (int a = X_MIN*RESOLUTION; a < X_MAX*RESOLUTION; a++) {
			for (int b = Y_MIN*RESOLUTION; b < Y_MAX*RESOLUTION; b++) {
				std::complex<double> c(double(a)/double(RESOLUTION), double(b)/double(RESOLUTION));
				std::complex<double> z = c;
				double length = norm(z);
				int iter = 0;
				while (length <= 2 && (++iter) < ITER) {
					z = pow(z, 2) + c;
					length = norm(z);
				}
				
				//std::cout << length << std::endl;
				
				int row = b-Y_MIN*RESOLUTION;
				int col = a-X_MIN*RESOLUTION;
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
