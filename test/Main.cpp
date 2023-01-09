#define CATCH_CONFIG_RUNNER
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <catch2/catch.hpp>
#include <iostream>

int main(const int argc, const char* argv[])
{
	return Catch::Session().run(argc, argv);
}