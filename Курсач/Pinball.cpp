#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include "Pinball.h"
#include "Points.h"

int record = 0;
int attempt = 3;//НАДО поставить 3!!!!!!!!
int numberActiv = 0;
float powerSpring = 0;
bool exitGame = true;
static const float SCALE = 30.f;
b2RevoluteJoint* m_leftJoint;
b2RevoluteJoint* m_rightJoint;

void CreateGround(b2World& World, float X, float Y, float Angle, float width, float length);

void UpdateVariable();

void CreateBall(b2World& World, float X, float Y);
void CreateFlipperLeft(b2World& World, b2Body* pinball);
void CreateFlipperRight(b2World& World, b2Body* pinball);
void CreateBonusCircle(b2World& World, float X, float Y);
void CreateBonusLine(b2World& World, float X, float Y, float Angle);

int startGame(RenderWindow & window, bool soundP)
{
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	b2BodyDef BodyDef;
	b2Body* pinball = World.CreateBody(&BodyDef);

	UpdateVariable();

	////////////Стенки ограничивающие поле для игры
	CreateGround(World, 300.f, .0f, .0f, 300.f, 1.f);
	CreateGround(World, 0.f, 300.0f, 1.57f, 300.f, 1.f);
	CreateGround(World, 21.f, 300.f, 1.57f, 300.f, 1.f);
	CreateGround(World, 379.f, 330.f, 1.57f, 300.f, 1.f);
	CreateGround(World, 400.f, 300.f, 1.57f, 300.f, 1.f);
	CreateGround(World, 65.5f, 465.5f, 1.57f / 2.0f, 95.f, 1.f); // 1.57 радиан - 90 градусов
	CreateGround(World, 334.5f, 465.5f, -1.57f / 2.0f, 95.f, 1.f);
	CreateGround(World, 392.f, 7.f, 1.57f / 2.0, 10.f, 1.f);
	CreateGround(World, 390.f, 400.f, 0, 10.f, 1.f);
	CreateGround(World, 83.f, 360.f, -1.1687f, 43.5f, 1.f);
	CreateGround(World, 99.f, 433.f, 0.791978f, 46.7f, 1.f);
	CreateGround(World, 317.f, 360.f, 1.1687f, 43.5f, 1.f);
	CreateGround(World, 301.f, 433.f, -0.791978f, 46.7f, 1.f);

	CreateBall(World, 390.f, 370.f);
	CreateFlipperRight(World, pinball);
	CreateFlipperLeft(World, pinball);
	CreateBonusCircle(World, 100.f, 100.f);
	CreateBonusCircle(World, 300.f, 100.f);
	CreateBonusCircle(World, 200.f, 200.f);
	CreateBonusLine(World, 116.f, 393.f, 1.355f);
	CreateBonusLine(World, 284.f, 393.f, -1.355f);

	Texture BonusTextureLine, BonusTextureCircle;
	Texture BoxTexture;
	Texture BallTexture, SpringTexture, FlipperLeftTexture, FlipperRightTexture;
	Texture Ground;
	BonusTextureCircle.loadFromFile("images/Game/Light.png");
	BonusTextureLine.loadFromFile("images/Game/BonusLine.png");
	BallTexture.loadFromFile("images/Game/Ball.png");
	SpringTexture.loadFromFile("images/Game/Spring.png");
	FlipperLeftTexture.loadFromFile("images/Game/left.png");
	FlipperRightTexture.loadFromFile("images/Game/right.png");
	Ground.loadFromFile("images/Game/GroundGame.png");

	Sprite ground(Ground), spring(SpringTexture), circle(BonusTextureCircle);
	ground.setPosition(0, 0);
	spring.setPosition(384, 406);

	bool buttonRight = false, buttonLeft = false;

	Font font;
	font.loadFromFile("font/fontGame.ttf");
	Text text;
	text.setFont(font); // выбираем шрифт
	
	Clock clock; // часы запускаются
	Time timePower = clock.getElapsedTime();
	bool startTime = false, playSoundRight = false, playSoundLeft = false, playPower = false;

	window.draw(ground);
	window.draw(spring);
	window.display();
	Music musicGround, soundStart, soundFliperLeft, soundFliperRight, soundPower, soundCircle, soundLine;//создаем объект музыки
	musicGround.openFromFile("music/start.wav");//загружаем файл
	if (soundP) musicGround.play();//воспроизводим музыку
	clock.restart();
	while (clock.getElapsedTime() < seconds(5.0f) && soundP);
	soundStart.openFromFile("music/beginBall.wav");
	soundFliperLeft.openFromFile("music/Flipper.wav");
	soundFliperRight.openFromFile("music/Flipper.wav");
	soundPower.openFromFile("music/Power.wav");
	soundCircle.openFromFile("music/Circle.wav");
	soundLine.openFromFile("music/Line.wav");
	if (soundP) soundStart.play();

	while (exitGame  && window.isOpen())
	{
		World.Step(1 / 60.f, 5, 2);
		Event event; // Проверяем все события окна, которые были вызваны с последней итерации цикла
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // Закрываем окно, в случае получения управляющего события
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
			if (event.type == sf::Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space) && startTime == false)
			{
				clock.restart();
				startTime = true;

			}
				if (event.type == Event::KeyReleased && event.key.code == Keyboard::Space && startTime == true)
				{
					if (event.type == Event::KeyReleased)
					{
						timePower = clock.getElapsedTime();
						for (b2Body* BodyBall = World.GetBodyList(); BodyBall != 0; BodyBall = BodyBall->GetNext())
						{
							if (BodyBall->GetUserData() == "Ball" && 380 / SCALE < BodyBall->GetPosition().x && BodyBall->GetPosition().x < 400 / SCALE && 380 / SCALE < BodyBall->GetPosition().y && BodyBall->GetPosition().y < 400 / SCALE)
							{
								float pusk = 0;
								for (Time t = seconds(0.f);t < timePower;t += seconds(0.05f)) pusk += 0.5f;
								if (timePower > seconds(4.0f)) pusk = 80.0f;
								BodyBall->ApplyLinearImpulseToCenter(b2Vec2(0, -pusk), 1);
								
							}
						}
						startTime = false;
					}
				}
			

		}

		if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			buttonLeft = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			buttonRight = true;
		}

		if (buttonLeft)
		{
			if (!playSoundLeft)
			{
				if(soundP) soundFliperLeft.play();
				playSoundLeft = true;
			}
			m_leftJoint->SetMotorSpeed(-15.0f);
			sleep(seconds(0.001f));
		}
		else
		{
			playSoundLeft = false;
			m_leftJoint->SetMotorSpeed(25.0f);
			sleep(seconds(0.001f));
		}
		buttonLeft = false;

		if (buttonRight)
		{
			if (!playSoundRight)
			{
				if (soundP) soundFliperRight.play();
				playSoundRight = true;
			}
			m_rightJoint->SetMotorSpeed(15.0f);
			sleep(seconds(0.001f));
		}
		else
		{
			playSoundRight = false;
			m_rightJoint->SetMotorSpeed(-25.0f);
			sleep(seconds(0.001f));
		}
		buttonRight = false;

		window.clear(Color::Transparent);
		if(startTime == true)
		{
			timePower = clock.getElapsedTime();
			if (timePower < seconds(4.f))
			{
				powerSpring = 1.0f * (timePower / seconds(0.13793f));
			}
			else
			{
				powerSpring = 29.0f;
			}
			playPower = true;
		}
		else 
		{
			if (playPower)
			{
				if (soundP) soundPower.play();
				playPower = false;
			}
			powerSpring = 0.0f;
		}
		spring.setPosition(384, 406 + powerSpring);
		window.draw(spring);
		window.draw(ground);

		for (b2Contact* contact = World.GetContactList(); contact; contact = contact->GetNext())
		{
			b2Fixture* a = contact->GetFixtureA();
			b2Fixture* b = contact->GetFixtureB();
			b2Body* AA = a->GetBody();
			b2Body* BB = b->GetBody();
			bool beginContact = contact->IsTouching();
			if (beginContact == true && ( AA->GetUserData() == "Ball" || BB->GetUserData() == "Ball") && (AA->GetUserData() == "BonusCircle" || BB->GetUserData() == "BonusCircle"))
			{
				if (BB->GetPosition().x < 150/SCALE) { circle.setPosition(100 - 5, 100 - 5); }
				else if (BB->GetPosition().y > 150/ SCALE) { circle.setPosition(200 - 5, 200 - 5); }
				else if (BB->GetPosition().x > 250/ SCALE) { circle.setPosition(300 - 5, 100 - 5); }
				record += 100;
				if (soundP) soundCircle.play();
				window.draw(circle);
			}		
			if (beginContact == true && (AA->GetUserData() == "Ball" || BB->GetUserData() == "Ball") && (AA->GetUserData() == "BonusLine" || BB->GetUserData() == "BonusLine"))
			{
				record += 200;
				if (soundP) soundLine.play();
			}
		}
		

		std::ostringstream buffRecord, buffPoints;
		buffRecord << record;
		text.setString(buffRecord.str()); // выбираем отображаемую строку
		text.setCharacterSize(28); // выбираем размер символов
		text.setColor(sf::Color::Red); // выбираем цвет
		text.setStyle(sf::Text::Bold); // выбираем стиль текста	
		text.setPosition(460.f, 285.f);//задаем позицию текста, отступая от центра камеры
		window.draw(text);//рисую этот текст
		buffPoints << attempt;
		text.setString(buffPoints.str()); // выбираем отображаемую строку
		text.setCharacterSize(28); // выбираем размер символов
		text.setColor(sf::Color::Red); // выбираем цвет
		text.setStyle(sf::Text::Bold); // выбираем стиль текста	
		text.setPosition(555.f, 203.f);//задаем позицию текста, отступая от центра камеры
		window.draw(text);//рисую этот текст
		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{

			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				Sprite Sprite;
				if (BodyIterator->GetUserData() == "Ball")
				{
					if (BodyIterator->GetPosition().y > 600 / SCALE)
					{
						if(attempt != 1 && soundP) soundStart.play();
						attempt--;
						BodyIterator->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
						BodyIterator->SetTransform(b2Vec2(390.f / SCALE, 360.f / SCALE), 0.f);
					}
					Sprite.setTexture(BallTexture);
					Sprite.setOrigin(9.f, 9.f);
				}
				else if (BodyIterator->GetUserData() == "RightFlipper")
				{
					Sprite.setTexture(FlipperRightTexture);
					Sprite.setOrigin(60.f, 5.f);
				}
				else if (BodyIterator->GetUserData() == "LeftFlipper")
				{
					Sprite.setTexture(FlipperLeftTexture);
					Sprite.setOrigin(50.f, 5.f);
				}
				else
				{
					Sprite.setTexture(BoxTexture);
					Sprite.setOrigin(16.f, 16.f);
				}
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				window.draw(Sprite);
			}
			else
			{
				Sprite GroundSprite;
				if (BodyIterator->GetUserData() == "BonusLine")
				{
					GroundSprite.setTexture(BonusTextureLine);
					GroundSprite.setOrigin(75.f, 1.f);
				}
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				window.draw(GroundSprite);
			}
		}
		window.display();

		if (attempt == 0)
		{
			exitGame = false;
		}
	}
	if(attempt == 0) recPoints(record);
	return record;
}

