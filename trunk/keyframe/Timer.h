#pragma once
#include "utilitis.h"
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

using namespace boost;
using namespace boost::posix_time;


class CTimer
{



public:
	CTimer(void)
	{
		m_ElapsedTimerStart =  microsec_clock::local_time();
		m_LastFpsUpdate = microsec_clock::local_time();
		m_Fps = 5;
		counter = 0;

	};

	float  m_Fps;
	ptime  m_LastFpsUpdate;
	int counter;

	ptime  m_ElapsedTimerStart;
	//==================================================================================
	bool TimeElapsed(int Miliseconds = 1000)
	{
		time_duration diff = microsec_clock::local_time() - m_ElapsedTimerStart;
		if ( diff.total_milliseconds() >=Miliseconds )
		{
			m_ElapsedTimerStart = microsec_clock::local_time();
			return true;
		};
		return false;
	}
	//==================================================================================
	void UpdateFps()
	{

		ptime now =  microsec_clock::local_time();
		float newFps = m_Fps =float( 1000.0 / ( now - m_LastFpsUpdate ).total_milliseconds() );
		m_Fps = m_Fps  * 0.99 + 0.01 * newFps;
		m_Fps = newFps;
		m_LastFpsUpdate = microsec_clock::local_time();
		if (counter == 0)
			cout<<"FPS:"<<m_Fps <<endl;
		counter ++;
		if (counter > 10) 
			counter = 0;
	};
	//==================================================================================
};
