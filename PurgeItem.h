#pragma once
#include <time.h>

namespace Manage{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	ref class PurgeItem : Object
	{
	private: 
		time_t t;
		int index;
	public:
		PurgeItem(time_t t, int index);
		int getIndex();
		time_t getTime();
		virtual int GetHashCode() override;
	};
}