void CreateBall(b2World& World, float X, float Y) //Создание мячика
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2CircleShape Circle;
	Circle.m_radius = 9.f / SCALE;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 3.5f;
	FixtureDef.restitution = 0.3f;
	FixtureDef.shape = &Circle;
	Body->CreateFixture(&FixtureDef);
	Body->SetUserData("Ball");
	Body->SetBullet(true);
}

void CreateFlipperLeft(b2World& World, b2Body* pinball) //Созданий левого флипера
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(131.f / SCALE, 539.5f / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* leftFlipper = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(60.f / SCALE, 5.f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 3.0f;
	FixtureDef.shape = &Shape;
	leftFlipper->CreateFixture(&FixtureDef);
	leftFlipper->SetUserData("LeftFlipper");

	b2RevoluteJointDef jd;
	jd.bodyA = pinball;
	jd.localAnchorB.SetZero();
	jd.enableMotor = true;
	jd.maxMotorTorque = 1000.0f;
	jd.enableLimit = true;

	jd.motorSpeed = 0.0f;
	jd.localAnchorA = b2Vec2(131.f / SCALE, 539.5f / SCALE);
	jd.bodyB = leftFlipper;
	jd.lowerAngle = -15.0f * b2_pi / 180.0f;
	jd.upperAngle = 30.0f * b2_pi / 180.0f;
	m_leftJoint = (b2RevoluteJoint*)World.CreateJoint(&jd);
	leftFlipper->SetBullet(true);
}

void CreateFlipperRight(b2World& World, b2Body* pinball) //создание правого флипера
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(269.f / SCALE, 539.5f / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* rightFlipper = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(60.f / SCALE, 5.f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 3.0f;
	FixtureDef.shape = &Shape;
	rightFlipper->CreateFixture(&FixtureDef);
	rightFlipper->SetUserData("RightFlipper");

	b2RevoluteJointDef jd;
	jd.bodyA = pinball;
	jd.localAnchorB.SetZero();
	jd.enableMotor = true;
	jd.maxMotorTorque = 1000.0f;
	jd.enableLimit = true;

	jd.motorSpeed = 0.0f;
	jd.localAnchorA = b2Vec2(269.f / SCALE, 539.5f / SCALE);
	jd.bodyB = rightFlipper;
	jd.lowerAngle = -30.0f * b2_pi / 180.0f;
	jd.upperAngle = 15.0f * b2_pi / 180.0f;
	m_rightJoint = (b2RevoluteJoint*)World.CreateJoint(&jd);
	rightFlipper->SetBullet(true);
}

//////////Предмет/координата х/координата у/угол/ширина/длина
void CreateGround(b2World& World, float X, float Y, float Angle, float width, float length)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.angle = Angle;
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(width / SCALE, length / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.restitution = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateBonusCircle(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2CircleShape Circle;
	Circle.m_radius = 25.f / SCALE;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.restitution = 1.7f;
	//FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Circle;
	Body->CreateFixture(&FixtureDef);
	Body->SetUserData("BonusCircle");
}

void CreateBonusLine(b2World& World, float X, float Y, float Angle)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.angle = Angle;
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(75.f / SCALE, 1.f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.restitution = 2.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
	Body->SetUserData("BonusLine");
}

void UpdateVariable() //Обновление переменных
{
	record = 0;
	attempt = 3;
	exitGame = true;
}