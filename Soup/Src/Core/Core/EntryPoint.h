#pragma once

#include "Core/Application.h"

extern Soup::Application* Soup::CreateApplication(int argc, char* argv[]);

int main(int argc, char* argv[])
{
  Soup::Application* app = Soup::CreateApplication(argc, argv);

  app->Run();

  delete app;

  return 0;
}
