#include <SFML/Graphics.hpp>
#include <windows.h> 
#include "Menu.h"

using namespace sf;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ShowWindow(GetConsoleWindow(), SW_HIDE); //Скрываем консольное окно
	RenderWindow window(VideoMode(600, 600), "Pinball",Style::None|Style::Close); 
	window.setFramerateLimit(70);

	menu(window);

	return 0;
}