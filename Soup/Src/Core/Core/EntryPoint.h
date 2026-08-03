#pragma once

#include "Core/Application.h"

extern Soup::Application* Soup::CreateApplication();

int main(int, char*[])
{
  Soup::Log::Init();

  Soup::Application* app = Soup::CreateApplication();
  app->Run();
  delete app;

  return 0;
}
