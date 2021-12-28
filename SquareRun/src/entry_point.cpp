#include <serialization/config.h>
#include <iostream>

int main(int argc, char** argv)
{
	int x = Serialization::GetConfigElement<int>("window", "width");
	return 0;
}