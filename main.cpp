#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <list>
#include <iostream>
#include "SpriteGameObject.h"
#include "ActiveObject.h"
#include "Utils.h"
#include "InputMgr.h"
#include "Branch.h"
#include "Player.h"
#include "ResourceMgr.h"

using namespace sf;
using namespace std;

void UpdateBranches(vector<Branch*>& branches, int& current, vector<Vector2f>& posArr)
{
    current = (current + 1) % branches.size();

    for (int i = 0; i < branches.size(); ++i)
    {
        int index = (current + i) % branches.size();
        branches[index]->SetPosition(posArr[i]);
        if (i == branches.size() - 1)
        {
            branches[index]->SetSide((Sides)Utils::Range(0, 2));
        }
    }
}

int main()
{
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!", Style::Default);

    //SoundBuffer bgmbuffer;
    //bgmbuffer.loadFromFile("sound/Elinia.wav");
    //Sound bgm;
    //bgm.setBuffer(bgmbuffer);
    //bgm.play();

    //SoundBuffer timeOutBuffer;
    //timeOutBuffer.loadFromFile("sound/out_of_time.wav");

    Sound timeOutSound;
    timeOutSound.setBuffer(*ResourceMgr::GetInstance()->GetSoundBuffer("sound/out_of_time.wav"));

    Texture texBackground;
    texBackground.loadFromFile("graphics/background.png");
    Texture texBee;
    texBee.loadFromFile("graphics/bee.png");
    Texture texCloud;
    texCloud.loadFromFile("graphics/cloud.png");
    Texture texTree;
    texTree.loadFromFile("graphics/tree.png");
    Texture texBranches;
    texBranches.loadFromFile("graphics/branch.png");

    Texture texAxe;
    texAxe.loadFromFile("graphics/-2.png");
    Texture texRip;
    texRip.loadFromFile("graphics/rip.png");
    Texture texPlayer;
    texPlayer.loadFromFile("graphics/player.png");



    vector<SpriteGameObject*> gameobjectlist;
    gameobjectlist.push_back(new SpriteGameObject(texBackground));

    vector<float> startVector = { 2000, -300 };
    vector<float> endVector = { -300, 2000 };
    for (int i = 0; i < 3; ++i)
    {
        auto activeobj = new ActiveObject(texCloud);
        auto index = Utils::Range(0, 2);
        activeobj->Set(
            { 200,400 }, { 100,400 },
            { startVector[index], 0 }, { endVector[index], 0 });
        gameobjectlist.push_back(activeobj);
    }

    Vector2u size = window.getSize();
    SpriteGameObject* tree = new SpriteGameObject(texTree, Vector2f(size.x * 0.5f, 910));
    tree->SetOrigin(Origins::BC);
    gameobjectlist.push_back(tree);

    vector<Branch*> branches(6); //가지가 여섯개
    vector<Vector2f> branchPosArr(branches.size());

    for (int i = 0; i < branches.size(); ++i)
    {
        branches[i] = new Branch(texBranches, tree);
        branches[i]->SetSide((Sides)Utils::Range(0, 2));
        gameobjectlist.push_back(branches[i]);
    }      

    auto activeobj = new ActiveObject(texBee);
    activeobj->Set({ 200,400 }, { 500,1000 }, { 2000,0 }, { -100,0 });
    gameobjectlist.push_back(activeobj);


    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //UI OBJ CREATES
    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(75);
    messageText.setFillColor(Color::Magenta);
    messageText.setString("Press Enter To Start!");

    Utils::SetOrigin(messageText, Origins::MC);
    //FloatRect rect = messageText.getLocalBounds();    
    //messageText.setOrigin(rect.width * 0.5f, rect.height * 0.5f);

    messageText.setPosition(size.x * 0.5f, size.y * 0.5f);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(Color::Yellow);
    
    scoreText.setPosition(20, 20);
    Utils::SetOrigin(scoreText, Origins::TL);

    Vector2f timerBarSize(400, 80);
    RectangleShape timerBar;
    timerBar.setSize(timerBarSize);
    timerBar.setFillColor(Color::Red);
    timerBar.setPosition(size.x * 0.5f - timerBarSize.x * 0.5f, size.y - 100);

    float duration = 10.0f;
    float timer = duration;
    int n = 0;

    bool isPause = true;
    bool isgameover = false;

    Player* player = new Player(texPlayer, texRip, texAxe, n);
    gameobjectlist.push_back(player);

    player->Set(tree->GetPosition());

    for (auto go : gameobjectlist)
    {
        go->Init();
    }

    int currentBranch = 0;
    float x = branches[0]->GetPosition().x;
    float y = 800;
    float offset = branches[0]->GetSize().y;
    offset += 100;

    for (int i = 0; i < branches.size(); ++i)
    {
        branchPosArr[i] = Vector2f(x, y);
        y -= offset;
    }

    UpdateBranches(branches, currentBranch, branchPosArr);

    Clock clock;
    while (window.isOpen())
    {
        scoreText.setString("Score: " + to_string(n));
        Time dt = clock.restart();
        Event ev;
        InputMgr::ClearInput();
        while (window.pollEvent(ev))
        {
            InputMgr::UpdateInput(ev);
        }            

        

        //if (InputMgr::GetKeyDown(Keyboard::A))
        //{
        //    if (timer == 0.f || isPause)
        //    {
        //        continue;
        //    }
        //    n += 10;
        //}

        //if (InputMgr::GetKeyDown(Keyboard::S))
        //{
        //    if (timer == 0.f || isPause)
        //    {
        //        continue;
        //    }
        //    n -= 10;
        //}

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
            continue;
        }


        if (InputMgr::GetKeyDown(Keyboard::Key::Left) || InputMgr::GetKeyDown(Keyboard::Key::Right))
        {
            if (!isPause)
            {
                UpdateBranches(branches, currentBranch, branchPosArr);
                ++timer;
                timer = min(timer, 10.f);
            }       
        }

        if (InputMgr::GetKeyDown(Keyboard::Return))
        {
            if (isgameover || !player->GetAlive())
            {
                messageText.setString("Press Enter To Start!");
                Utils::SetOrigin(messageText, Origins::MC);
                isgameover = !isgameover;
                timer = duration;
                n = 0;
                for (auto obj : gameobjectlist)
                {
                    obj->Init();
                }
                auto pos = player->GetPos();
                pos = pos == Sides::Left ? Sides::Right : Sides::Left;
                branches[currentBranch]->SetSide(pos);
                isPause = true;
            }
            else
                isPause = !isPause;
        }

        size = window.getSize();
        float deltatime = isPause ? 0.f : dt.asSeconds();

        // UI Update
        timer -= deltatime;
        if (timer < 0.f)
        {
            if (!isPause)
            {
                timeOutSound.setVolume(50);
                timeOutSound.play();
            }
            timer = 0.f;
            isPause = true;
            isgameover = true;
            messageText.setString("Time Over!");
            Utils::SetOrigin(messageText, Origins::MC);
        }

        float normTime = timer / duration;
        float timerSizeX = timerBarSize.x * normTime;
        timerBar.setSize({ timerSizeX, timerBarSize.y });
        timerBar.setPosition(size.x * 0.5f - timerSizeX * 0.5f, size.y - 100);

        //Update
        for (auto go : gameobjectlist)
        {
            go->Update(deltatime);
        }
        //Draw
        window.clear();
        for (auto go : gameobjectlist)
        {
            go->Draw(window);
        }

        if (player->GetPos() == branches[currentBranch]->GetSide())
        {
            player->Die();
            isPause = true;
            isgameover = true;
            messageText.setString("Game Over!");
            Utils::SetOrigin(messageText, Origins::MC);
        }
        if (isPause)
        {
            window.draw(messageText);
        }

        window.draw(scoreText);
        window.draw(timerBar);

        window.display();
    }

    for (auto go : gameobjectlist)
    {
        go->Release();
        delete go;
    }
    gameobjectlist.clear();
    return 0;
}