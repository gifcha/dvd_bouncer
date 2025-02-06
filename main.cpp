#include <iostream>
#include <filesystem>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Floater: public sf::RectangleShape{
	private:
		int currTextureIndex = 8;
	public:
		// motion and speed is in pixels
		int speed = 3;
		sf::Vector2i motion = sf::Vector2i(speed, speed);
		std::vector<sf::Texture> textures;

		Floater(std::string img_dir) {
			textures = getTexturesFromDir(img_dir);
			sf::Texture &tex = textures.at(0);
			setSize((sf::Vector2f)tex.getSize());
			setTexture((sf::Texture*) &tex);
		}

		std::vector<sf::Texture> getTexturesFromDir(std::string img_dir) {
			std::vector<sf::Texture> textures;
			// reserves 9 slots for the dvd images to prevent multiple resizes early
			textures.reserve(9);

			std::cout << "Loading textures\n";
			for(auto& p: std::filesystem::directory_iterator(img_dir)) {
				std::cout << p << '\n';

				sf::Texture tex;
				tex.loadFromFile(p.path().string());
				textures.push_back(tex);
			}

			return textures;
		}

		void switchTexture() {
			currTextureIndex += 1;
			if (currTextureIndex > textures.size()-1) {
				currTextureIndex = 0;
			}
			setTexture((sf::Texture*) &textures.at(currTextureIndex), true);
			setSize((sf::Vector2f) textures.at(currTextureIndex).getSize());
		}

		sf::Vector2f getCenterPos() {
			return sf::Vector2f(getPosition().x + getSize().x/2, getPosition().y + getSize().y/2);
		}
};


int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bouncer");

	sf::View view(sf::FloatRect(1, 1, window.getSize().x, window.getSize().y));
	window.setView(view);

	window.setFramerateLimit(60);


	Floater floater("images");

    while (window.isOpen())
    {
		view.zoom(1);

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

		// bounce logic
		if (floater.getCenterPos().x + floater.getSize().x/2 >= window.getSize().x) {
			floater.motion = sf::Vector2i(-floater.motion.x, floater.motion.y);
			floater.switchTexture();
		}
		if (floater.getCenterPos().x - floater.getSize().x/2 <= 0) {
			floater.motion = sf::Vector2i(-floater.motion.x, floater.motion.y);
			floater.switchTexture();
		}
		if (floater.getCenterPos().y + floater.getSize().y/2 >= window.getSize().y) {
			floater.motion = sf::Vector2i(floater.motion.x, -floater.motion.y);
			floater.switchTexture();
		}
		if (floater.getCenterPos().y - floater.getSize().y/2 <= 0) {
			floater.motion = sf::Vector2i(floater.motion.x, -floater.motion.y);
			floater.switchTexture();
		}

		// update floater position
		sf::Vector2i fpos = window.mapCoordsToPixel(floater.getPosition()) + floater.motion;
		floater.setPosition(window.mapPixelToCoords(fpos));

		window.draw(floater);

		// Resizing event
		if(event.type == sf::Event::Resized)
		{
			sf::FloatRect view(0, 0, event.size.width, event.size.height);
			window.setView(sf::View(view));
		}

        // Update the window
        window.display();
    }


	return EXIT_SUCCESS;
}

