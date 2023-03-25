#include <Windows.h>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>

extern "C"
{
	__declspec (dllexport) void __cdecl ClickTimes(int times, int delay);
	__declspec (dllexport) void __cdecl Mode();
	__declspec (dllexport) void __cdecl initAutoClicker();
}

std::atomic<bool> Left = true;
std::atomic<bool> TempClickToggle = false;
std::atomic<bool> ToggleClick = false;
std::atomic<int> TimesToClick = 0;
std::atomic<int> LastTimesToClick = 0;
std::atomic<int> Delay = 0;
std::atomic<bool> InitCalled = false;
std::atomic<bool> ToggleEverything = false;

void Mode() {
	Left = !Left;
}

void clickMouse() {
	POINT p;
	if (GetCursorPos(&p)) {
		if (Left) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);

			mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
		}
		else {
			mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);

			mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
		}
	}
}


void ClickHandler() {
	while (ToggleEverything) {
		if (GetAsyncKeyState(VK_F6) & 1) {
			TimesToClick.store(LastTimesToClick.load());
			TempClickToggle = !TempClickToggle;
		}
		if (GetAsyncKeyState(VK_F7) & 1) {
			ToggleClick = !ToggleClick;
		}
	}
}

void ClickTimesThread() {
	while (ToggleEverything) {
		if (TempClickToggle && TimesToClick > 0) {
			clickMouse();
			if (Delay > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
			}
			TimesToClick--;
		}
		if (TimesToClick == 0) {
			TempClickToggle = false;
		}
	}
}

void ClickToggleThread() {
	while (ToggleEverything) {
		if (ToggleClick) {
			clickMouse();
			if (Delay > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
			}
		}
	}
}

void ClickTimes(int times, int delay) {
	Delay = delay;
	LastTimesToClick = times;
	TimesToClick = times;
}

void initAutoClicker() {
	std::unique_ptr<std::thread> clickTimesThread;
	std::unique_ptr<std::thread> clickToggleThread;
	std::unique_ptr<std::thread> clickHandler;
		
	if (!InitCalled) {
		InitCalled = true;
		ToggleEverything = true;
		clickTimesThread = std::make_unique<std::thread>(ClickTimesThread);
		clickTimesThread->detach();
		clickToggleThread = std::make_unique<std::thread>(ClickToggleThread);
		clickToggleThread->detach();
		clickHandler = std::make_unique<std::thread>(ClickHandler);
		clickHandler->detach();
	}
	else {
		ToggleEverything = false;
		InitCalled = false;
	}
}

int main() {
	//   std::string delayString;
	   //std::string clickTimesString;
	//   std::cout << "delay" << std::endl;
	//   std::getline(std::cin, delayString);
	//   std::cout << "clicktimes" << std::endl;
	//   std::getline(std::cin, clickTimesString);
	//   int delay = std::stoi(delayString);
	   //int clickTimes = std::stoi(clickTimesString);
	   //int i = clickTimes;

	//   bool toggle = false;
	//   bool toggle2 = false;
	//   bool toggle3 = false;
	 //  while (true) {
	 //      if (GetAsyncKeyState(VK_F6) & 1) {
	 //          toggle = !toggle;
	 //          if (!toggle) {
	 //              toggle2 = false;
	 //              toggle3 = false;
		   //		i = clickTimes;
	 //          }
	 //      }

	 //      if (GetAsyncKeyState(VK_F7) & 1) {
	 //          toggle2 = !toggle2;
	 //          if (!toggle2) {
	 //              toggle = false;
	 //              toggle3 = false;
	 //              i = clickTimes;
	 //          }
	 //      }

	 //      if (GetAsyncKeyState(VK_F8) & 1) {
	 //          toggle3 = !toggle3;
	 //          if (!toggle3) {
	 //              toggle = false;
	 //              toggle2 = false;
	 //              i = clickTimes;
	 //          }
	 //      }

	 //      if (toggle && i > 0) {
	 //          clickMouse("left");
	 //          if (delay > 0) {
	 //              std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	 //          }
	 //          i--;
	 //      }
	 //      else if (toggle2 && i > 0) {
	 //          clickMouse("right");
	 //          if (delay > 0) {
	 //              std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	 //          }
	 //          i--;
	 //      }

		   //else if (toggle3 && i > 0) {
		   //	clickMouse("other");
	 //          if (i == clickTimes / 2) {
	 //              POINT p;
	 //              if (GetCursorPos(&p)) {
	 //                  SetCursorPos(p.x, p.y + 220);
	 //              }
	 //          }
		   //	if (delay > 0) {
		   //		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		   //	}
		   //	i--;
		   //}
	 //  }
	return 0;
}
