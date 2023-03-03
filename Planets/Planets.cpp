// Planets.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Game.h"
#include "GridGameObject.h"
#include "SpaceObject.h"


int main()
{
    Game* testGame = new Game();;

    testGame->GameObjects.push_back(new GridGameObject());

    SpaceObject* cube0 = new SpaceObject(true);

    cube0->SetPosition(Vector3(1, 1, 1));

    SpaceObject* sphere0 = new SpaceObject(false, cube0);
    sphere0->SetPosition(Vector3(1, 1, 1) * 6);
    sphere0->rotationAxis = Vector3(0, 1, 0);
    sphere0->orbitSpeed = -0.5;
    sphere0->rotationSpeed = -3;

    SpaceObject* cube1 = new SpaceObject(true, sphere0);

    cube1->SetPosition(Vector3(1, 0, 1) * 2);
    cube1->rotationAxis = Vector3(0, 1, 0);
    cube1->orbitAxis = Vector3(0, 1, 0);
    cube1->orbitSpeed = -10;
    cube1->rotationSpeed = 100;



    SpaceObject* cube2 = new SpaceObject(true, cube0);

    cube2->SetPosition(Vector3(1, 0, 0) * 4);
    cube2->rotationAxis = Vector3(0, 0, 1);
    cube2->orbitAxis = Vector3(0, 1, 0);

    SpaceObject* sphere1 = new SpaceObject(false, cube0);
    sphere1->SetPosition(Vector3(1, 0, 1) * 10);
    sphere1->rotationAxis = Vector3(0, 1, 0);
    sphere1->orbitSpeed = -0.5;
    sphere1->rotationSpeed = -3;

    testGame->GameObjects.push_back(cube0);
    testGame->GameObjects.push_back(sphere0);
    testGame->GameObjects.push_back(cube1);
    testGame->GameObjects.push_back(cube2);
    testGame->GameObjects.push_back(sphere1);

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
