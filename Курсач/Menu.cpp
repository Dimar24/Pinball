#include <iostream>
#include <dos.h>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pinball.h"

bool SoundP = true;//���������� ��� ������������ �����
int volume = 1;//����������� ������
Music musicGround;//������� ������ ������

void menuTablicaRecords(RenderWindow & window, Sound tap);
void menuSettings(RenderWindow & window, Sound tap);
void playSound(Sound tap);
void playMusic();

void menu(RenderWindow & window)
{
	SoundBuffer tapBuffer; //������ ����� ��� �����
	tapBuffer.loadFromFile("music/tik.ogg"); //��������� � ���� ����
	Sound tap(tapBuffer); //������� ���� � ��������� � ���� ���� �� ������

	musicGround.openFromFile("music/musicGround.ogg");//��������� ����
	musicGround.play();//������������� ������
	musicGround.setLoop(true);//����������� ������
	playMusic();

	Texture menuTextureGround, menuTextureStart, menuTextureSettings, menuTextureTablicaRecords, menuTextureExit;
	menuTextureGround.loadFromFile("images/Menu/Ground.png"); //���
	menuTextureStart.loadFromFile("images/Menu/NewGame.png"); //������ - "����� ����"
	menuTextureSettings.loadFromFile("images/Menu/Settings.png"); //������ - "���������"
	menuTextureTablicaRecords.loadFromFile("images/Menu/TablicaRecords.png"); //������ - "������� ��������"
	menuTextureExit.loadFromFile("images/Menu/Exit.png"); //������ - "�����"
	Sprite menuSta(menuTextureStart), menuTab(menuTextureTablicaRecords), menuSet(menuTextureSettings), menuEx(menuTextureExit), ground(menuTextureGround);
	menuSta.setPosition(150, 125);//��������� ��������� ������� ������ "����� ����"
	menuTab.setPosition(125, 200);//��������� ��������� ������� ������ "������� ��������"
	menuSet.setPosition(150, 275);//��������� ��������� ������� ������ "���������"
	menuEx.setPosition(150, 350);//��������� ��������� ������� ������ "�����"
	ground.setPosition(0, 0);

	bool exitMenu = true;//���������� ��� ������ �� ����� ���� - ����� �� ����
	int numberMenu = -1;//���������� ��� ������������� ���������� ������

	while (exitMenu && window.isOpen())//���� ��������� � �������������� � ����
	{
		Event event; // ��������� ��� ������� ����, ������� ���� ������� � ��������� �������� �����
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // ��������� ����, � ������ ��������� ������������ �������
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		menuSta.setColor(Color::White);//���������  ����� ������ "����� ����"
		menuTab.setColor(Color::White);//��������� ����� ������ "������� ��������"
		menuSet.setColor(Color::White);//��������� ����� ������ "���������"
		menuEx.setColor(Color::White);//��������� ����� ������ "�����"

		//�������� �� ���������� ���� �� ����. ��� ���������� ����� �� ����� �� ������, ������ ��������������� � �������
		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenu = -1; }
		if (IntRect(190, 125, 220, 50).contains(Mouse::getPosition(window))) { menuSta.setColor(Color::Red); numberMenu = 1; }
		if (IntRect(130, 200, 340, 50).contains(Mouse::getPosition(window))) { menuTab.setColor(Color::Red); numberMenu = 2; }
		if (IntRect(190, 275, 220, 50).contains(Mouse::getPosition(window))) { menuSet.setColor(Color::Red); numberMenu = 3; }
		if (IntRect(240, 350, 120, 50).contains(Mouse::getPosition(window))) { menuEx.setColor(Color::Red); numberMenu = 0; }

		if (Mouse::isButtonPressed(Mouse::Left)) //�������� �� ������� ����� �� ������
		{
			if (numberMenu != -1) playSound(tap);
			if (numberMenu == 1) { startGame(window, SoundP); }
			if (numberMenu == 2) { menuTablicaRecords(window, tap); }
			if (numberMenu == 3) { menuSettings(window, tap); }
			if (numberMenu == 0) { sleep(seconds(0.1f)); exitMenu = false; }
			numberMenu = -1;
		}

		//��������� ������ � ����
		window.clear();
		window.draw(ground);
		window.draw(menuSta);
		window.draw(menuTab);
		window.draw(menuSet);
		window.draw(menuEx);
		window.display();
	}
}

