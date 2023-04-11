// Timber.cpp : Defines the entry point for the console application.
//

#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
enum class side{LEFT, RIGHT, NONE};
side branchPosition[NUM_BRANCHES];

int main()
{
	sf::VideoMode vm(1366, 768);
	sf::RenderWindow window(vm, "Timber!!!", sf::Style::Fullscreen);

	//check running game
	bool paused = true;

	//add background
	sf::Texture background;
	background.loadFromFile("graphics/background.png");
	sf::Sprite sBackground;
	sBackground.setTexture(background);
	sBackground.setPosition(sf::Vector2f(0, 0));
	sBackground.setScale(sf::Vector2f(0.8, 0.8));
	// add tree
    sf::Texture tree;
    tree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(tree);
    spriteTree.setPosition(570, 0);

	//add clouds
	sf::Texture clouds;
	clouds.loadFromFile("graphics/cloud.png");
	sf::Sprite sClouds[3];
	for(int i=0;i < 3; i++)
	{
		sClouds[i].setTexture(clouds);
		sClouds[i].setPosition(0, i * 100);
	}

    bool cloudActive1 = false;
    bool cloudActive2 = false;
    bool cloudActive3 = false;

    float cloudSpeed1 = 0.0f;
    float cloudSpeed2 = 0.0f;
    float cloudSpeed3 = 0.0f;


	//add bee
	sf::Texture bee;
	bee.loadFromFile("graphics/bee.png");
	sf::Sprite sBee;
	sBee.setTexture(bee);
	sBee.setPosition(sf::Vector2f(0, 600));
	bool beeActive = false;
	float beeSpeed = 0.0f;

	//prepare the player
	sf::Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(550, 550);
    side playerSide = side::LEFT;
    spritePlayer.setScale(sf::Vector2f((spritePlayer.getScale().x * -1), spritePlayer.getScale().y));

	//prepare the gravestone
	sf::Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	sf::Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//prepare the axe
	sf::Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(530, 620);

    const float AXE_POSITION_LEFT = 530;
    const float AXE_POSITION_RIGHT = 700;

    //prepare the flying log
    sf::Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(-200, 830);
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

	////add font and text
	//font
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
    //text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);
    //position the text
    sf::FloatRect textRect = messageText.getGlobalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                          textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1360 / 2.0f, 768 / 2.0f);
    scoreText.setPosition(20, 20);

    //prepare 6 branch
    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for(int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setPosition(220, 20);
    }

	//clock
	sf::Clock clock;

	// time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 40;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1366 / 2) - timeBarStartWidth / 2, 650);

    sf::Time getTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //add sounds
    sf::SoundBuffer bufferChop, bufferDeath, bufferOOF;
    bufferChop.loadFromFile("sound/chop.wav");
    bufferDeath.loadFromFile("sound/death.wav");
    bufferOOF.loadFromFile("sound/out_of_time.wav");
    sf::Sound chop, death, oof;
    chop.setBuffer(bufferChop);
    death.setBuffer(bufferDeath);
    oof.setBuffer(bufferOOF);


    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    bool acceptInput = false;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
		    if(event.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput = true;
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                paused = false;
                score = 0;
                timeRemaining = 6;

                for(int i = 1; i < NUM_BRANCHES; i++)
                {
                    branchPosition[i] = side::NONE;
                }

                spriteRIP.setPosition(675, 2000);
                spritePlayer.setPosition(550, 550);

                acceptInput = true;

                updateBranches(1);
                updateBranches(2);
                updateBranches(3);
                updateBranches(4);
                updateBranches(5);
            }
            if(acceptInput)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    if(playerSide == side::LEFT)
                    {
                        spritePlayer.setScale(sf::Vector2f((spritePlayer.getScale().x * -1), spritePlayer.getScale().y));
                    }
                    playerSide = side::RIGHT;

                    score++;
                    timeRemaining += (2 / score) + .15;

                    spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(840, 550);
                    updateBranches(score);

                    spriteLog.setPosition(570, 610);
                    logSpeedX = -5000;
                    logActive = true;

                    acceptInput = false;

                    chop.play();
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    if(playerSide == side::RIGHT)
                    {
                        spritePlayer.setScale(sf::Vector2f((spritePlayer.getScale().x * -1), spritePlayer.getScale().y));
                    }
                    playerSide = side::LEFT;


                    score++;
                    timeRemaining += (2 / score) + .15;

                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(550, 550);
                    updateBranches(score);

                    spriteLog.setPosition(570, 610);
                    logSpeedX = 5000;
                    logActive = true;

                    acceptInput = false;

                    chop.play();
                }
            }
		}

		//update game

		// start of pause loop
		if(!paused)
        {

        //measure time
        sf::Time dt = clock.restart();
        timeRemaining -= dt.asSeconds();
        timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

        if(timeRemaining <= 0)
        {
            //paused the game
            oof.play();
            paused = true;
            messageText.setString("Out of time!!");
            sf::FloatRect textRect2 = messageText.getLocalBounds();
            messageText.setOrigin(textRect2.left + textRect2.width / 2.0f,
                                  textRect2.top + textRect2.height / 2.0f);
            messageText.setPosition(1366 / 2.0f, 768 / 2.0f);
        }

        //set up the bee
        if(!beeActive)
        {
            srand((int)time(0));
            beeSpeed = (rand() % 200) + 200;

            srand((int)time(0) * 10);
            float height = (rand() % 500) + 200;
            sBee.setPosition(1380, height);
            beeActive = true;
        }
        else
        {
            sBee.setPosition(sBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                             sBee.getPosition().y);

            if(sBee.getPosition().x < -100)
            {
                beeActive = false;
            }
        }

        //set up the clouds
        if(!cloudActive1)
        {
            srand((int)time(0) * 10);
            cloudSpeed1 = (rand() % 200);

            srand((int)time(0) * 10);
            float height = (rand() % 150);
            sClouds[0].setPosition(-200, height);
            cloudActive1 = true;
        }
        else
        {
            sClouds[0].setPosition(sClouds[0].getPosition().x
                                   + (cloudSpeed1 * dt.asSeconds()), sClouds[0].getPosition().y);
            if(sClouds[0].getPosition().x > 1366)
            {
                cloudActive1 = false;
            }
        }

         if(!cloudActive2)
        {
            srand((int)time(0) * 20);
            cloudSpeed2 = (rand() % 200);

            srand((int)time(0) * 20);
            float height = (rand() % 300) - 150;
            sClouds[1].setPosition(-200, height);
            cloudActive2 = true;
        }
        else
        {
            sClouds[1].setPosition(sClouds[1].getPosition().x
                                   + (cloudSpeed2 * dt.asSeconds()), sClouds[1].getPosition().y);
            if(sClouds[1].getPosition().x > 1366)
            {
                cloudActive2 = false;
            }
        }

         if(!cloudActive3)
        {
            srand((int)time(0) * 30);
            cloudSpeed3 = (rand() % 200);

            srand((int)time(0) * 30);
            float height = (rand() % 450) - 150;
            sClouds[2].setPosition(-200, height);
            cloudActive3 = true;
        }
        else
        {
            sClouds[2].setPosition(sClouds[2].getPosition().x
                                   + (cloudSpeed3 * dt.asSeconds()), sClouds[2].getPosition().y);
            if(sClouds[2].getPosition().x > 1366)
            {
                cloudActive3 = false;
            }
        }


        //update the score text
        std::stringstream ss;
        ss << "Score = " << score;
        scoreText.setString(ss.str());

        for(int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 120;
            if(branchPosition[i] == side::LEFT)
            {
                branches[i].setPosition(610, height);
                branches[i].setRotation(180);
            }
            else if(branchPosition[i] == side::RIGHT)
            {
                branches[i].setPosition(800, height);
                branches[i].setRotation(0);
            }
            else
            {
                branches[i].setPosition(3000, height);
            }
        }

        if(logActive)
        {
            spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                                  spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

            if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
            {
                logActive = false;
                spriteLog.setPosition(550, 910);
            }
        }

        if(branchPosition[5] == playerSide)
        {
            paused = true;
            acceptInput = false;

            spriteRIP.setPosition(600, 600);
            spritePlayer.setPosition(2000, 0);
            spriteLog.setPosition(550, 920);
            spriteAxe.setPosition(2000, spriteAxe.getPosition().y);

            death.play();

            messageText.setString("SQUISHED!!");

          //  sf::FloatRect textRect8 = messageText.getGlobalBounds();

          //  messageText.setOrigin(textRect8.left + textRect8.width / 2.0f, textRect8.top + textRect8.height / 2.0f);
            messageText.setPosition(920, 350);
        }

        } // end of pause loop

		window.clear(sf::Color::White);

		window.draw(sBackground);

		for(int k=0; k<3; k++)
			window.draw(sClouds[k]);

        for(int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteLog);
        window.draw(spriteAxe);
        window.draw(spriteRIP);

        window.draw(timeBar);
		window.draw(sBee);


		//draw the text
		window.draw(scoreText);
		if(paused)
        {
            window.draw(messageText);
        }

		window.display();
	}
    return 0;
}

void updateBranches(int seed)
{
    for(int j = NUM_BRANCHES-1; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;
    case 1:
        branchPosition[0] = side::RIGHT;
        break;
    default:
        branchPosition[0] = side::NONE;
        break;
    }
}

