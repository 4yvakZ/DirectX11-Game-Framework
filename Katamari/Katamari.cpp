// Katamari.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Game.h"

#include "GridGameObject.h"
#include "RollingBall.h"
#include "PlaneObject.h"
#include "StickyObject.h"
#include "RenderComponentFBX.h"


int main()
{
    Game* testGame = new Game();

    float ballR = 0.425;
    float ballScale = 0.8;
    RollingBall* ball = new RollingBall(ballR * ballScale);
    ball->SetPosition(Vector3(0, ballR * ballScale, 0));
    ball->SetScale(ballScale);

    //testGame->GameObjects.push_back(new GridGameObject());
    testGame->GameObjects.push_back(ball);
    testGame->GameObjects.push_back(new PlaneObject());

    StickyObject* cube = new StickyObject("../FBX/cube.fbx");
    cube->SetPosition(Vector3(5, 1, 5));
    cube->SetScale(1);
    cube->radius = 1.1;
    testGame->GameObjects.push_back(cube);

    StickyObject* sphere = new StickyObject("../FBX/sphere.fbx");
    sphere->SetPosition(Vector3(-5, 0.7, -5));
    sphere->SetScale(0.7);
    sphere->radius = 0.7;
    testGame->GameObjects.push_back(sphere);

    StickyObject* guitar = new StickyObject("../FBX/guitar/guitar.fbx", "../FBX/guitar/TD_Checker.png");
    guitar->FbxRenderComponent->isZUp = false;
    guitar->SetPosition(Vector3(5, 0, -5));
    guitar->SetScale(4);
    guitar->SetCenterPoint(Vector3(0, 0.08f, 0));
    guitar->radius = 0.9;
    testGame->GameObjects.push_back(guitar);

    StickyObject* pillBottle = new StickyObject("../FBX/pill_bottle/pill_bottle.fbx", "../FBX/pill_bottle/TD_Checker.png");
    pillBottle->SetPosition(Vector3(-5, 0, 5));
    pillBottle->SetScale(4);
    pillBottle->SetCenterPoint(Vector3(0, 0.02f, 0));
    pillBottle->radius = 0.3;
    testGame->GameObjects.push_back(pillBottle);

    StickyObject* mug = new StickyObject("../FBX/Winter_Mug/Winter_Mug.fbx", "../FBX/Winter_Mug/Winter_Mug.png");
    mug->SetPosition(Vector3(0, 0, 5));
    mug->SetScale(12);
    mug->radius = 0.5;
    mug->SetCenterPoint(Vector3(0, 0.004f, 0));
    testGame->GameObjects.push_back(mug);

    testGame->Run();

    delete testGame;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
