#pragma once

namespace fw
{
	class ivec2
	{
	public:
		ivec2() :m_x(0), m_y(0){}
		ivec2(int x,int y):m_x(x),m_y(y){}
		int m_x ;
		int m_y ;
	};
}