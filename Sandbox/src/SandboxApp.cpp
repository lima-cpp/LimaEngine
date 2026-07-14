#include "Lima.h"

class Sandbox : public Lima::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Lima::Application* Lima::CreateApplication()
{
	return new Sandbox();
}