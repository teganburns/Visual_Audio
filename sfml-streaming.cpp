#include <iostream>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>


#include <numeric>
#include <fstream>
#include <vector>

#include <bitset>

using namespace std;


class MyRecorder : public sf::SoundRecorder
{
    virtual bool onStart()
    {
        cout << "Recorder Started" << endl;

        return true;
    }

    virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount )
    {
        // Do something useful with samples
        sc = sampleCount;
        sample = samples;

        // Return true to continue false to stop
        return true;
    }

    virtual void onStop()
    {
        // Clean up here if necessary
        cout << "Recorder Stoped" << endl;
    }



public:

    std::size_t sc;
    const sf::Int16* sample;

};




int main()
{
    int window_height = 1080;
    int window_width = 1920;

    std::vector<string> list;
    std::string s;
    unsigned int sampleRate = 16000; //44100

    int x;
    int y;


    if (!MyRecorder::isAvailable())
    {
        cerr << "Error: Audio Device not aviable" << endl;
        return -1;
    }

    MyRecorder recorder;
    if (!recorder.start(sampleRate))
    {
        cerr << "Error starting recorder" << endl;
        return -1;
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Audio Visualizer");
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(9);

    std::size_t sc = 16000; // initallize it till it get a real value

    sf::Vector2u size = window.getSize();
    window_width = size.x;
    window_height = size.y;

    sleep(0.2);

    // run the program as long as the window is open
    while (window.isOpen())
    {

	cout << "Sample Count: " << sc << "        HxW: " << window_height << " " << window_width << "  More shit: " << float(float(window_width) / float(sc)) * 1600 << endl;
	sc = recorder.sc;
	sf::VertexArray line(sf::LineStrip, sc);
	//line[0].position = sf::Vector2f((window_height / 2 ) + (window_width / 2))

        for (int i = 1; i < int(sc); i++)
        {
            x = int( float(float(window_width) / float(sc)) * i);
            y = int(window_height / 2) + (*recorder.sample++ / 25 );
            line[i].position = sf::Vector2f( x, y);
            line[i].color = sf::Color::Red;
        }

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
	    }


        // clear the window with black color
        window.clear(sf::Color::Black);


        window.draw(line);

        // end the current frame
        window.display();
    }


    recorder.stop();


	return 0;
}
