#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <numeric>
#include <fstream>
#include <vector>

#include <bitset>

using namespace std;


int main(int argc, const char *argv[]) {

    // Argument check
    if ( argc < 2 ) { std::cout << "Please provice an audio file" << std::endl; }

    // Variables
    int x, y;
    int window_height = 1080;
    int window_width = 1920;
    int frame_rate = 30;

    std::vector<string> list;
    std::string s;

    const sf::Int16* sample_buffer;
    uint64_t sample_count;
    unsigned int sample_rate;
    unsigned int channel_count;
    int32_t duration_milliseconds; // Note: One second = 1000 milliseconds

    int samples_per_frame;

    std::chrono::system_clock::time_point tp;

    // Start
    sf::SoundBuffer sb;
    sb.loadFromFile( argv[1] );

    sf::Sound sound;
    sound.setBuffer( sb );

    // Get Buffer Info
    sample_buffer = sb.getSamples();
    sample_count = sb.getSampleCount();
    sample_rate = sb.getSampleRate();
    channel_count = sb.getChannelCount();
    duration_milliseconds = sb.getDuration().asMilliseconds();

    samples_per_frame = sample_rate / frame_rate;
    
    std::cout << "file: " << argv[1] << std::endl;
    std::cout << "sample_count: " << sample_count << std::endl;
    std::cout << "sample_rate: " << sample_rate << std::endl;
    std::cout << "channel_count: " << channel_count << std::endl;
    std::cout << "duration_milliseconds: " << duration_milliseconds << std::endl;
    std::cout << "samples_per_frame: " << samples_per_frame << std::endl;

    // Exit if more than one channel
    if ( channel_count > 1 ) { std::cout << "Not designed to handle more than one channel" << std::endl; return 0; }

    // Create Window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Audio Visualizer");
    //window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    sf::Vector2u size = window.getSize();
    window_width = size.x;
    window_height = size.y;

    sleep(0.2);

    sound.play();

    // run the program as long as the window is open
    while ( window.isOpen() ) {

        // Get Start Time of Loop
        tp = std::chrono::system_clock::now();


        sf::VertexArray line(sf::LineStrip, samples_per_frame );
        ///line[0].position = sf::Vector2f((window_height / 2 ) + (window_width / 2))

        for (int i = 1; i < samples_per_frame; i++) {

            x = int( float(float(window_width) / float( samples_per_frame )) * i);
            y = int(window_height / 2) + (*sample_buffer++ / 25 );
            line[i].position = sf::Vector2f( x, y);
            line[i].color = sf::Color::Red;
        }

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while ( window.pollEvent(event) ) {

            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
	    }


        // clear the window with black color
        window.clear(sf::Color::Black);

        window.draw(line);

        // end the current frame
        window.display();

        // Calculate how much time has elapsed
        std::chrono::duration< double > time_span = std::chrono::duration_cast< std::chrono::duration< double > > ( std::chrono::system_clock::now() - tp );
        std::cout << "time_span ns: " << std::chrono::duration_cast< std::chrono::nanoseconds >( time_span ).count() << std::endl;

        // Calculate when frame is supposed to end
        //std::chrono::system_clock::time_point frame_duration = tp + std::chrono::seconds( 1 / frame_rate );
        std::chrono::duration< int, std::ratio<1,30 > > frame_duration (1);
        std::chrono::system_clock::time_point frame_end = tp + std::chrono::duration_cast< std::chrono::nanoseconds > ( frame_duration );

        // Calculate how much time to sleep for
        std::chrono::duration< double > time_sleep = std::chrono::duration_cast< std::chrono::duration< double > > ( frame_end - std::chrono::system_clock::now() );
        std::cout << "time_sleep ns: " << std::chrono::duration_cast< std::chrono::nanoseconds >( time_sleep ).count() << std::endl;

        // Sleep the rest of the frame duration
        std::this_thread::sleep_for( time_sleep );

    }


    sound.stop();


	return 0;
}


