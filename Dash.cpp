#include <iostream>
#include "GUIClass.cpp"
using  std::cout;
using namespace dash;
int main(array<System::String^> ^args)
{
	//array holds all the general starting off test info. General form of a test array is as follows:
	//{test name, default time range, current time range(manipulated later)}
	array<Object^, 2>^ TestArray = gcnew array<Object^, 2>(4, 2){ {"Cat Scan", TimeSpan(7, 0, 0, 0)}, { "ER Visit", TimeSpan(7, 0, 0, 0) }, { "Hospitalization", TimeSpan(31, 0, 0, 0) } };
	GUIClass^ gui = gcnew GUIClass(TestArray);
	Application::Run(gui);

}