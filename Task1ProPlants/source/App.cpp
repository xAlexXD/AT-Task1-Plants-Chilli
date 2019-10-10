#include "..\includes\App.h"
#include <sstream>
#include <iomanip>

App::App() : _wnd(800, 600, "AT Task1 Proc Plants")
{
}

int App::Go()
{
	while (true)
	{
		//processes messages pending but doesnt block application flow. If ecode has a value after getting fed optional from process messages its time to quit
		if (const auto ecode = Window::ProcessMessages())
		{
			//if the returned optional actually has a value its time to leave
			return *ecode;
		}
		//Else just carry on with normal execution
		DoFrame();
	}
}

void App::DoFrame()
{
	const float t = _timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	_wnd.SetTitle(oss.str());
}
