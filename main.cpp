#include "NBody.hpp"

int main(int argc, char* argv[])
{
    int number_of_bodies = 0; //number of bodies read from the file
    double radius = 0;        //Radius of the universe read from the file
    std::vector<Body*> planets; //Vector to store the data from the file

    //Set background music
    sf::Music background_music;
    if(!background_music.openFromFile("Cliff_Lin_Ultraviolence.ogg"))
    {
        std::cout << "Sorry, couldn't open the music file. Exiting..." << std::endl;
        exit(1);
    }
    background_music.setLoop(true);
    background_music.play();

    //Sets font
    sf::Font font;
    if(!font.loadFromFile("DejaVuSerif-Bold.ttf"))
    {
        std::cout << "Sorry, couldn't load the  font file. Exiting..." << std::endl;
        exit(1);
    }

    //Sets the time at the left top corner of the screen
    sf::Text timer;
    timer.setFont(font);
    timer.setPosition(0.f,0.f);
    timer.setScale(0.5f, 0.5f);

    //Creates and opens background image
    sf::Image background;
    if(!background.loadFromFile("starfield.jpg"))
    {
        std::cout << "Sorry, couldn't load the image file. Exiting..." << std::endl;
        exit(1);
    }
    //Gets the size of the background image and uses it as the size of the window
    sf::Vector2u size = background.getSize();
    sf::RenderWindow window(sf::VideoMode(size.x,size.y),"ps3a - NBody Simulation (Dynamic Version)");

    //Creates the texture and sprite of the background image to open it
    sf::Texture texture;
    texture.loadFromImage(background);
    sf::Sprite sprite;
    sprite.setTexture(texture);

    int counter = 0;        	      //counter used to keep track of dt
    double t = (double)atoi(argv[1]); //time in [s]
    double dt =(double)atoi(argv[2]); //change in time in [s]
    std::cin >> number_of_bodies;     //Gets the number of planets to read
    std::cin >> radius;               //Gets the radius of the universe

    for(int i = 0; i < number_of_bodies; i++)
    {
        //Initializes the object with defaults values
        Body* nPlanet = new Body(0,0,0,0,0,"0");
        //Stores the object into the vector
        planets.push_back(nPlanet);
        //Reads the next set of lines and stores them into the object
        std::cin >> *planets[i];
    }

    //Opens the window
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {    
		//Prints all the parameters into the scree before windows close
		std::cout << number_of_bodies << "\n"
			  << radius << std::endl;
    		print_parameters(planets,number_of_bodies);
		window.close();
	    }
        }

        //If the counter hasn't reach the timer
        if(counter < t)
        {
            window.clear(sf::Color::White);
            window.draw(sprite);
            //calculates the information to move the planets
            planets[0]->step(dt,planets,number_of_bodies,0);

            for(int i = 0; i < number_of_bodies; i++)
            {
                //Updates the position from the center of the screen
                planets[i]->update_Pos(radius,size.x/2,size.y/2);
                //Draws the body into the screen
                window.draw(*planets[i]);
                //Makes the planets rotate slower
                sf::sleep(sf::seconds(0.005));
            }
            //Updates the timer
            timer.setString(std::to_string(counter));
            //Increases the counter
            counter = counter + dt;
        }
        window.draw(timer);
        window.display();
    }
    //Saves the final state the planets into a text file
    save_state(number_of_bodies,planets,radius);
    return 0;
}