void menuTablicaRecords(RenderWindow &window, Sound tap)
{
	Texture menuTextureGround, menuTextureBack, menuTextureInsert;
	menuTextureGround.loadFromFile("images/MenuTablica/Ground.png");
	menuTextureBack.loadFromFile("images/MenuTablica/Back.png");
	menuTextureInsert.loadFromFile("images/MenuTablica/Insert.png");
	Sprite ground(menuTextureGround), menuBack(menuTextureBack), menuInsert(menuTextureInsert);
	ground.setPosition(0, 0);
	menuBack.setPosition(15, 540);
	menuInsert.setPosition(295, 540);

	bool exitMenuTablicaRecords = true;
	int numberMenuRecord = -1;

	Font font;
	font.loadFromFile("font/arial.ttf");
	Text text;
	text.setFont(font); // �������� �����

	typedef struct
	{
		char player[15];
		int point;
	} record;

	int Kol = 0;
	char buff[100];
	record *TABLICA = NULL;
	std::ifstream file(L"record/record.txt", ios_base::app);
	file >> Kol;
	TABLICA = (record*)realloc(TABLICA, sizeof(record)*(Kol));
	for (int i = 0; i < Kol; i++) {
		file >> buff;
		sprintf_s(TABLICA[i].player, "%s", buff);
		file >> TABLICA[i].point;
	}
	file.close();

	while (exitMenuTablicaRecords  && window.isOpen())
	{
		Event event; // ��������� ��� ������� ����, ������� ���� ������� � ��������� �������� �����
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // ��������� ����, � ������ ��������� ������������ �������
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		ground.setColor(Color::White);
		menuBack.setColor(Color::White);
		menuInsert.setColor(Color::White);

		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenuRecord = -1; }
		if (IntRect(15, 540, 150, 50).contains(Mouse::getPosition(window))) { menuBack.setColor(Color::Red); numberMenuRecord = 0; }
		if (IntRect(380, 540, 205, 50).contains(Mouse::getPosition(window))) { menuInsert.setColor(Color::Red); numberMenuRecord = 1; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (numberMenuRecord != -1) playSound(tap);
			if (numberMenuRecord == 0) { exitMenuTablicaRecords = false; }
			if (numberMenuRecord == 1) { if (Kol != 0) free(TABLICA); Kol = 0; }
			numberMenuRecord = -1;
		}

		window.draw(ground);
		float sizeX = 0.0f, sizeY = 0.0f;
		for (int i = 0;i < 11;i++)
		{
			sf::String buffRecord;
			if (i == 0)
			{
				buffRecord = L"�����           �����           ������";
				sizeX -= 40.0f;
			}
			else if (i <= Kol)
			{
				char buffer[100];
				sprintf_s(buffer, u8"%d           %s", i, TABLICA[i - 1].player);
				buffRecord = buffer;
				//for (int j = 0; j < 20 - strlen(TABLICA[i - 1].play er); j++)
			}
			else
			{
				buffRecord = L"                  �����";
			}
			text.setString(buffRecord); // �������� ������������ ������
			text.setCharacterSize(40); // �������� ������ ��������
			text.setColor(sf::Color::Red); // �������� ����
			text.setStyle(sf::Text::Bold); // �������� ����� ������	
			text.setPosition(100.f + sizeX, 15.f + sizeY);//������ ������� ������, �������� �� ������ ������
			window.draw(text);//����� ���� �����
			sizeX = 0.0f;
			if (i <= Kol && i != 0)
			{
				char buffer[100];
				sprintf_s(buffer, u8"%d", TABLICA[i - 1].point);
				buffRecord = buffer;
				sizeX += 320.0f;
				text.setString(buffRecord); // �������� ������������ ������
				text.setCharacterSize(40); // �������� ������ ��������
				text.setColor(sf::Color::Red); // �������� ����
				text.setStyle(sf::Text::Bold); // �������� ����� ������	
				text.setPosition(100.f + sizeX, 15.f + sizeY);//������ ������� ������, �������� �� ������ ������
				window.draw(text);//����� ���� �����
			}
			else
			{
				if (i == 0)sizeY += 20.0f;
			}
			sizeY += 45.0f;
			sizeX = 0.0f;
		}
		window.draw(menuInsert);
		window.draw(menuBack);
		window.display();
	}
	file.open("record/record.txt");
	if (Kol == 0 && file.is_open())
	{
		file.close();
		remove("record/record.txt");
	}
}

void menuSettings(RenderWindow &window, Sound tap)
{
	Texture menuTextureGround, menuTextureSoundText, menuTextureSoundOn, menuTextureSoundOff, menuTextureMusicText, menuTextureMusicUp, menuTextureMusicOn, menuTextureMusicOff, menuTextureMusicDown, menuTextureBack;
	menuTextureGround.loadFromFile("images/MenuSound/Ground.png");
	menuTextureSoundOn.loadFromFile("images/MenuSound/SoundOn.png");
	menuTextureSoundOff.loadFromFile("images/MenuSound/SoundOff.png");
	menuTextureMusicUp.loadFromFile("images/MenuSound/Up.png");
	menuTextureMusicOn.loadFromFile("images/MenuSound/MusicOn.png");
	menuTextureMusicOff.loadFromFile("images/MenuSound/MusicOff.png");
	menuTextureMusicDown.loadFromFile("images/MenuSound/Down.png");
	menuTextureBack.loadFromFile("images/MenuSound/Back.png");
	menuTextureMusicText.loadFromFile("images/MenuSound/MusicText.png");
	menuTextureSoundText.loadFromFile("images/MenuSound/SoundText.png");
	Sprite ground(menuTextureGround), menuSOn(menuTextureSoundOn), menuSOff(menuTextureSoundOff), menuMUp(menuTextureMusicUp), menuMDown(menuTextureMusicDown), menuST(menuTextureSoundText), menuMT(menuTextureMusicText), menuBack(menuTextureBack);
	ground.setPosition(0, 0);
	menuST.setPosition(15, 200);
	menuMT.setPosition(75, 250);
	menuSOn.setPosition(340, 200);
	menuSOff.setPosition(340, 200);
	menuMDown.setPosition(255, 250);
	menuMUp.setPosition(425, 250);
	menuBack.setPosition(15, 540);

	bool exitMenuSettings = true;
	int numberMenuSettings = -1;

	while (exitMenuSettings  && window.isOpen())
	{
		Event event; // ��������� ��� ������� ����, ������� ���� ������� � ��������� �������� �����
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // ��������� ����, � ������ ��������� ������������ �������
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		ground.setColor(Color::White);
		menuST.setColor(Color::White);
		menuMT.setColor(Color::White);
		menuSOn.setColor(Color::White);
		menuSOff.setColor(Color::White);
		menuMUp.setColor(Color::White);
		menuMDown.setColor(Color::White);
		menuBack.setColor(Color::White);

		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenuSettings = -1; }
		if (IntRect(340, 200, 50, 50).contains(Mouse::getPosition(window))) { menuSOn.setColor(Color::Red); menuSOff.setColor(Color::Red); numberMenuSettings = 1; }
		if (IntRect(15, 540, 150, 50).contains(Mouse::getPosition(window))) { menuBack.setColor(Color::Red); numberMenuSettings = 0; }
		if (IntRect(265, 250, 25, 50).contains(Mouse::getPosition(window))) { menuMDown.setColor(Color::Red); numberMenuSettings = 2; }
		if (IntRect(435, 250, 25, 50).contains(Mouse::getPosition(window))) { menuMUp.setColor(Color::Red); numberMenuSettings = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{

			if (numberMenuSettings != -1) playSound(tap);
			if (numberMenuSettings == 0) { exitMenuSettings = false; }
			if (numberMenuSettings == 1) {
				if (SoundP == true) { SoundP = false; }
				else { SoundP = true; }
			}
			if (numberMenuSettings == 2 && volume != 0) { volume--; playMusic(); }
			if (numberMenuSettings == 3 && volume != 5) { volume++; playMusic(); }
			numberMenuSettings = -1;
		}

		window.draw(ground);
		window.draw(menuST);
		window.draw(menuMT);
		window.draw(menuMUp);
		window.draw(menuMDown);
		window.draw(menuBack);

		if (SoundP == true) window.draw(menuSOn);
		else window.draw(menuSOff);
		float start = 300;
		for (int i = 0; i < volume;i++, start += 25.0)
		{
			Sprite  menuMOn(menuTextureMusicOn);
			menuMOn.setPosition(start, 250);
			window.draw(menuMOn);
		}
		for (int i = volume; i < 5;i++, start += 25.0)
		{
			Sprite  menuMOff(menuTextureMusicOff);
			menuMOff.setPosition(start, 250);
			window.draw(menuMOff);
		}

		window.display();
	}
}

void playSound(Sound tap)
{
	if (SoundP == true) tap.play();
	sleep(seconds(0.2f));
}

void playMusic()
{
	if (volume == 0) musicGround.pause();
	if (volume != 0 && musicGround.getStatus() == 1) musicGround.play();
	if (volume != 0) musicGround.setVolume(volume * 20.0f);
}